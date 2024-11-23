
#include "ports.h"
#include "macros.h"
#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"

void Init_Port6(void);

//------------------------------------------------------------------------------
// Name: Init_Ports()
// Description: calls private functions to initialize ports 1,2,3,4,5, and 6
//------------------------------------------------------------------------------
void Init_Ports(){
    Init_Port1();
    Init_Port2();
    Init_Port3();
    Init_Port4();
    Init_Port5();
    Init_Port6();
}

//------------------------------------------------------------------------------
// Name: Init_Port1()
// Description: Configures pins for port 1
//------------------------------------------------------------------------------
void Init_Port1(){
     P1OUT = 0x00; // P1 set Low
     P1DIR = 0x00; // Set all P1 direction to input initially

     P1SEL0 &= ~RED_LED; // RED_LED GPIO operation
     P1SEL1 &= ~RED_LED; // RED_LED GPIO operation
     P1OUT |= RED_LED; // Set Red LED On
     P1DIR |= RED_LED; // Set Red LED direction to output

     P1SELC |= V_A1_SEEED; // ADC input for A1_SEED
     P1SELC |= V_DETECT_L; // ADC input for V_DETECT_L
     P1SELC |= V_DETECT_R; // ADC input for V_DETECT_R

     P1SELC |= V_A4_SEEED; // ADC input for V_A4_SEEED
     P1SELC |= V_THUMB; // ADC input for V_THUMB
     P1SEL0 |= UCA0TXD; // UCA0TXD pin
     P1SEL1 &= ~UCA0TXD; // UCA0TXD pin
     P1SEL0 |= UCA0RXD; // UCA0RXD pin
     P1SEL1 &= ~UCA0RXD; // UCA0RXD pin

     P1SELC |= V_THUMB;

//     P1SEL0 &= ~UCA0RXD; // Back Channel UCA0RXD
//     P1SEL1 |= UCA0RXD; // Back Channel UCA0RXD
//     P1SEL0 &= ~UCA0TXD; // Back Channel UCA0TXD
//     P1SEL1 |= UCA0TXD; // Back Channel UCA0TXD

     P1SEL0 |= ~UCA0RXD; // Back Channel UCA0RXD
     P1SEL1 &= ~UCA0RXD; // Back Channel UCA0RXD
     P1SEL0 |= UCA0TXD; // Back Channel UCA0TXD
     P1SEL1 &= ~UCA0TXD; // Back Channel UCA0TXD

     ///Initial code from project 5:

//     P1SEL0 &= ~V_A1_SEEED; // V_A1_SEEED GPIO operation
//     P1SEL1 &= ~V_A1_SEEED; // V_A1_SEEED GPIO operation
//     P1DIR &= ~V_A1_SEEED; // Direction = input
//
//     P1SEL0 &= ~V_DETECT_L; // V_DETECT_L GPIO operation
//     P1SEL1 &= ~V_DETECT_L; // V_DETECT_L GPIO operation
//     P1DIR &= ~V_DETECT_L; // Direction = input
//
//     P1SEL0 &= ~V_DETECT_R; // V_DETECT_R GPIO operation
//     P1SEL1 &= ~V_DETECT_R; // V_DETECT_R GPIO operation
//     P1DIR &= ~V_DETECT_R; // Direction = input

//     P1SEL0 &= ~V_A4_SEEED; // V_A4_SEEED GPIO operation
//     P1SEL1 &= ~V_A4_SEEED; // V_A4_SEEED GPIO operation
//     P1DIR &= ~V_A4_SEEED; // Direction = input
//
//     P1SEL0 &= ~V_THUMB; // V_THUMB GPIO operation
//     P1SEL1 &= ~V_THUMB; // V_THUMB GPIO operation
//     P1DIR &= ~V_THUMB; // Direction = input
//
//     P1SEL0 |= UCA0RXD; // USCI_A0 RXD UART operation
//     P1SEL1 &= ~UCA0RXD; // USCI_A0 RXD UART operation
//
//     P1SEL0 |= UCA0TXD; // USCI_A0 TXD UART operation
//     P1SEL1 &= ~UCA0TXD; // USCI_A0 TXD UART operation
//
//     P3DIR |= TEST_PROBE;    // Set PROBE pin (P3.2) as output
//     P5DIR |= LCD_BACKLITE;  // Set LCD backlight pin (P5.3) as output
}

