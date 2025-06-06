/* Port Initialization Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : ports.c
  Description:  This file contains the port initializations for all 6 ports
  Programmer: Connor Savugot
  Date Created: Sep 13, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
*/

// #include as of 11-27-24
//// Header Files
#include "ADC.h"
#include "DAC.h"
#include "Display.h"
#include "functions.h"
#include "interrupts.h"
#include "IR.h"
#include "IOT.h"
#include "LCD.h"
#include "macros.h"
#include "menu.h"
#include "motors.h"
#include "msp430.h"
#include "ports.h"
#include "serial.h"
#include "switches.h"
#include "timers.h"
//// Libraries
#include <stdio.h>
#include <string.h>

// Global Variables declared and referenced in Header file


//Init_Ports
void Init_Ports(){
    Init_Port1();
    Init_Port2();
    Init_Port3();
    Init_Port4();
    Init_Port5();
    Init_Port6();
}

// Code from Analog IO Main Slides
void Init_Port1(void){
    P1OUT = 0x00;
    P1DIR = 0x00;

    P1SEL0 &= ~RED_LED; // Set RED_LED as GP I/O
    P1SEL1 &= ~RED_LED; // Set RED_LED as GP I/O
    P1OUT &= ~RED_LED; // Set Red LED On
    P1DIR |= RED_LED; // Set Red LED direction to output

    P1SELC |= V_A1_SEEED; // ADC input for A1_SEEED
    P1SELC |= V_DETECT_L; // ADC input for V_DETECT_L
    P1SELC |= V_DETECT_R; // ADC input for V_DETECT_R
    P1SELC |= V_A4_SEEED; // ADC input for V_A4_SEEED
    P1SELC |= V_THUMB; // ADC input for V_THUMB

    P1SEL0 |= UCA0TXD; // UCA0TXD pin
    P1SEL1 &= ~UCA0TXD; // UCA0TXD pin
    P1SEL0 |= UCA0RXD; // UCA0RXD pin
    P1SEL1 &= ~UCA0RXD; // UCA0RXD pin
}

void Init_Port2(void){  // Configure Port 2
    P2OUT = 0x00;   // P2 set Low
    P2DIR = 0x00;   // Set P2 direction to output

    P2SEL0 &= ~SLOW_CLK;    // SLOW_CLK GPIO operation
    P2SEL1 &= ~SLOW_CLK;    // SLOW_CLK GPIO operation
    P2OUT  &= ~SLOW_CLK;    // Initial Value = Low / Off
    P2DIR  |=  SLOW_CLK;    // Direction = output

    P2SEL0 &= ~CHECK_BAT;   // CHECK_BAT GPIO operation
    P2SEL1 &= ~CHECK_BAT;   // CHECK_BAT GPIO operation
    P2OUT  &= ~CHECK_BAT;   // Initial Value = Low/ Off
    P2DIR  |=  CHECK_BAT;   // Direction = output

    P2SEL0 &= ~IR_LED;  // P2_2 GPIO operation
    P2SEL1 &= ~IR_LED;  // P2_2 GPIO operation
    P2OUT  &= ~IR_LED;  // Initial Value = Low / Off
    P2DIR  |=  IR_LED;  // Direction = input

    P2SEL0 &= ~SW2; // SW1 GPIO operation
    P2SEL1 &= ~SW2; // SW1 GPIO operation
    P2DIR  &= ~SW2; // Direction = input
    P2REN  |=  SW2; // Enable pullup resistor
    P2OUT  |=  SW2; // Configure pull-up resistor SW1
    P2IES  |=  SW2; // SW1 Hi/Lo edge interrupt
    P2IFG  &= ~SW2; // IFG SW1 cleared
    P2IE   |=  SW2; // SW1 interrupt Enabled

    P2SEL0 &= ~IOT_RUN_RED;
    P2SEL1 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
    P2OUT  &= ~IOT_RUN_RED; // Initial Value = Low / Off
    P2DIR  |=  IOT_RUN_RED; // Direction = input

    P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
    P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
    P2OUT  |=  DAC_ENB; // Initial Value = High
    P2DIR  |=  DAC_ENB; // Direction = output

    P2SEL0 &= ~LFXOUT;  // LFXOUT Clock operation
    P2SEL1 |= LFXOUT;   // LFXOUT Clock operation

    P2SEL0 &= ~LFXIN;   // LFXIN Clock operation
    P2SEL1 |= LFXIN;    // LFXIN Clock operation
}

