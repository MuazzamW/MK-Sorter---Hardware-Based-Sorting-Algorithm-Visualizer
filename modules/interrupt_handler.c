#include "address_map.h"
#include "interrupt_handler.h"

#define CLOCK_RATE 1000000
#define EXIT_SUCCESS 0
//interrupt handler address
static void handler(void) __attribute__ ((interrupt("machine")));
void set_interval_timer(void);
void interval_timer_ISR(void);

//MMIO POINTERS
volatile int * timer_ptr = (int *) TIMER_BASE;

int setup_interrupts(void){

    set_interval_timer();

    int mstatus_value, mtvec_value, mie_value;
    mstatus_value = 0b1000; // interrupt bit mask
    // disable interrupts
    __asm__ volatile ("csrc mstatus, %0" :: "r"(mstatus_value));
    mtvec_value = (int) &handler; // set trap address
    __asm__ volatile ("csrw mtvec, %0" :: "r"(mtvec_value));
    // disable all interrupts that are currently enabled
    __asm__ volatile ("csrr %0, mie" : "=r"(mie_value));
    __asm__ volatile ("csrc mie, %0" :: "r"(mie_value));
    mie_value = 0x50088; // KEY, itimer, mtimer, SW interrupts
    // set interrupt enables
    __asm__ volatile ("csrs mie, %0" :: "r"(mie_value));
    // enable Nios V interrupts
    __asm__ volatile ("csrs mstatus, %0" :: "r"(mstatus_value));


    return EXIT_SUCCESS;
}

void handler(void){
    int mcause_value;
    __asm__ volatile ("csrr %0, mcause" : "=r"(mcause_value));
    if(mcause_value ==  0x80000010){
        //this is due to the interval timer
        interval_timer_ISR();
    }
}


void interval_timer_ISR(){
    //clear the intterupt
    *timer_ptr = 0;
    //figure out what to do

}

void set_interval_timer(){
    int loadValue = CLOCK_RATE;
    *(timer_ptr + 2) = (loadValue & 0xFFFF);
    *(timer_ptr + 3) = (loadValue>>16) & 0xFFFF;
    *(timer_ptr + 1) = 0x7;
}