//------------------------------------------------------------------------------
// Name: Init_Port2()
// Description: Configures pins for port 2
//------------------------------------------------------------------------------
void Init_Port2(){
     P2OUT = 0x00; // P2 set Low
     P2DIR = 0x00; // Set P2 direction to output

     P2SEL0 &= ~RESET_LCD; // SLOW_CLK GPIO operation
     P2SEL1 &= ~RESET_LCD; // SLOW_CLK GPIO operation
     P2OUT &= ~RESET_LCD; // Initial Value = Low / Off
     P2DIR |= RESET_LCD; // Direction = output

     P2SEL0 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
     P2SEL1 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
     P2OUT &= ~CHECK_BAT; // Initial Value = Low / Off
     P2DIR |= CHECK_BAT; // Direction = output

     P2SEL0 &= ~IR_LED; // P2_2 GPIO operation
     P2SEL1 &= ~IR_LED; // P2_2 GPIO operation
     P2OUT &= ~IR_LED; // Initial Value = Low / Off
     P2DIR |= IR_LED; // Direction = output

     P2SEL0 &= ~SW2; // SW2 Operation
     P2SEL1 &= ~SW2; // SW2 Operation
     P2DIR &= ~SW2; // Direction = input
     P2PUD |= SW2; // Configure pull-up resistor
     P2REN |= SW2; // Enable pullup resistor
     P2IES |= SW2; // SW1 Hi/Lo edge interrupt
     P2IFG &= ~SW2; // IFG SW1 cleared
     P2IE |= SW2; // SW1 interrupt Enabled

     P2SEL0 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
     P2SEL1 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
     P2OUT &= ~IOT_RUN_RED; // Initial Value = Low / Off
     P2DIR |= IOT_RUN_RED; // Direction = output
     P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
     P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
     P2OUT |= DAC_ENB; // Initial Value = High
     P2DIR |= DAC_ENB; // Direction = output
     P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
     P2SEL1 |= LFXOUT; // LFXOUT Clock operation
     P2SEL0 &= ~LFXIN; // LFXIN Clock operation
     P2SEL1 |= LFXIN; // LFXIN Clock operation

//     P2SEL0 &= ~IOT_RUN_CPU;
//     P2SEL1 &= ~IOT_RUN_CPU;
//     P2OUT &= ~IOT_RUN_CPU;
//     P2DIR |= IOT_RUN_CPU;

//     P2SEL0 &= ~DAC_ENB;
//     P2SEL1 &= ~DAC_ENB;
//     P2OUT |= DAC_ENB;
//     P2DIR |= DAC_ENB;
}

//------------------------------------------------------------------------------
// Name: Init_Port3()
// Description: Configures pins for port 3
//------------------------------------------------------------------------------
void Init_Port3(){
    P3OUT = 0x00; // P3 set Low
    P3DIR = 0x00; // Set all P3 direction to input initially

    P3SEL0 &= ~TEST_PROBE; // TEST_PROBE GPIO operation
    P3SEL1 &= ~TEST_PROBE; // TEST_PROBE GPIO operation
    P3OUT &= ~TEST_PROBE; // Initial Value = Low / Off
    P3DIR &= ~TEST_PROBE; // Direction = output

    P3SEL0 |= OA2O; // OA2O GPIO operation
    P3SEL1 |= OA2O; // OA2O GPIO operation
    P3OUT &= ~OA2O; // Direction = input
    P3DIR &= ~OA2O; // Direction = input

    P3SEL0 |= OA2N; // OA2- Operation
    P3SEL1 |= OA2N; // V_DETECT_L Operation
    P3OUT &= ~OA2N; // Initial Value = Low / Off
    P3DIR &= ~OA2N; // Direction = output

    P3SEL0 |= OA2P; // OA2+ Operation
    P3SEL1 |= OA2P; // OA2+ Operation
    P3OUT &= ~OA2P; // Initial Value = Low / Off
    P3DIR &= ~OA2P; // Direction = output

    P3SEL0 |=  SMCLK_OUT;    // Set second bit to Low / '1' (SMCLK)
    P3SEL1 &= ~SMCLK_OUT;    // Set first bit to Low / '0' (SMCLK)
    P3OUT  &= ~SMCLK_OUT;    // Initial Value = Low / Off
    P3DIR  |=  SMCLK_OUT;    // Set Direction = output

    P3SEL0 &= ~DAC_CNTL; // DAC_CNTL GPIO operation
    P3SEL1 &= ~DAC_CNTL; // DAC_CNTL GPIO operation
    P3OUT &= ~DAC_CNTL; // Initial Value = Low / Off
    P3DIR |= DAC_CNTL; // Direction = output

    P3SEL0 &= ~IOT_LINK_GRN; // IOT_LINK_GRN GPIO operation
    P3SEL1 &= ~IOT_LINK_GRN; // IOT_LINK_GRN GPIO operation
    P3OUT &= ~IOT_LINK_GRN; // Initial Value = Low / Off
    P3DIR |= IOT_LINK_GRN; // Direction = output

    P3SEL0 &= ~IOT_EN; // IOT_EN GPIO operation
    P3SEL1 &= ~IOT_EN; // IOT_EN GPIO operation
    P3OUT &= ~IOT_EN; // Initial Value = Low / Off
    P3DIR |= IOT_EN; // Direction = output
}

