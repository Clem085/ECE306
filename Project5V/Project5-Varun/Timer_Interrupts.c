/*
 * Timer_Interrupts.c
 *
 *  Created on: Feb 22, 2024
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
//------------------------------------------------------------------------------
//variable declarations
unsigned int display_count;
volatile unsigned char display_changed;
extern char display_line[4][11];
volatile unsigned char update_display;
volatile char one_time;
volatile unsigned int Time_Sequence;
unsigned int tmr_ms_count;
unsigned int blink_count;
unsigned int CCR0_counter;
unsigned int transmit_count;
unsigned int iot_on_time;
unsigned int start_iot_time;
char transmit_state;
unsigned int initialize_done;
unsigned int run_time;
unsigned int run_time_flag;
unsigned int step_counter;
unsigned int step_flag;
unsigned int detect_time;
unsigned int detect_flag;
extern unsigned int turn_flag;
extern unsigned int turn_count;
extern unsigned int disp_flag;
extern unsigned int disp_count;

extern unsigned int Lucas_counter;
extern unsigned int flag_lucas;
extern unsigned int Varun_counter;
extern unsigned int Flag_varun;
extern unsigned int dawg_counter;
extern unsigned int flag_schnawg;
extern unsigned int wait_flag;
extern unsigned int wait_count;

unsigned int line_flag;
char its_close;
//------------------------------------------------------------------------------

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //------------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //----------------------------------------------------------------------------
    //...... Add What you need happen in the interrupt ......
    // LCD Backlight

    if (display_count++ == 20){
            display_count = 0;
            update_display = TRUE;
            tmr_ms_count++;
            CCR0_counter +=1;
            iot_on_time++;
            if (transmit_state == TRANSMIT){
                transmit_count++;
            }
        }

    if (detect_flag == TRUE){
        detect_time++;
    }

    if (step_flag == TRUE){
        step_counter++;
    }

    if (wait_flag == TRUE){
        wait_count++;
    }

    if (turn_flag == TRUE){
        turn_count++;
    }

    if (disp_flag == TRUE){
        disp_count++;
    }

    ADCCTL0 |= ADCSC; // Start next sample

    if (run_time_flag){
        run_time++;
    }
    if(flag_lucas == TRUE){
     Lucas_counter++;
     }
     if(Flag_varun == TRUE){
     Varun_counter++;
     }
     if(flag_schnawg == TRUE){
     dawg_counter++;
     }

    if(blink_count++ > MS_COUNT){
        blink_count = 0; // Reset for next count
        P6OUT ^= LCD_BACKLITE; // Flip State of LCD_BACKLITE
    }
    one_time = TRUE;
    if(Time_Sequence++ > 250){
        Time_Sequence = 0;
    }

    start_iot_time++;
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
        P4IFG &= ~SW1;
        P4IE |= SW1;
        TB0CCTL1 &= ~CCIFG;
        TB0CCTL1 &= ~CCIE;
        TB0CCR1 += TB0CCR1_INTERVAL; // Add Offset to TBCCR1
        TB0CCTL0 |= CCIE;
        break;
    case 4: // CCR2 not used
        P2IFG &= ~SW2;
        P2IE |= SW2;
        TB0CCTL2 &= ~CCIFG;
        TB0CCTL2 &= ~CCIE;
        TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR1
        TB0CCTL0 |= CCIE;
        break;
    case 14: // overflow
        //...... Add What you need happen in the interrupt ......
        break;
    default: break;
}
//----------------------------------------------------------------------------
}
