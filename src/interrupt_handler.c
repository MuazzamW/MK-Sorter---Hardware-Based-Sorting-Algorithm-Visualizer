#include "interrupt_handler.h"

#define EXIT_SUCCESS 0
#define VGA_WIDTH 320
#define VGA_HEIGHT 240
#define MOUSE_RESOLUTION 2 // 2 counts/mm
#define MAX_MOUSE_COUNT 250 //maximum displcement in either x or y direction as +- 250 counts
#define MOUSE_PIXELS_PER_COUNT_X ((double)VGA_WIDTH / (2.0 * MAX_MOUSE_COUNT))
#define MOUSE_PIXELS_PER_COUNT_Y ((double)VGA_HEIGHT / (2.0 * MAX_MOUSE_COUNT))
#define MOUSE_GAIN_X 1
#define MOUSE_GAIN_Y 1

struct mouse_ptr {

    volatile unsigned char data;
    volatile unsigned char rValid;
    volatile unsigned short rAvail;
    volatile unsigned char reField; // for enabling interrupts
    volatile unsigned char pendingAndError; // bits to indicate pending interrupt and error when sending message
    volatile unsigned short unused;
};

volatile mouse_packet mouse_info;
volatile bool mouseClicked;

volatile int arrayStep = 0;
volatile bool currentlyDisplaying = false;
volatile int keyNumber = -1;

//MMIO POINTERS
volatile int * timer_ptr = (int *) TIMER_BASE;
struct mouse_ptr *const mousep = ((struct mouse_ptr *) PS2_BASE);
volatile int* key_ptr = (int*) KEY_BASE;

//interrupt handler address
static void handler(void) __attribute__ ((interrupt("machine")));
void set_interval_timer(void);
void interval_timer_ISR(void);
void key_ISR(void);
void mouse_handler_ISR(volatile mouse_packet* p);


//helper functions for mouse input:
static int ps2_byte_available(void) {
    return (mousep->rValid & 0x80) != 0;  
}

static char ps2_read_byte(void) {
    return (char)(mousep->data);
}

static void ps2_write_byte(char b) {
    mousep->data = b;
}

static char ps2_wait_byte(void){
    while(!ps2_byte_available()){}
    return ps2_read_byte();
}

static inline void ps2_enable_irq(void) {
    mousep->reField = 0x1;   // RE bit
}

mouse_packet get_mouse_packet(void){
    return mouse_info;
}

//reads unsigned char for movement information and converts it into a signed 9 bit number using the signed bit flags
static int ps2_decode_delta(unsigned char flags, unsigned char data, int sign_mask) {
    int v = data | ((flags & sign_mask) ? 0x100 : 0);
    if (v & 0x100) v -= 0x200;
    return v;
}


void ps2_mouse_init(void){
    printf("entered mouse init\n");
    char b;

    ps2_write_byte(0xFF); //reset
    b = ps2_wait_byte(); //expect FA

    printf("reset ack = 0x%02X\n", b);

    b = ps2_wait_byte();           // expect AA
    printf("self test = 0x%02X\n", b);

    b = ps2_wait_byte();           // expect 00
    printf("mouse id   = 0x%02X\n", b);

    ps2_write_byte(0xE8);          // set resolution
    b = ps2_wait_byte();           // expect FA
    printf("E8 ack     = 0x%02X\n", b);

    ps2_write_byte(0x01);          // 1 counts/mm
    b = ps2_wait_byte();           // expect FA
    printf("res ack    = 0x%02X\n", b);

    ps2_enable_irq();              // enable PS/2 core interrupt

    ps2_write_byte(0xF4);          // enable streaming mode
    b = ps2_wait_byte();           // expect FA
    printf("F4 ack     = 0x%02X\n", b);
}

