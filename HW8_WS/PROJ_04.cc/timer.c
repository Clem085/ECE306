
#include "msp430.h"
#include "macros.h"
#include "ports.h"
#include "functions.h"
#include <msp430fr2355.h>
#include "ADC.h"
#include "DAC.h"

extern volatile unsigned int Time_Sequence;
volatile unsigned int LCD_Update;

volatile unsigned int pause_counter;
volatile unsigned int pause_counter1;

//------------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow
void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK; // SMCLK source
    TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS; // Continuous up
    TB0CTL |= ID__2; // Divide clock by 2
    TB0EX0 = TBIDEX__8; // Divide clock by an additional 8
    TB0CCR0 = TB0CCR0_INTERVAL; // CCR0  // Period for interrupt ( hz) how long it needs to wait between interrupts
    TB0CCTL0 |= CCIE; // CCR0 enable interrupt

    TB0CTL &= ~TBIE; // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
// you had this on
//     TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
//     TB0CCTL1 |= CCIE; // CCR1 enable interrupt
//     TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
//     TB0CCTL2 |= CCIE; // CCR2 enable interrupt
}
//------------------------------------------------------------------------------
//#pragma vector = TIMER0_B0_VECTOR
//__interrupt void Timer0_B0_ISR(void){
////------------------------------------------------------------------------------
//// TimerB0 0 Interrupt handler
////----------------------------------------------------------------------------
////...... Add What you need happen in the interrupt ......
//    if(Time_Sequence % 200 == 0){
//        update_display = 1;
//    }
//    Time_Sequence++;
//
//    display_changed = TRUE;
//    LCD_Update = TRUE;
//
////----------------------------------------------------------------------------
//}


// Don't do anything different for Init_Timer_B3(void)
void Init_Timer_B3(void) {
//------------------------------------------------------------------------------
    TB3CTL = TBSSEL__SMCLK; // SMCLK
    TB3CTL |= MC__UP; // Up Mode
    TB3CTL |= TBCLR; // Clear TAR
    PWM_PERIOD = WHEEL_PERIOD; // PWM Period [Set this to 50005]
    TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
    LCD_BACKLITE_DIMING = PERCENT_80;
    TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
    TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
    TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    TB3CCTL5 = OUTMOD_7; // CCR5 reset/set
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
//------------------------------------------------------------------------------
}

//#pragma vector=TIMER0_B1_VECTOR
//interrupt void TIMER0_B1_ISR(void){
////----------------------------------------------------------------------------
//// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
////----------------------------------------------------------------------------
//switch(__even_in_range(TB0IV,14)){
//    case 0: break; // No interrupt
//    case 2: // CCR1 not used
//
//    TB0CCTL1 &= ~TBIE;
//    P4IFG &= ~SW1;
//    P4IE |= SW1;
//
//    break;
//    case 4: // CCR2 not used
//        TB0CCTL2 &= ~TBIE;
//        P2IFG &= ~SW2;
//        P2IE |= SW2;
//
//    TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR2
//    break;
//    case 14: // overflow
//        // The following line should be done in a timer overflow interrupt [after 2 or 3 overflows]
//          P2OUT   |=  DAC_ENB;                  // Value = High [enabled]
//        // Each time through the overflow time after enable, subtract 50 from DAC_data
//          DAC_data -= 50;
//          SAC3DAT  = DAC_data;                  // Stepping DAC Output
//        // Somewhere around 1200 will be about 6v. You will need to measure it.
//
//          if (DAC_data < 1200){
//              TB0CTL &= ~TBIE; // Disable Overflow Interrupt
//              TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
//          }
//    break;
//    default: break;
//    }
//}




