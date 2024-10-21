/*
 * interrupts.c
 *
 *  Created on: Oct 21, 2024
 *      Author: jammi
 */


// #include as of 10-21-24
// Header Files
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "motors.h"
#include  "Display.h"
#include  "timers.h"
#include  "switches.h"
#include  "ADC.h"
#include  "IR.h"
// Libraries
#include  <string.h>
#include  <stdio.h>



// External Globals
extern volatile unsigned char update_display;

volatile unsigned int Time_Sequence;
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern unsigned int count_debounce_SW1;
extern unsigned int count_debounce_SW2;

extern volatile unsigned int ADC_Channel;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
extern volatile unsigned int ADC_Thumb;
extern char state;
//extern volatile unsigned int State_Sequence;
extern char adc_char[10];
extern char IR_status;
extern char IR_changed;
//extern char ADC_Update;

// Timers
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //    ADC_Update = TRUE;
    //-----------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //---------------------------------------------------------------------------
    update_display = TRUE;
    Time_Sequence++;
    //    State_Sequence = Time_Sequence;
    //    P6OUT ^= LCD_BACKLITE;
    TB0CCR0 += TB0CCR0_INTERVAL;
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
    //---------------------------------------------------------------------------
    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
    //---------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
    case  0: break;                    // No interrupt
    case  2:                           // CCR1 Used for SW1 Debounce
        count_debounce_SW1++;
        if (count_debounce_SW1 >= DEBOUNCE_TIME){
            count_debounce_SW1 = 0;

            TB0CCTL1 &= ~CCIE;
            P4IFG &= ~SW1;
            P4IE  |= SW1;

            //            TB0CCTL0 |= CCIE;
        }

        TB0CCR1 += TB0CCR1_INTERVAL;


        break;

    case  4:                           // CCR2 Used for SW2 Debounce
        count_debounce_SW2++;
        if (count_debounce_SW2 >= DEBOUNCE_TIME){
            count_debounce_SW2 = 0;

            TB0CCTL2 &= ~CCIE;
            P2IFG &= ~SW2;
            P2IE  |=  SW2;

            //            TB0CCTL0 |= CCIE;
        }


        TB0CCR2 += TB0CCR2_INTERVAL;

        break;

    case 14:                           // overflow available for greater than 1 second timer
        break;
    default: break;
    }
}











