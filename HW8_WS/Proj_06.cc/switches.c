#include "msp430.h"
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

// Global variables for debounce
// SW1 debounce counter
volatile unsigned int debounce_count1 = 0;
// SW2 debounce counter
volatile unsigned int debounce_count2 = 0;

// Backlight toggle state
volatile unsigned char backlight_on = 1;

// SW1 debounce active flag
volatile unsigned char debounce_active = 0;

// Increment for the debounce interval for the switches
#define increment 3277;


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

//------------------------------------------------------------------------------
//  Description: SW1 interrupt handler (Port 4)
//------------------------------------------------------------------------------
#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
    if ((P4IFG & SW1) && (debounce_active == 0)) {  // Check if SW1 caused the interrupt
        Display_Change(1);  // Display change for SW1

        P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
        P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]

//        debounce_count1 = 0;  // Reset SW1 debounce counter
//        debounce_active = 1;  // Start SW1 debounce
//
//        P6OUT &= ~LCD_BACKLITE;  // Turn off backlight
//        TB0CCTL0 &= ~CCIE;  // Disable backlight blinking timer
//
//        disable_switch_interrupts();
//
//        TB0CCR1 = TB0R + increment;  // ~100ms debounce interval for SW1
//        TB0CCTL1 |= CCIE;  // Enable CCR1 interrupt for SW1 debounce
//
//        P4IFG &= ~SW1;  // Clear SW1 interrupt flag
    }
}

//------------------------------------------------------------------------------
//  Description: SW2 interrupt handler (Port 2)
//------------------------------------------------------------------------------
//#pragma vector = PORT2_VECTOR
//__interrupt void switchP2_interrupt(void) {
//    if ((P2IFG & SW2) && (debounce_active == 0)) {  // Check if SW2 caused the interrupt
//        Display_Change(2);  // Display change for SW2
//
//        debounce_count2 = 0;  // Reset SW2 debounce counter
//        debounce_active = 1;  // Start SW2 debounce
//
//        P6OUT &= ~LCD_BACKLITE;  // Turn off backlight
//        TB0CCTL0 &= ~CCIE;  // Disable backlight blinking timer
//
//        disable_switch_interrupts();
//
//        TB0CCR2 = TB0R + increment;  // ~100ms debounce interval for SW2
//        TB0CCTL2 |= CCIE;  // Enable CCR2 interrupt for SW2 debounce
//
//        P2IFG &= ~SW2;  // Clear SW2 interrupt flag
//    }
//}

//------------------------------------------------------------------------------
//  Description: Timer B0 CCR0 ISR - Backlight Blinking
//------------------------------------------------------------------------------
//#pragma vector = TIMER0_B0_VECTOR
//__interrupt void Timer0_B0_ISR(void) {
//    if (backlight_on) {
//        P6OUT &= ~LCD_BACKLITE;  // Turn off backlight
//    } else {
//        P6OUT |= LCD_BACKLITE;  // Turn on backlight
//    }
//
//    backlight_on = !backlight_on;  // Toggle backlight state
//    TB0CCTL0 &= ~CCIFG;  // Clear interrupt flag
//}

//------------------------------------------------------------------------------
//  Description: Timer B0 CCR1 and CCR2 ISR - Debounce Handling
//------------------------------------------------------------------------------
//#pragma vector = TIMER0_B1_VECTOR
//__interrupt void Timer0_B1_ISR(void) {
//    switch (TB0IV) {
//        case TB0IV_TBCCR1:  // SW1 debounce completion
//            debounce_count1++;
//            if (debounce_count1 >= 12) {  // ~1 second debounce time
//                debounce_active = 0;  // End SW1 debounce
//                enable_switch_interrupts();
//                TB0CCTL1 &= ~CCIE;  // Disable CCR1 interrupt
//
//                // Re-enable backlight blinking timer
//                TB0CCTL0 |= CCIE;
//            }
//            break;
//
//        case TB0IV_TBCCR2:  // SW2 debounce completion
//            debounce_count2++;
//            if (debounce_count2 >= 12) {  // ~1 second debounce time
//                debounce_active = 0;  // End SW2 debounce
//                enable_switch_interrupts();
//                TB0CCTL2 &= ~CCIE;  // Disable CCR2 interrupt
//
//                // Re-enable backlight blinking timer
//                TB0CCTL0 |= CCIE;
//            }
//            break;
//
//        default:
//            break;
//    }
//}

