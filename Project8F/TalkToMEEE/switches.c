/* Switches Program Information Header
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : switches.c
  Description:  This file contains the code to control the 2 buttons (SW1 & SW2) on the MSP430 Board
  Programmer: Connor Savugot
//  Date Created: Sep 20, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */

// #include as of 11-10-24
//// Header Files
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "motors.h"
#include  "Display.h"
#include  "timers.h"
#include  "interrupts.h"
#include  "switches.h"
#include  "ADC.h"
#include  "IR.h"
#include  "serial.h"
#include  "DAC.h"
#include  "menu.h"
//// Libraries
#include  <string.h>
#include  <stdio.h>

// Global Variables declared and referenced in Header file

// Global variables for debounce


// Global variables for debounce


// Backlight toggle state
volatile unsigned char backlight_on = 1;

// SW1 debounce active flag
volatile unsigned char debounce_active = 0;

// Global flags
//extern volatile int switch1_pressed;  // Flag to track switch 1 press

// Increment for the debounce interval for the switches
#define increment 3277;




// SW1 debounce counter
volatile unsigned int debounce_count1;
volatile unsigned int debounce_count2;

// Backlight toggle state
//volatile unsigned char backlight_on = 1;

extern volatile unsigned int localCounter;

// SW1 debounce active flag
volatile char switch_debounce_active = 0; // Bitmask for debounce status

// Increment for the debounce interval for the switches
#define increment 3277;

volatile int switch1_pressed;  // Declare the variable as external
volatile int switch2_pressed;  // Declare the variable as external

//------------------------------------------------------------------------------
//  Description: Disables siwtch interrupts
//------------------------------------------------------------------------------
void disable_switch_interrupts(void){
    P4IE &= ~SW1;
    P2IE &= ~SW2;
}

//------------------------------------------------------------------------------
//  Description: Enables siwtch interrupts
//------------------------------------------------------------------------------
void enable_switch_interrupts(void){
    P4IE |= SW1;
    P2IE |= SW2;
}

void Forward_On(void){ //Sets the two port 6 outputs to high for forward direction
    P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
    P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]
}

//------------------------------------------------------------------------------
//  Description: SW1 interrupt handler (Port 4)
//------------------------------------------------------------------------------
#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
    if ((P4IFG & SW1) && (switch_debounce_active == 0)) {  // Check if SW1 caused the interrupt
        switch1_pressed = 1;
        //localCounter = 0;
        localCounter++;

        debounce_count1 = 0;  // Reset SW1 debounce counter
        switch_debounce_active = 1;  // Start SW1 debounce

        TB0CCR1 = TB0R + increment;  // ~100ms debounce interval for SW1
        TB0CCTL1 |= CCIE;  // Enable CCR1 interrupt for SW1 debounce

        P4IFG &= ~SW1;  // Clear SW1 interrupt flag

       disable_switch_interrupts();
    }
}

//------------------------------------------------------------------------------
//  Description: SW2 interrupt handler (Port 2)
//------------------------------------------------------------------------------
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void) {
    if ((P2IFG & SW2) && (switch_debounce_active == 0)) {  // Check if SW2 caused the interrupt
        switch2_pressed = 1;
        localCounter++;

        debounce_count2 = 0;  // Reset SW2 debounce counter
        switch_debounce_active = 1;  // Start SW2 debounce

        TB0CCR2 = TB0R + increment;  // ~100ms debounce interval for SW2
        TB0CCTL2 |= CCIE;  // Enable CCR2 interrupt for SW2 debounce

        P2IFG &= ~SW2;  // Clear SW2 interrupt flag

        //disable_switch_interrupts();
    }
}

//------------------------------------------------------------------------------
//  Description: Timer B0 CCR0 ISR - Backlight Blinking
//------------------------------------------------------------------------------


//#pragma vector=TIMER0_B1_VECTOR
//__interrupt void TIMER0_B1_ISR(void){
////----------------------------------------------------------------------------
//// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
////----------------------------------------------------------------------------
//
//    switch (__even_in_range(TB0IV, 14)) {
//            case 2:  // SW1 debounce completion
//                debounce_count1++;
//                if (debounce_count1 >= 10) {  // Adjust as needed
//                    switch_debounce_active = 0;  // End debounce
//                    //enable_switch_interrupts();
//                    TB0CCTL1 &= ~CCIE;  // Disable debounce interrupt
//                }
//                break;
//
//            case 4:  // SW2 debounce completion
//                debounce_count2++;
//                if (debounce_count2 >= 10) {
//                    switch_debounce_active = 0;
//                    enable_switch_interrupts();
//                    TB0CCTL2 &= ~CCIE;
//                }
//                break;
//
//            default: break;
//        }
//    switch(__even_in_range(TB0IV,14)){
//        case 0: break; // No interrupt
//        case 2: // CCR1 not used
//        //...... Add What you need happen in the interrupt ......
//        TB0CCR1 += TB0CCR1; // Add Offset to TBCCR1
//        break;
//    case 4: // CCR2 not used
//        //...... Add What you need happen in the interrupt ......
//        TB0CCR2 += TB0CCR2; // Add Offset to TBCCR2
//        break;
//        case 14: // overflow
//        //...... Add What you need happen in the interrupt ......
//        break;
//    default: break;
//    }
//}
//---------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Description: Timer B0 CCR1 and CCR2 ISR - Debounce Handling
//------------------------------------------------------------------------------
#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR(void) {
    switch (TB0IV) {
        case TB0IV_TBCCR1:  // SW1 debounce completion
            debounce_count1++;
            if (debounce_count1 >= 12) {  // ~1 second debounce time
                switch1_pressed = 0;
                switch_debounce_active = 0;  // End SW1 debounce
                //enable_switch_interrupts();
                TB0CCTL1 &= ~CCIE;  // Disable CCR1 interrupt

                // Re-enable backlight blinking timer
                TB0CCTL0 |= CCIE;
            }
            break;

        case TB0IV_TBCCR2:  // SW2 debounce completion
            debounce_count2++;
            if (debounce_count2 >= 12) {  // ~1 second debounce time
                switch2_pressed = 0;
                switch_debounce_active = 0;  // End SW2 debounce
                //enable_switch_interrupts();
                TB0CCTL2 &= ~CCIE;  // Disable CCR2 interrupt

                // Re-enable backlight blinking timer
                TB0CCTL0 |= CCIE;
            }
            break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////