int set_up_interrupt_handler(void){
    //printf("entered interrupt handler\n");
    int mstatus_value, mtvec_value, mie_value;

    //set initial mouse location
    mouse_info.x = VGA_WIDTH/2;
    mouse_info.y = VGA_HEIGHT/2;

    mstatus_value = 0x8;
    // disable interrupts
    __asm__ volatile ("csrc mstatus, %0" :: "r"(mstatus_value));

    //configure external hardware
    set_interval_timer();
    ps2_mouse_init();
    *(key_ptr+3) = 0xF;//clear keys
    *(key_ptr+2) = 0xF; //initialize key interrupts

    mstatus_value = 0b1000; // interrupt bit mask

    mtvec_value = (int) &handler; // set trap address
    __asm__ volatile ("csrw mtvec, %0" :: "r"(mtvec_value));
    // disable all interrupts that are currently enabled
    __asm__ volatile ("csrr %0, mie" : "=r"(mie_value));
    __asm__ volatile ("csrc mie, %0" :: "r"(mie_value));

    mie_value = 0x450000; // KEY, itimer, PS/2
    // set interrupt enables
    __asm__ volatile ("csrs mie, %0" :: "r"(mie_value));
    // enable Nios V interrupts
    __asm__ volatile ("csrs mstatus, %0" :: "r"(mstatus_value));

    return EXIT_SUCCESS;

}

void handler(void){
    int mcause_value;
    __asm__ volatile ("csrr %0, mcause" : "=r"(mcause_value));
    mcause_value = mcause_value & 0x7FFFFFFF;
    if(mcause_value ==  16){
        //this is due to the interval timer
        interval_timer_ISR();
    }else if(mcause_value == 18){
        key_ISR();
    }else if(mcause_value == 22){
        mouse_handler_ISR(&mouse_info);
    }
}

void key_ISR(void){
    printf("entered edge isr\n");

    int edgeCapture = *(key_ptr+3);
    *(key_ptr+3) = 0b1111;
    keyNumber = edgeCapture;
}

void mouse_handler_ISR(volatile mouse_packet *p){
    //printf("entered mouse handler ISR\n");
    static unsigned char buffer[3];
    static int count = 0;

    while (ps2_byte_available()) {
        unsigned char byte = (unsigned char)ps2_read_byte();

        if (count == 0 && !(byte & 0x08)) {
            continue;   // wait for a valid first packet byte
        }

        buffer[count++] = byte;

        if (count == 3) {
            unsigned char f = buffer[0]; //flag
            if (f & 0xC0) {
                count = 0;
                return;
            }

            p->flags = buffer[0];
            p->dx = ps2_decode_delta(f, buffer[1], 0x10);
            p->dy = ps2_decode_delta(f, buffer[2], 0x20);

            if (p->dx < -50 || p->dx > 50 || p->dy < -50 || p->dy > 50) {
                count = 0;
                return;
            }

            p->x += p->dx*MOUSE_GAIN_X;
            p->y -= p->dy*MOUSE_GAIN_Y; 

            if (p->x < 0) p->x = 0;
            if (p->x > VGA_WIDTH - 1) p->x = VGA_WIDTH - 1;

            if (p->y < 0) p->y = 0;
            if (p->y > VGA_HEIGHT - 1) p->y = VGA_HEIGHT - 1;

            p->leftButtonClicked  = (buffer[0] & 0x01) != 0;
            p->rightButtonClicked = (buffer[0] & 0x02) != 0;

            count = 0;

            // if (p->leftButtonClicked) {
            //     printf("left button clicked: %d\n", p->leftButtonClicked);
            // }
            // if (p->rightButtonClicked) {
            //     printf("right button clicked: %d\n", p->rightButtonClicked);
            // }

            // printf("x location %d\n",p->x);
            // printf("y location %d\n",p->y);

        }
    }
}


void interval_timer_ISR(){
    //clear the intterupt
    *timer_ptr = 0;
    if(currentlyDisplaying){
        arrayStep++;
    }
    //printf("timer triggered\n");

}

void set_interval_timer(){
    int loadValue = LOAD_VALUE;
    *(timer_ptr + 2) = (loadValue & 0xFFFF);
    *(timer_ptr + 3) = (loadValue>>16) & 0xFFFF;
    *(timer_ptr + 1) = 0x7;
}