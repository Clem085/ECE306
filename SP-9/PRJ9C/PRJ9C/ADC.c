/*
 * ADC.c
 *
 *  Created on: Feb 26, 2025
 *      Author: price
 */
#include  "msp430.h"
#include  <string.h>
#include  "include\functions.h"
#include  "Include\LCD.h"
#include  "include\ports.h"
#include "macros.h"
#include "include\shapes.h"

extern volatile unsigned char display_changed;
extern char display_line[4][11];
extern volatile unsigned char update_display;

unsigned int ADC_Channel;
unsigned int adc_char[];
unsigned int ADC_Left_Detect;
unsigned int ADC_Right_Detect;
unsigned int ADC_Thumb;
int i;
int j;
unsigned int hex_value;
unsigned int Timer_value;
extern char PRJ7Timer[];

void Init_ADC(void){
//------------------------------------------------------------------------------
// V_DETECT_L (0x04) // Pin 2 A2
// V_DETECT_R (0x08) // Pin 3 A3
// V_THUMB (0x20) // Pin 5 A5
//------------------------------------------------------------------------------
// ADCCTL0 Register
     ADCCTL0 = 0; // Reset
     ADCCTL0 |= ADCSHT_2; // 16 ADC clocks
     ADCCTL0 |= ADCMSC; // MSC
     ADCCTL0 |= ADCON; // ADC ON
    // ADCCTL1 Register
     ADCCTL1 = 0; // Reset
     ADCCTL1 |= ADCSHS_0; // 00b = ADCSC bit
     ADCCTL1 |= ADCSHP; // ADC sample-and-hold SAMPCON signal from sampling timer.
     ADCCTL1 &= ~ADCISSH; // ADC invert signal sample-and-hold.
     ADCCTL1 |= ADCDIV_0; // ADC clock divider - 000b = Divide by 1
     ADCCTL1 |= ADCSSEL_0; // ADC clock MODCLK
     ADCCTL1 |= ADCCONSEQ_0; // ADC conversion sequence 00b = Single-channel single-conversion
    // ADCCTL1 & ADCBUSY identifies a conversion is in process
    // ADCCTL2 Register
     ADCCTL2 = 0; // Reset
     ADCCTL2 |= ADCPDIV0; // ADC pre-divider 00b = Pre-divide by 1
     ADCCTL2 |= ADCRES_2; // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
     ADCCTL2 &= ~ADCDF; // ADC data read-back format 0b = Binary unsigned.
     ADCCTL2 &= ~ADCSR; // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
    // ADCMCTL0 Register
     ADCMCTL0 |= ADCSREF_0; // VREF - 000b = {VR+ = AVCC and VR� = AVSS }
     ADCMCTL0 |= ADCINCH_2; // V_THUMB (0x20) Pin 5 A5 - enabling channel 2 at start
     ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
     ADCCTL0 |= ADCENC; // ADC enable conversion.
     ADCCTL0 |= ADCSC; // ADC start conversion.
}
//-----------------------------------------------------------------
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//-----------------------------------------------------------------
void HEXtoBCD(int hex_value){
    int value = 0;
    for(i=0; i < 4; i++) {
        adc_char[i] = '0';
    }
    while (hex_value > 999){
        hex_value = hex_value - 1000;
        value = value + 1;
        adc_char[0] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 99){
        hex_value = hex_value - 100;
        value = value + 1;
        adc_char[1] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 9){
        hex_value = hex_value - 10;
        value = value + 1;
        adc_char[2] = 0x30 + value;
    }
    adc_char[3] = 0x30 + hex_value;
}

//-----------------------------------------------------------------
//-------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 9
//
//-------------------------------------------------------------
void adc_line(char line, char location){
//-------------------------------------------------------------
 int i;
 unsigned int real_line;
 real_line = line - 1;
 for(i=0; i < 4; i++) {
     display_line[real_line][i+location] = adc_char[i];
 }
 display_changed = TRUE;
}

//void PRJ7Timer_line(char line, char location){
////-------------------------------------------------------------
// int j;
// unsigned int timer_line;
// timer_line = line - 1;
// for(j=0; j < 4; j++) {
//     display_line[timer_line][j+location] = PRJ7Timer[j];
// }
// display_changed = TRUE;
//}


//-------------------------------------------------------------

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
             case 0x00: // Channel A2 Interrupt
                 ADC_Left_Detect = ADCMEM0; // Move result into Global Values
                 ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4
                 ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
                 ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
                 break;

             case 0x01: // Channel A3 Interrupt
                 ADC_Right_Detect = ADCMEM0; // Move result into Global Values
                 ADC_Right_Detect = ADC_Right_Detect >> 2; // Divide the result by 4
                 ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A2
                 ADCMCTL0 |= ADCINCH_5; // Enable Next channel A3
                 break;

              case 0x02:
                  ADC_Thumb = ADCMEM0; // Move result into Global Values
                  ADC_Thumb = ADC_Thumb >> 2; // Divide the result by 4
                  ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A2
                  ADCMCTL0 |= ADCINCH_2; // Enable First channel 2
                  ADC_Channel=0;
                  break;
                default:
                  break;
          }
          ADCCTL0 |= ADCENC; // Enable Conversions
          ADCCTL0 |= ADCSC; // Start next sample
          default:
              break;
              }

}


