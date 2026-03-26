#include "address_map.h"
#include "interrupt_handler.h"
#include <stdint.h>

#define CLOCK_RATE 1000000
#define EXIT_SUCCESS 0
#define VGA_WIDTH 320
#define VGA_HEIGHT 240
#define MOUSE_RESOLUTION 2 // 2 counts/mm
#define MAX_MOUSE_COUNT 250 //maximum displcement in either x or y direction as +- 250 counts
#define MOUSE_PIXELS_PER_COUNT VGA_WIDTH/(2*MAX_MOUSE_COUNT)

typedef struct {
    volatile double dx;
    volatile double dy;
    volatile char flags;
    volatile bool leftButtonClicked;
    volatile bool rightButtonClicked;

    volatile double x;
    volatile double y;

}mouse_packet;


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

//MMIO POINTERS
volatile int * timer_ptr = (int *) TIMER_BASE;
struct mouse_ptr *const mousep = ((struct mouse_ptr *) PS2_BASE);

//interrupt handler address
static void handler(void) __attribute__ ((interrupt("machine")));
void set_interval_timer(void);
void interval_timer_ISR(void);
void mouse_handler_ISR(mouse_packet* p);


//helper functions for mouse input:
static int ps2_byte_available(void) {
    return (mousep->rValid & 0x80) != 0;  
}

static uint8_t ps2_read_byte(void) {
    return (uint8_t)(mousep->data);
}

static void ps2_write_byte(uint8_t b) {
    mousep->data = b;
}

mouse_packet get_mouse_packet(void){
    return mouse_info;
}

void ps2_mouse_init(void){
    char byte1, byte2, byte3;

    ps2_write_byte(0xFF); //reset
    for (volatile int i = 0; i < 1000000; i++) { }

    while (ps2_byte_available()) {
        byte1 = byte2;
        byte2 = byte3;
        byte3 = mousep->data;
        if((byte2 == (char)0xAA) && (byte3 == (char)0x00)){

            //ready to send info
            //change resolution from 4 counts/mm to 2 counts/mm
            ps2_write_byte(0xE8);
            while(ps2_read_byte()!=0xFA){}
            ps2_write_byte(0x01); // set resolution to 2 counts/mm

            mousep->reField = 0x1; //set RE bit to 1 to request an interrupt
            mousep->data = 0xF4; //set mouse into 
        }
        
    }

    ps2_write_byte(0xF4);
}

int setup_interrupts(void){
    int mstatus_value, mtvec_value, mie_value;

    //set initial mouse location
    mouse_info.x = VGA_WIDTH/2;
    mouse_info.y = VGA_HEIGHT/2;

    // disable interrupts
    __asm__ volatile ("csrc mstatus, %0" :: "r"(mstatus_value));

    //configure external hardware
    set_interval_timer();

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

}

void handler(void){
    int mcause_value;
    __asm__ volatile ("csrr %0, mcause" : "=r"(mcause_value));
    mcause_value = mcause_value & 0x7FFFFFFF;
    if(mcause_value ==  16){
        //this is due to the interval timer
        interval_timer_ISR();
    }else if(mcause_value == 18){
        //KEYISR
    }else if(mcause_value == 22){
        mouse_handler_ISR(&mouse_info);
    }
}

void mouse_handler_ISR(mouse_packet *p){
    static char buffer[3];
    int count;
    while(ps2_byte_available()){
        char byte = ps2_read_byte();
        if (count == 0 && !(byte & 0x08)){
            continue;
        }
        buffer[count++] = byte;
        if(count==3){
            p->flags = buffer[0];
            p->dx = (double)buffer[1];
            p->dy = (double)buffer[2];

            p->leftButtonClicked = (buffer[0]&0x1 == 1 ? true : false);
            p->rightButtonClicked = (buffer[0]&0x2 == 1 ? true : false);

            if(buffer[0] & 0x10){
                //x displacement is negative
                p->x -= p->dx * MOUSE_PIXELS_PER_COUNT;
            }else{
                p->x += p->dx*MOUSE_PIXELS_PER_COUNT;
            }
            if(buffer[0] & 0x20){
                //y displacement negative
                p->y -= p->dy * MOUSE_PIXELS_PER_COUNT;
            }else{
                p->y += p->dy * MOUSE_PIXELS_PER_COUNT;
            }
            count = 0;
        }
            
    }

}


void interval_timer_ISR(){
    //clear the intterupt
    *timer_ptr = 0;
    //printf("timer triggered\n");
}

void set_interval_timer(){
    int loadValue = CLOCK_RATE;
    *(timer_ptr + 2) = (loadValue & 0xFFFF);
    *(timer_ptr + 3) = (loadValue>>16) & 0xFFFF;
    *(timer_ptr + 1) = 0x7;
}
