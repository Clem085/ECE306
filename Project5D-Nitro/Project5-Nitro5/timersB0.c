// Includes
#include "Display.h"
#include "LCD.h"
#include "functions.h"
#include "macros.h"
#include "motors.h"
#include "msp430.h"
#include "ports.h"
#include "switch.h"
#include <string.h>

// Globals
volatile unsigned int Time_Sequence;
volatile char one_time;
unsigned int counter_B0;

void Init_Timers(void) { Init_Timer_B0(); }

// ---ORIGINAL-------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow
void Init_Timer_B0(void) {
  TB0CTL = TBSSEL__SMCLK;     // SMCLK source
  TB0CTL |= TBCLR;            // Resets TB0R, clock divider, count direction
  TB0CTL |= MC__CONTINOUS;    // Continuous up
  TB0CTL |= ID__2;            // Divide clock by 2
  TB0EX0 = TBIDEX__8;         // Divide clock by an additional 8
  TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
  TB0CCTL0 |= CCIE;           // CCR0 enable interrupt
  // TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
  // TB0CCTL1 |= CCIE; // CCR1 enable interrupt
  // TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
  // TB0CCTL2 |= CCIE; // CCR2 enable interrupt
  TB0CTL &= ~TBIE;  // Disable Overflow Interrupt
  TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag

  Time_Sequence = 0; // Added to ensure Time_Sequence starts at 0
}
//----------------------------------------------------------------------------

// Delays
void half_sec_delay(void) {
  counter_B0++;
  if (!(counter_B0 % HALF_SEC)) {
    TB0CCTL0 &= ~CCIE;
    counter_B0 = COUNTER_RESET;
    delay_time = COUNTER_RESET;
  }
}

// TIMER INTERRUPT - CLOCK __
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void) {
  //------------------------------------------------------------------------------
  // TimerB0 0 Interrupt handler for 10msec timer
  //----------------------------------------------------------------------------
  Time_Sequence++;
  // Add counter to count 200msec worth of interrupts then set update_display =
  // TRUE;
  TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
  //----------------------------------------------------------------------------
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void) {
  //----------------------------------------------------------------------------
  // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
  //----------------------------------------------------------------------------
  switch (__even_in_range(TB0IV, 14)) {
  case 0:
    break; // No interrupt
  case 2:  // CCR1 Used for SW1 Debounce
    // Disable Timer B0 CCR1
    P4IFG &= ~SW1; // Clear SW1 Interrupt Flag
    P4IE |= SW1;   // Enable SW1 Interrupt
    // Add Offset to TBCCR1

    half_sec_delay();
    break;
  case 4: // CCR2 Used for SW2 Debounce
    // 1. Disable Timer B0 CCR2
    P2IFG &= ~SW2; // Clear SW1 Interrupt Flag
    P2IE |= SW2;   // Enable SW1 Interrupt
    break;
  case 14: // overflow available for greater than 1 second timer
    break;
  default:
    break;
  }
}



#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
  // Switch 1
  if (P4IFG & SW1) {
    P4IFG &= ~SW1; // IFG SW1 cleared
    // Set a variable to identify the switch has been pressed.
    // Set a variable to identify the switch is being debounced.
    // Reset the count required of the debounce.
    // Disable the Switch Interrupt.
    // Clear any current timer interrupt.
    P6OUT &= ~LCD_BACKLITE;
  }
}


