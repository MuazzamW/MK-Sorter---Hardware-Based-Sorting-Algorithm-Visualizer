// --- Start of modules/address_map.h ---
/*******************************************************************************
 * This file provides address values that exist in the DE1-SoC Computer
 ******************************************************************************/

#ifndef __SYSTEM_INFO__
#define __SYSTEM_INFO__

#define BOARD				"DE1-SoC"

/* Memory */
#define DDR_BASE			0x40000000
#define DDR_END				0x7FFFFFFF
#define A9_ONCHIP_BASE			0xFFFF0000
#define A9_ONCHIP_END			0xFFFFFFFF
#define SDRAM_BASE			0x00000000
#define SDRAM_END			0x03FFFFFF
#define FPGA_PIXEL_BUF_BASE		0x08000000
#define FPGA_PIXEL_BUF_END		0x0803FFFF
#define FPGA_CHAR_BASE			0x09000000
#define FPGA_CHAR_END			0x09001FFF

/* Cyclone V FPGA devices */
#define LED_BASE			0xFF200000
#define LEDR_BASE			0xFF200000
#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#define SW_BASE				0xFF200040
#define KEY_BASE			0xFF200050
#define JP1_BASE			0xFF200060
#define JP2_BASE			0xFF200070
#define PS2_BASE			0xFF200100
#define PS2_DUAL_BASE			0xFF200108
#define JTAG_UART_BASE			0xFF201000
#define IrDA_BASE			0xFF201020
#define TIMER_BASE			0xFF202000
#define TIMER_2_BASE			0xFF202020
#define AV_CONFIG_BASE			0xFF203000
#define RGB_RESAMPLER_BASE    		0xFF203010
#define PIXEL_BUF_CTRL_BASE		0xFF203020
#define CHAR_BUF_CTRL_BASE		0xFF203030
#define AUDIO_BASE			0xFF203040
#define VIDEO_IN_BASE			0xFF203060
#define EDGE_DETECT_CTRL_BASE		0xFF203070
#define ADC_BASE			0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE			0xFF709000
#define I2C0_BASE			0xFFC04000
#define I2C1_BASE			0xFFC05000
#define I2C2_BASE			0xFFC06000
#define I2C3_BASE			0xFFC07000
#define HPS_TIMER0_BASE			0xFFC08000
#define HPS_TIMER1_BASE			0xFFC09000
#define HPS_TIMER2_BASE			0xFFD00000
#define HPS_TIMER3_BASE			0xFFD01000
#define FPGA_BRIDGE			0xFFD0501C

#endif

// --- End of modules/address_map.h ---

// --- Start of modules/interrupt_handler.c ---
// Stripped local include: #include "address_map.h"
//#include "interrupt_handler.h"

#define CLOCK_RATE 100000
#define EXIT_SUCCESS 0


#include <stdio.h>

//interrupt handler address
static void handler(void) __attribute__ ((interrupt("machine")));
void set_interval_timer(void);
void interval_timer_ISR(void);

//MMIO POINTERS
volatile int * timer_ptr = (int *) TIMER_BASE;

int main(void){
    int mstatus_value = 0x8,mtvec_value, mie_value;

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
        //mouseISR
    }
}


void interval_timer_ISR(){
    //clear the intterupt
    *timer_ptr = 0;
    printf("timer triggered\n");
}

void set_interval_timer(){
    int loadValue = CLOCK_RATE;
    *(timer_ptr + 2) = (loadValue & 0xFFFF);
    *(timer_ptr + 3) = (loadValue>>16) & 0xFFFF;
    *(timer_ptr + 1) = 0x7;
}

// --- End of modules/interrupt_handler.c ---