void Init_Port3(void){ //Configure Port 3
    P3SEL0 &= ~TEST_PROBE;
    P3SEL1 &= ~TEST_PROBE;
    P3OUT  &= ~TEST_PROBE;
    P3DIR  |= TEST_PROBE;

    P3SEL0 &= ~OA2O;
    P3SEL1 &= ~OA2O;
    P3OUT  &= ~OA2O;
    P3DIR  |=  OA2O;

    P3SEL0 &= ~OA2N;
    P3SEL1 &= ~OA2N;
    P3OUT  &= ~OA2N;
    P3DIR  |=  OA2N;

    switch(p3_4_type){
    case USE_GPIO:
        P3SEL0 &= ~SMCLK_OUT;
        P3SEL1 &= ~SMCLK_OUT;
        P3OUT  &= ~SMCLK_OUT;
        P3DIR  &= ~SMCLK_OUT;
        break;

    case USE_SMCLK:
        P3SEL0 |= SMCLK_OUT;
        P3SEL1 &= ~SMCLK_OUT;
        P3OUT  &= ~SMCLK_OUT;
        P3DIR  |= SMCLK_OUT;
        break;

    default:
        P3SEL0 &= ~SMCLK_OUT;
        P3SEL1 &= ~SMCLK_OUT;
        P3OUT  &= ~SMCLK_OUT;
        P3DIR  |= SMCLK_OUT;
        break;
    }

    P3SEL0 &= ~DAC_CNTL;
    P3SEL1 &= ~DAC_CNTL;
    P3OUT  &= ~DAC_CNTL;
    P3DIR  |=  DAC_CNTL;

    P3SEL0 &= ~IOT_LINK_GRN;
    P3SEL1 &= ~IOT_LINK_GRN;
    P3OUT  &= ~IOT_LINK_GRN;
    P3DIR  |=  IOT_LINK_GRN;

    P3SEL0 &= ~IOT_EN;
    P3SEL1 &= ~IOT_EN;
    P3OUT  &= ~IOT_EN;
    P3DIR  |=  IOT_EN;
}

void Init_Port4(void){ // Configure PORT 4
    P4OUT = 0x00; // P4 set Low
    P4DIR = 0x00; // Set P4 direction to output

    P4SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P4SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P4OUT &= ~RESET_LCD; // Initial Value = Low / Off
    P4DIR |= RESET_LCD; // Direction = output

    P4SEL0 &= ~SW1; // SW1 GPIO operation
    P4SEL1 &= ~SW1; // SW1 GPIO operation
    P4DIR  &= ~SW1; // Direction = input
    P4REN  |=  SW1; // Enable pullup resistor
    P4OUT  |=  SW1; // Configure pull-up resistor SW1
    P4IES  |=  SW1; // SW1 Hi/Lo edge interrupt
    P4IFG  &= ~SW1; // IFG SW1 cleared
    P4IE   |=  SW1; // SW1 interrupt Enabled

    P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation
    P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

    P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
    P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation

    P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
    P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output

    P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
    P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

    P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
    P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

    P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
    P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
}





// Code from Analog IO Main Slides
void Init_Port5(void){
    P5OUT = 0x00;
    P5DIR = 0x00;

    P5SELC |= V_BAT; // ADC input for V_BAT
    P5SELC |= V_5_0; // ADC input for V_BAT
    P5SELC |= V_DAC; // ADC input for V_DAC
    P5SELC |= V_3_3; // ADC input for V_3_3
    P5SEL0 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
    P5SEL1 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
    P5OUT |= IOT_BOOT; // Set Output value inactive
    P5DIR |= IOT_BOOT; // Set direction to output

}

void Init_Port6(void){

    P6OUT = 0x00;
    P6DIR = 0x00;

//SEL has to be 01 for tb3 configuration


//Pin 3
    P6SEL0 |= R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6DIR |= R_FORWARD; //set output Right Foward
   // P6OUT &= ~R_FORWARD;

//Pin 1
    P6SEL0 |= R_REVERSE;
    P6SEL1 &= ~R_REVERSE;
    P6DIR |= R_REVERSE; //set outut for reverse Right
  //  P6OUT &= ~R_REVERSE;


//Pin 4
    P6SEL0 |= L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6DIR |= L_FORWARD; //set to output Left foward
    //P6OUT &= ~L_FORWARD;

//Pin 2
    P6SEL0 |= L_REVERSE;
    P6SEL1 &= ~L_REVERSE;
    P6DIR |= L_REVERSE; //set output for reverse left
   // P6OUT &= ~L_REVERSE;

//Pin 0
    P6SEL0 |= LCD_BACKLITE;
    P6SEL1 &= ~LCD_BACKLITE;
    P6DIR |= LCD_BACKLITE;
    //P6OUT |= LCD_BACKLITE;

//Pin 5
    P6SEL0 &= ~P6_5;
    P6SEL1 &= ~P6_5;
    P6DIR &= ~P6_5;
    P6OUT &= ~P6_5;


 //Pin 6
    P6SEL0 &= ~GRN_LED;
    P6SEL1 &= ~GRN_LED;
    P6DIR |= GRN_LED;
    P6OUT &= ~GRN_LED;


}




