/*
 * Timer_Interrupts.c
 *
 *  Created on: Nov 21, 2024
 *  Author: Kayla Radu
 *
 */
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "strings.h"
#include "wheels.h"
#include "Timers.h"
#include  "DAC.h"
#include "switches.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//variable declarations
unsigned int display_count;
volatile unsigned char display_changed;
extern char display_line[4][11];
volatile unsigned char update_display;
volatile char one_time;
volatile unsigned int Time_Sequence;
unsigned int CCR0_counter;
unsigned int iot_on_time;
char transmit_state;
unsigned int initialize_done;
unsigned int run_time;
unsigned int run_time_flag;
extern volatile unsigned int debounce_statesw1 = 0;
extern volatile unsigned int debounce_statesw2 = 0;
extern volatile unsigned int count_debounce_SW1;
extern volatile unsigned int count_debounce_SW2;
unsigned int DimFlag = TRUE;
unsigned int FlagSpinL = FALSE;
unsigned int FlagSpinR = FALSE;
unsigned int FlagWait = FALSE;
unsigned int Blink_counter;

extern unsigned int SpincountL;
extern unsigned int SpincountR;
extern unsigned int Waitcount;
extern unsigned int ping;
unsigned int pingcount = 0;
extern unsigned int lostCounter;
extern unsigned int lostflg;
extern unsigned int arch_counter;
extern unsigned int motorDrain;

//------------------------------------------------------------------------------

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //------------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //----------------------------------------------------------------------------
    //...... Add What you need happen in the interrupt ......
    // LCD Backlight

    if(display_count++ == 4){
        display_count = 0;
        update_display = TRUE;

        CCR0_counter +=1;
        iot_on_time++;
//        if (transmit_state == TRANSMIT){
//            transmit_count++;
//        }

    }

    if(pingcount++ == 100){
//        P1OUT ^= RED_LED;
        ping = !ping;
        pingcount = 0;
    }

    if(lostflg){
        lostCounter++;
        P1OUT ^= RED_LED;
    }else{
        lostCounter = 0;
    }

    // Time Sequence
//    one_time = 1; //CARLSON CODE
//    if(Time_Sequence++ > 250){
//        Time_Sequence = 0;
//    }



    if(DimFlag == TRUE){
        if (Blink_counter++ >= 4){
            Blink_counter = 0;
            LCD_BACKLITE_DIMING = PERCENT_80; //Flips on

        }
    }

    if (run_time_flag){
        run_time++;
    }

//    if(FlagSpinL == TRUE){
//        SpincountL++;
//    }


    if(FlagSpinR == TRUE){
        SpincountR++;
    }

    if(FlagWait == TRUE){
        Waitcount++;
    }


    if(arch_counter++ > 65534){
        arch_counter = 0;
    }


    if(motorDrain++ > 65534){
        motorDrain = 0;
    }

    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0

    //----------------------------------------------------------------------------
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
//        P1OUT ^= RED_LED;
    //----------------------------------------------------------------------------
    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
    //----------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
    case 0: break; // No interrupt
    case 2:
//        TB0CCR1 = TB0R + TB0CCR1_INTERVAL; // WARNING HELP QUESTION why not TB0CCR1 += TB0CCR1_INTERVAL;
        TB0CCR1 += TB0CCR1_INTERVAL; // WARNING
//        TB0CCTL1 |= CCIE;//Enable TimerB0_1
//        P1OUT ^= RED_LED;
        if(DAC_data > 900){
            DAC_data -= 50;
            SAC3DAT = DAC_data;
        }
        break;
    case 4: // CCR2 used for ADC Sampling. 1 Sample every 20ms, 60ms for all 3 samples
//        P2IFG &= ~SW2;
//        P2IE |= SW2;
//        TB0CCTL2 &= ~CCIFG;
//        TB0CCTL2 &= ~CCIE;
//        TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR1
//        TB0CCTL0 |= CCIE;
        /*strcpy(display_line[0], "          ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;*/
        TB0CCR2 += TB0CCR2_INTERVAL;
        ADCCTL0 |= ADCSC; // Start next sample
        break;
    case 14: // overflow
        //...... Add What you need happen in the interrupt ......
        break;
    default: break;
    }
    //----------------------------------------------------------------------------
}

//#pragma vector=TIMER0_B1_VECTOR
//__interrupt void TIMER0_B1_ISR(void){
//    P1OUT ^= RED_LED;
//    //----------------------------------------------------------------------------
//    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//    //----------------------------------------------------------------------------
//    switch(__even_in_range(TB0IV,14)){
//    case  0: break;                    // No interrupt
//    case  2:                           // CCR1 Used for SW1 Debounce
//        P6OUT ^= GRN_LED;
//        count_debounce_SW1++;
//        if (count_debounce_SW1 >= DEBOUNCE_TIME){
//            count_debounce_SW1 = 0;
//
//            TB0CCTL1 &= ~CCIE;
//            P4IFG &= ~SW1;
//            P4IE  |= SW1;
//
//            TB0CCTL0 |= CCIE;
//        }
//
//        if(DAC_data > 900){
//            DAC_data -= 50;
//            SAC3DAT = DAC_data;
//        }
//        break;
//
//    case  4:                           // CCR2 Used for SW2 Debounce
//        count_debounce_SW2++;
//        if (count_debounce_SW2 >= DEBOUNCE_TIME){
//            count_debounce_SW2 = 0;
//
//            TB0CCTL2 &= ~CCIE;
//            P2IFG &= ~SW2;
//            P2IE  |=  SW2;
//
//            TB0CCTL0 |= CCIE;
//        }
//
//        break;
//
//    case 14:                           // overflow available for greater than 1 second timer
//        break;
//    default: break;
//    }
    //----------------------------------------------------------------------------