#pragma vector = TIMER0_B3_VECTOR
__interrupt void Timer0_B3_ISR(void){
    //-----------------------------------------------------------------------------
    // TimerB3 0 Interrupt handler
    //---------------------------------------------------------------------------
//    update_display = TRUE;
    //    State_Sequence = Time_Sequence;
    //    P6OUT ^= LCD_BACKLITE;
    TB3CCR0 += TB3CCR0_INTERVAL;
}


    // Switches
    //-----------------------------------------------------------------------------
    // Port 4 interrupt for switch 1, it is disabled for the duration
    // of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT4_VECTOR
    __interrupt void switch1_interrupt(void) {
        // Switch 1
        if (P4IFG & SW1) {

            P4IE &= ~SW1;
            P4IFG &= ~SW1;

            TB0CCTL1 &= ~CCIFG;             // Clear SW1 debounce interrupt flag
            TB0CCR1 = TB0CCR1_INTERVAL;     // CCR1 add offset
            TB0CCTL1 |= CCIE;               // CCR1 enable interrupt

            //SW1 FUNCTIONS:
            state = WAIT;
            Time_Sequence = 0;
            //        State_Sequence = 0;

        }
        //-----------------------------------------------------------------------------
    }
    //-----------------------------------------------------------------------------
    // Port 2 interrupt for switch 2, it is disabled for the duration
    // of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT2_VECTOR
    __interrupt void switch2_interrupt(void) {
        // Switch 2
        if (P2IFG & SW2) {

            P2IE &= ~SW2;
            P2IFG &= ~SW2;

            TB0CCTL2 &= ~CCIFG;             // Clear SW2 debounce interrupt flag
            TB0CCR2 = TB0CCR2_INTERVAL;     // CCR2 add offset
            TB0CCTL2 |= CCIE;               // CCR2 enable interrupt

            //SW2 FUNCTIONS:
            //        // Implement Later
            //        ADC_Update ^= 1; // Toggles the State of ADC_Update, Makes ADC Values Appear/Disappear on LCD
            //        if(!ADC_Update){
            //            strcpy(display_line[0], "          ");
            //            strcpy(display_line[1], "          ");
            //            strcpy(display_line[2], "          ");
            //            strcpy(display_line[3], "          ");
            //            update_display =TRUE;
            //            backlight_status = OFF;
            //        }
            if(IR_status == OFF){
                IR_changed = TRUE;
                strcpy(display_line[0], "  IR ON   ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                update_display = TRUE;
                IR_status = ON;
            }
            else{// IR_status = ON
                IR_changed = TRUE;
                strcpy(display_line[0], "  IR OFF  ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                update_display =TRUE;
                IR_status = OFF;
            }


        }
        //-----------------------------------------------------------------------------
    }









    // ADC Interrupt
#pragma vector=ADC_VECTOR
    __interrupt void ADC_ISR(void){
        //    backlight_status = ON;
        switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:   // When a conversion result is written to the ADCMEM0
            // before its previous conversion result was read.
            break;
        case ADCIV_ADCTOVIFG:   // ADC conversion-time overflow
            break;
        case ADCIV_ADCHIIFG:    // Window comparator interrupt flags
            break;
        case ADCIV_ADCLOIFG:    // Window comparator interrupt flag
            break;
        case ADCIV_ADCINIFG:    // Window comparator interrupt flag
            break;
        case ADCIV_ADCIFG:      // ADCMEM0 memory register with the conversion result
            ADCCTL0 &= ~ADCENC;                          // Disable ENC bit.
            switch (ADC_Channel++){
            case 0x00:                                   // Channel A2 Interrupt
                ADCMCTL0 &= ~ADCINCH_2;                  // Disable Last channel A2
                ADCMCTL0 |=  ADCINCH_3;                  // Enable Next channel A3

                ADC_Left_Detect = ADCMEM0;               // Move result into Global Values
                ADC_Left_Detect = ADC_Left_Detect >> 2;  // Divide the result by 4

                if(state != WAIT2 && state != BLACKLINE){
                    HexToBCD(ADC_Left_Detect);
                    dispPrint(adc_char,2);
                }

                //            ADC_Update = TRUE;

                break;

            case 0x01:                                   // Channel A3 Interrupt
                ADCMCTL0 &= ~ADCINCH_3;                  // Disable Last channel A2
                ADCMCTL0 |=  ADCINCH_5;                  // Enable Next channel A1

                ADC_Right_Detect = ADCMEM0;              // Move result into Global Values
                ADC_Right_Detect = ADC_Right_Detect >> 2;// Divide the result by 4
                if(state != WAIT2 && state != BLACKLINE){
                    HexToBCD(ADC_Left_Detect);
                    dispPrint(adc_char,3);
                }
                //            ADC_Update = TRUE;

                break;

            case 0x02:                                   // Channel A1 Interrupt
                ADCMCTL0 &= ~ADCINCH_5;                  // Disable Last channel A?
                ADCMCTL0 |= ADCINCH_2;                   // Enable Next [First] channel 2

                ADC_Thumb = ADCMEM0;                     // Move result into Global Values
                ADC_Thumb = ADC_Thumb >> 2;              // Divide the result by 4
                if(state != WAIT2 && state != BLACKLINE){
                    HexToBCD(ADC_Left_Detect);
                    dispPrint(adc_char,4);
                }
                //            ADC_Update = TRUE;

                ADC_Channel = 0;
                break;

            default:
                break;
            }
            ADCCTL0 |= ADCENC;                          // Enable Conversions
            ADCCTL0 |= ADCSC;
            break;
            default:
                break;
        }
    }
