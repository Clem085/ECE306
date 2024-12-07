/* Interrupts Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : interrupts.c
  Description:  This file contains the code to control all ISR (Interrupt Service Routines)
  Programmer: Connor Savugot
  Date Created: Oct 21, 2024
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
volatile unsigned char update_display;
volatile unsigned char display_changed;

// // // ADC Interrupt // // //
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
    case ADCIV_NONE:
        break;
    case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
        // before its previous conversion result was read.
        break;
    case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
        break;
    case ADCIV_ADCHIIFG: // Window comparator interrupt flags
        break;
    case ADCIV_ADCLOIFG: // Window comparator interrupt flag
        break;
    case ADCIV_ADCINIFG: // Window comparator interrupt flag
        break;
    case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
        ADCCTL0 &= ~ADCENC; // Disable ENC bit.
        switch (ADC_Channel++){
        case 0:                                      // Channel A2 Interrupt
            ADCMCTL0 &= ~ADCINCH_2;                  // Disable Last channel A2
            ADCMCTL0 |=  ADCINCH_3;                  // Enable Next channel A3

            ADC_Left_Detect = ADCMEM0;               // Move result into Global Values
            ADC_Left_Detect = ADC_Left_Detect >> 2;  // Divide the result by 4
            break;
        case 1:                                       // Channel A3 Interrupt
            ADCMCTL0 &= ~ADCINCH_3;                   // Disable Last channel A2
            ADCMCTL0 |=  ADCINCH_5;                   // Enable Next channel A1

            ADC_Right_Detect = ADCMEM0;               // Move result into Global Values
            ADC_Right_Detect = ADC_Right_Detect >> 2; // Divide the result by 4
            break;

        case 2:                                       // Channel A1 Interrupt
            ADCMCTL0 &= ~ADCINCH_5;                   // Disable Last channel A?
            ADCMCTL0 |= ADCINCH_2;                    // Enable Next [First] channel 2

            ADC_Thumb = ADCMEM0;                      // Move result into Global Values
            ADC_Thumb = ADC_Thumb >> 2;               // Divide the result by 4
            ADC_Channel = 0;
            break;

        default:
            break;
        }
        ADCCTL0 |= ADCENC; // Enable Conversions
        break;
        default:    break;
    }
}
