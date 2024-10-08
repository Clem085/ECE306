/*
 * Timers.c
 *
 *  Created on: Feb 19, 2024
 *      Author: varun
 */

//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "strings.h"
//------------------------------------------------------------------------------
//variable declarations
unsigned int display_count;
volatile unsigned char display_changed;
extern char display_line[4][11];
volatile unsigned char update_display;
unsigned int tmr_ms_count;
//------------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow
void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK; // SMCLK source
    TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS; // Continuous up
    TB0CTL |= ID__4; // Divide clock by 4
    TB0EX0 = TBIDEX__8; // Divide clock by an additional 8
    TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
    TB0CCTL0 |= CCIE; // CCR0 enable interrupt
//    TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
//    TB0CCTL1 |= CCIE; // CCR1 enable interrupt
// TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
// TB0CCTL2 |= CCIE; // CCR2 enable interrupt
    TB0CTL &= ~TBIE; // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}
//------------------------------------------------------------------------------

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //------------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //----------------------------------------------------------------------------
    //...... Add What you need happen in the interrupt ......
    if (display_count++ == 20){
        display_count = 0;
        update_display = TRUE;
        tmr_ms_count++;
    }
    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0

    //----------------------------------------------------------------------------
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
    case 0: break; // No interrupt
    case 2: // CCR1 not used
//...... Add What you need happen in the interrupt ......
//        TB0CCR1 += TB0CCR1_INTERVAL; // Add Offset to TBCCR1
        break;
    case 4: // CCR2 not used
        //...... Add What you need happen in the interrupt ......
//        TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR2
        break;
    case 14: // overflow
        //...... Add What you need happen in the interrupt ......
        break;
    default: break;
}
//----------------------------------------------------------------------------
}
