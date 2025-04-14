/*
 * timersB0.c
 *
 *  Created on: Oct 11, 2024
 *      Author: kaylaradu
 */


// Includes
#include "LCD.h"
#include "functions.h"
#include "macros.h"
#include "wheels.h"
#include "msp430.h"
#include "ports.h"
#include "switches.h"
#include <string.h>
#include "timersB0.h"
#include <stdio.h>


// Globals 042
unsigned char dec = ' ';
unsigned char one = ' ';
unsigned char ten = ' ';
unsigned char hun = ' ';




void Init_Timers(void) {
    Init_Timer_B0();
    Init_Timer_B1();
    Init_Timer_B3();
}




// ---ORIGINAL-------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow
void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK;     // SMCLK source
    TB0CTL = TBSSEL__SMCLK;     // SMCLK source
       TB0CTL |= TBCLR;            // Resets TB0R, clock divider, count direction
       TB0CTL |= MC__CONTINOUS;    // Continuous up
       TB0CTL |= ID__8;            // Divide clock by 2
       TB0EX0 = TBIDEX__8;         // Divide clock by an additional 8
       TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
       TB0CCTL0 |= CCIE;           // CCR0 enable interrupt
       // TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
       // TB0CCTL1 |= CCIE; // CCR1 enable interrupt
       // TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
       // TB0CCTL2 |= CCIE; // CCR2 enable interrupt
       TB0CTL &= ~TBIE;  // Disable Overflow Interrupt
       TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag


}



void Init_Timer_B1(void) {
    TB0CTL = TBSSEL__SMCLK;     // SMCLK source
    TB0CTL = TBSSEL__SMCLK;     // SMCLK source
       TB0CTL |= TBCLR;            // Resets TB0R, clock divider, count direction
       TB0CTL |= MC__CONTINOUS;    // Continuous up
       TB0CTL |= ID__8;            // Divide clock by 2
       TB0EX0 = TBIDEX__8;         // Divide clock by an additional 8
       TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
       TB0CCTL0 |= CCIE;           // CCR0 enable interrupt
       // TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
       // TB0CCTL1 |= CCIE; // CCR1 enable interrupt
       // TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
       // TB0CCTL2 |= CCIE; // CCR2 enable interrupt
       TB0CTL &= ~TBIE;  // Disable Overflow Interrupt
       TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag


}



void Init_Timer_B3(void) {
    //-----------------------------------------------------------------------------
    // SMCLK source, up count mode, PWM Right Side
    // TB3.1 P6.0 LCD_BACKLITE
    // TB3.2 P6.1 R_FORWARD
    // TB3.3 P6.2 R_REVERSE
    // TB3.4 P6.3 L_FORWARD
    // TB3.5 P6.4 L_REVERSE
    //-----------------------------------------------------------------------------
    TB3CTL = TBSSEL__SMCLK; // SMCLK
    TB3CTL |= MC__UP;       // Up Mode
    TB3CTL |= TBCLR;        // Clear TAR

    PWM_PERIOD = WHEEL_PERIOD;         // PWM Period [Set this to 50005]

    TB3CCTL1 = OUTMOD_7;               // CCR1 reset/set
    LCD_BACKLITE_DIMING = PERCENT_80;  // P6.0 Right Forward PWM duty cycle

    TB3CCTL2 = OUTMOD_7;               // CCR2 reset/set
    RIGHT_FORWARD_SPEED = WHEEL_OFF;   // P6.1 Right Forward PWM duty cycle

    TB3CCTL3 = OUTMOD_7;               // CCR3 reset/se
    LEFT_FORWARD_SPEED = WHEEL_OFF;    // P6.2 Right Forward PWM duty cycl

    TB3CCTL4 = OUTMOD_7;               // CCR4 reset/set
    RIGHT_REVERSE_SPEED = WHEEL_OFF;   // P6.3 Left Forward PWM duty cycle

    TB3CCTL5 = OUTMOD_7;               // CCR5 reset/set
    LEFT_REVERSE_SPEED = WHEEL_OFF;    // P6.4 Right Reverse PWM duty cycle
    //-----------------------------------------------------------------------------
}



//void timerdisplay(void){
//    int x = Displaycount;
//    if(x%1000){
//        dec = (x / 1000) + '0';
//        x = x/1000;
//        one = (x / 100)+ '0';
//        x = x/100;
//        ten = (x / 10)+ '0';
//        x = x/10;
//        hun = x + '0';
//    }else if(x %100){
//        dec = (x / 1000) + '0';
//        one = (x / 100)+ '0';
//        x = x/100;
//        ten = (x / 10)+ '0';
//        x = x/10;
//        hun = x + '0';
//    }else if(x %10){
//        dec = (x / 1000) + '0';
//        one = (x / 100)+ '0';
//        ten = (x / 10)+ '0';
//        x = x/10;
//        hun = x + '0';
//    }else {
//        dec = (x / 1000) + '0';
//        one = (x / 100)+ '0';
//        ten = (x / 10)+ '0';
//        hun = x + '0';
//    }
//
//}