//------------------------------------------------------------------------------
// Name: Init_Port4()
// Description: Configures pins for port 4
//------------------------------------------------------------------------------
void Init_Port4(){
    P4OUT = 0x00; // P4 set Low
    P4DIR = 0x00; // Set P4 direction to output

    P4SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P4SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P4OUT &= ~RESET_LCD; // Initial Value = Low / Off
    P4DIR |= RESET_LCD; // Direction = output

    P4SEL0 &= ~SW1; // SW1 GPIO operation
    P4SEL1 &= ~SW1; // SW1 GPIO operation
    P4PUD |= SW1; // Configure pullup resistor
    P4DIR &= ~SW1; // Direction = input
    P4REN |= SW1; // Enable pullup resistor
    P4IES |= SW1; // SW1 Hi/Lo edge interrupt
    P4IFG &= ~SW1; // IFG SW1 Cleared
    P4IE |= SW1; // SW1 Interrupt Enabled

    P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

    P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation

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

//------------------------------------------------------------------------------
// Name: Init_Port5()
// Description: Configures pins for port 5
//------------------------------------------------------------------------------
void Init_Port5(){
    P5OUT = 0x00; // P5 set Low
    P5DIR = 0x00; // Set all P5 direction to input initially

    P5SELC |= V_BAT; // ADC input for V_BAT
    P5SELC |= V_5_0; // ADC input for V_BAT
    P5SELC |= V_DAC; // ADC input for V_DAC
    P5SELC |= V_3_3; // ADC input for V_3_3
    P5SEL0 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
    P5SEL1 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
    P5OUT |= IOT_BOOT; // Set Output value inactive
    P5DIR |= IOT_BOOT; // Set direction to output


    // Initial code from Project 5:

//    P5SEL0 &= ~V_BAT; // V_BAT GPIO operation
//    P5SEL1 &= ~V_BAT; // V_BAT GPIO operation
//    P5DIR &= ~V_BAT; // Direction = input
//
//    P5SEL0 &= ~V_5_0; // V_5_0 GPIO operation
//    P5SEL1 &= ~V_5_0; // V_5_0 GPIO operation
//    P5DIR &= ~V_5_0; // Direction = input
//
//    P5SEL0 &= ~V_DAC; // V_DAC GPIO operation
//    P5SEL1 &= ~V_DAC; // V_DAC GPIO operation
//    P5DIR &= ~V_DAC; // Direction = input
//
//    P5SEL0 &= ~V_3_3; // V_3_3 GPIO operation
//    P5SEL1 &= ~V_3_3; // V_3_3 GPIO operation
//    P5DIR &= ~V_3_3; // Direction = input

    //P5SEL0 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
//    P5SEL1 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
//    P5OUT &= ~IOT_BOOT; // Initial Value = Low / Off
//    P5DIR |= IOT_BOOT; // Direction = output
}

//------------------------------------------------------------------------------
// Name: Init_Port6()
// Description: Configures pins for port 6
//------------------------------------------------------------------------------
void Init_Port6(){
    P6SEL0 |= LCD_BACKLITE; // Clock/Timer Mode
    P6SEL1 &= ~LCD_BACKLITE; // SLOW_CLK GPIO operation
 //   P6OUT &= ~LCD_BACKLITE; // Initial Value = High / On
    P6DIR |= LCD_BACKLITE; // Direction = output

    P6SEL0 |= R_FORWARD; // RESET_LCD GPIO operation
    P6SEL1 &= ~R_FORWARD; // RESET_LCD GPIO operation
 //   P6OUT &= ~R_FORWARD; // Initial Value = Low / Off
    P6DIR |= R_FORWARD; // Direction = output

    P6SEL0 |= L_FORWARD; // RESET_LCD GPIO operation
    P6SEL1 &= ~L_FORWARD; // RESET_LCD GPIO operation
 //   P6OUT &= ~L_FORWARD; // Initial Value = Low / Off
    P6DIR |= L_FORWARD; // Direction = output

    P6SEL0 |= R_REVERSE; // RESET_LCD GPIO operation
    P6SEL1 &= ~R_REVERSE; // RESET_LCD GPIO operation
 //   P6OUT &= ~R_REVERSE; // Initial Value = Low / Off
    P6DIR |= R_REVERSE; // Direction = output

    P6SEL0 |= L_REVERSE; // RESET_LCD GPIO operation
    P6SEL1 &= ~L_REVERSE; // RESET_LCD GPIO operation
 //   P6OUT &= ~L_REVERSE; // Initial Value = Low / Off
    P6DIR |= L_REVERSE; // Direction = output

    P6SEL0 &= ~P6_5; // RESET_LCD GPIO operation
    P6SEL1 &= ~P6_5; // RESET_LCD GPIO operation
    P6OUT &= ~P6_5; // Initial Value = Low / Off
    P6DIR |= P6_5; // Direction = output

    P6SEL0 &= ~GRN_LED; // RESET_LCD GPIO operation
    P6SEL1 &= ~GRN_LED; // RESET_LCD GPIO operation
    P6OUT &= ~GRN_LED; // Initial Value = Low / Off
    P6DIR |= GRN_LED; // Direction = output
}





