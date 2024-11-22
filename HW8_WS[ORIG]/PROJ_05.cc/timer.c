#include "msp430.h"
#include "macros.h"
#include "ports.h"
#include <msp430fr2355.h>


// Global variables to manage debounce and switch state
volatile unsigned int debounce_count = 0;
volatile unsigned char switch_pressed = 0;
volatile unsigned char debounce_in_progress = 0;

#define TB0CCR0_INTERVAL 5000
//#define TBSSEL__SMCLK TASSEL_2  // SMCLK source is selected
//#define TAIDEX__8 (3 << 6);

// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow
//void Init_Timer_B0(void) {
//  TB0CTL = TBSSEL__SMCLK; // SMCLK source
//  TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
//  TB0CTL |= MC__CONTINOUS; // Continuous up
//  TB0CTL |= ID__2; // Divide clock by 2
////  TA0EX0 = TAIDEX__8; // Divide clock by an additional 8
//  TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
//  TB0CCTL0 |= CCIE; // CCR0 enable interrupt
//
//  TB0CTL &= ~TBIE; // Disable Overflow Interrupt
//  TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
//}



#pragma vector = PORT4_VECTOR;
__interrupt void switchP4_interrupt(void){
  // Switch 1
  if (PBIFG & SW1) {
    PBIFG &= ~SW1; // IFG SW1 cleared

    PBIFG &= ~SW1;

    TB0CCTL1 &= ~CCIFG;

//    TB0R += TBOCCR1_INTERVAL;

    TB0CCTL1 |= CCIE; // Enable interrupt for CCR1

    // Check if debounce is in progress
    if (!debounce_in_progress) {
      switch_pressed = 1;        // Indicate switch is pressed
      debounce_in_progress = 1;  // Set debounce in progress
      debounce_count = 0;        // Reset debounce timer counter

//      // Clear timer flags and start the timer for debounce
//      TB0CTL |= TBCLR;  // Clear timer
//      TB0CCTL0 |= CCIE;  // Enable Timer A interrupt
      TB0CCR0 = TB0CCR0_INTERVAL;  // Set timer period for debounce (~5ms)
      TB0CTL = TBSSEL_2 + MC_1;  // SMCLK, Up mode
    }

    P6OUT &= ~LCD_BACKLITE;
  }
}

// Timer A0 interrupt service routine for debounce control
//#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B0_ISR(void) {
  debounce_count++;  // Increment debounce count

  if (debounce_count >= 5) {  // Wait for 5 timer cycles (~25ms)
    debounce_in_progress = 0;  // End debounce process
    PBIE |= SW1;  // Re-enable switch interrupt
    TB0CCTL0 &= ~CCIE;  // Disable Timer A interrupt
  }
}


#pragma vector=PORT2_VECTOR;
__interrupt void switchP2_interrupt(void){
  // Switch 2
  if (P2IFG & SW2) {
    P2IE &= ~SW2; // Disable interrupt on SW2
    P2IFG &= ~SW2; // IFG SW2 cleared
    TB0CCTL2 &= ~CCIFG; // Clear interrupt flag for CCR2

    // 4. Add Interval to TB0R for TB0CCR2
//    TB0R += TB0CCR2_INTERVAL; // Add debounce interval to timer count for CCR2

    // 5. Enable TimerB0_2
    TB0CCTL2 |= CCIE; // Enable interrupt for CCR2
    P6OUT |= LCD_BACKLITE;
  }
}



