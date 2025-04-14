/*
 * Interrupt_Timer.c
 *
 *  Created on: Nov 11, 2024
 *      Author: kaylaradu
 */


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


// GLOBALS

volatile unsigned int Time_Sequence;
volatile char one_time;
unsigned int counter_B0;
unsigned int delay_time;
extern unsigned int backlight_status;
extern volatile unsigned char backlight_changed;
extern int Switch_Counter1;
extern volatile unsigned char update_display;
extern volatile unsigned int backlight;


extern volatile unsigned int debounce_statesw1 = 0;
extern volatile unsigned int debounce_statesw2 = 0;
extern volatile unsigned int count_debounce_SW1;
extern volatile unsigned int count_debounce_SW2;
unsigned int DimFlag = TRUE;
unsigned int FlagSpinL;
unsigned int FlagSpinR;
unsigned int Blink_counter;

extern unsigned int SpincountL;
extern unsigned int SpincountR;

unsigned int Displaycount = 0;

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

unsigned int tmr_ms_count;
unsigned int blink_count;
unsigned int CCR0_counter;
unsigned int transmit_count;
char transmit_state;


//-----------------------------------------------------------------------------
// 200ms TRIGGER WHAT HAPPENS
//---------------------------------------------------------------------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){

    if (Displaycount++ == 20){
                Displaycount = 0;
                update_display = TRUE;
                tmr_ms_count++;
                CCR0_counter +=1;
                if (transmit_state == TRANSMIT){
                    transmit_count++;
                }
            }

        if(blink_count++ > 20){
            blink_count = 0; // Reset for next count
            P6OUT ^= LCD_BACKLITE; // Flip State of LCD_BACKLITE
        }
        ADCCTL0 |= ADCSC; // Start next sample
        // Time Sequence
        one_time = 1;
        if(Time_Sequence++ > 250){
            Time_Sequence = 0;
        }
        TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0

//    if(DimFlag == TRUE){
//     if (Blink_counter++ >= 20){
//         Blink_counter = 0;
//         LCD_BACKLITE_DIMING = PERCENT_80; //Flips on
//
//     }
//    }
//
//
//    if(FlagSpinL == TRUE){
//        SpincountL++;
//    }
//    if(FlagSpinR == TRUE){
//            SpincountR++;
//        }

}

//-----------------------------------------------------------------------------
// DEBOUNCE TIMER
//---------------------------------------------------------------------------
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
    switch(__even_in_range(TB0IV,14)){
    case  0: break;                    // No interrupt
    case  2:                           // CCR1 Used for SW1 Debounce
        count_debounce_SW1++;
        if (count_debounce_SW1 >= DEBOUNCE_TIME){
            count_debounce_SW1 = 0;

            TB0CCTL1 &= ~CCIE;
            P4IFG &= ~SW1;
            P4IE  |= SW1;

            TB0CCTL0 |= CCIE;
        }
        break;

    case  4:                           // CCR2 Used for SW2 Debounce
        count_debounce_SW2++;
        if (count_debounce_SW2 >= DEBOUNCE_TIME){
            count_debounce_SW2 = 0;

            TB0CCTL2 &= ~CCIE;
            P2IFG &= ~SW2;
            P2IE  |=  SW2;

            TB0CCTL0 |= CCIE;
        }

        break;

    case 14:                           // overflow available for greater than 1 second timer
        break;
    default: break;
    }
}


#pragma vector = TIMER0_B3_VECTOR
__interrupt void Timer0_B3_ISR(void){
    TB0CCR2 += TB0CCR2_INTERVAL;
}


