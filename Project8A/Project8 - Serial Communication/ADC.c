/*
 * adc.c
 *
 *  Created on: Oct 14, 2024
 *      Author: jammi
 */
//#include  "msp430.h"
//#include  <string.h>
//#include <switches.h>
//#include  "functions.h"
//#include  "LCD.h"
//#include  "ports.h"
//#include  "macros.h"
//#include  "motors.h"
//#include  "Display.h"
//#include  "timersB0.h"
//#include  "switches.h"
//#include  "ADC.h"

// #include as of 11-08-24
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
#include  "serial.h"
#include  "DAC.h"
// Libraries
#include  <string.h>
#include  <stdio.h>



extern char backlight_status;
//extern char state;


// Globals
//volatile unsigned int ADC_Value;
volatile unsigned int ADC_Channel;
volatile unsigned int ADC_Left_Detect;
volatile unsigned int ADC_Right_Detect;
volatile unsigned int ADC_Thumb;
char ADC_Update;
char ADC_Display;
char adc_char[10];




// Init ADC
void Init_ADC(void){
    //-----------------------------------------------------------------------------
    // V_DETECT_L      (0x04) // Pin 2 A2
    // V_DETECT_R      (0x08) // Pin 3 A3
    // V_THUMB         (0x20) // Pin 5 A5
    //-----------------------------------------------------------------------------
    // ADCCTL0 Register
    ADCCTL0 = 0;                        // Reset
    ADCCTL0 |=  ADCSHT_2;                // 16 ADC clocks
    ADCCTL0 |=  ADCMSC;                  // MSC
    ADCCTL0 |=  ADCON;                   // ADC ON

    // ADCCTL1 Register
    ADCCTL1 = 0;                // Reset
    ADCCTL1 |=  ADCSHS_0;        // 00b = ADCSC bit
    ADCCTL1 |=  ADCSHP;          // ADC sample-and-hold SAMPCON signal from sampling timer
    ADCCTL1 &= ~ADCISSH;        // ADC invert signal sample-and-hold.
    ADCCTL1 |=  ADCDIV_0;        // ADC clock divider - 000b = Divide by 1
    ADCCTL1 |=  ADCSSEL_0;       // ADC clock MODCLK
    ADCCTL1 |=  ADCCONSEQ_0;     // ADC conversion sequence 00b = Single-channel single-conversion
    // ADCCTL1 & ADCBUSY  identifies a conversion is in process

    // ADCCTL2 Register
    ADCCTL2 = 0;
    ADCCTL2 |= ADCPDIV0;
    ADCCTL2 |= ADCRES_2;
    ADCCTL2 &= ~ADCDF;
    ADCCTL2 &= ~ADCSR;

    // ADCMCTL0 Register
    ADCMCTL0 |=  ADCSREF_0;
    ADCMCTL0 |=  ADCINCH_2;
    ADCIE    |=  ADCIE0;
    ADCCTL0  |=  ADCENC;
    ADCCTL0  |=  ADCSC;

}

//Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
void HexToBCD(int hex_value){
    int value;
    int i;
    for(i=0; i < 4; i++) {
        adc_char[i] = '0';
    }
    value = 0;
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

//// DECPRECATED - Good Coding Practices = Bad RESULTS
//// ADC Process
//void ADC_Process(void){
//    if(ADC_Update && state!=WAIT2){
//        ADC_Update = 0;
//        switch (ADC_Channel){
//        case 0x00: // Left_Detect
//            HexToBCD(ADC_Left_Detect);
//            dispPrint(adc_char,2);
//            break;
//        case 0x01: // Right Detect
//            HexToBCD(ADC_Right_Detect);
//            dispPrint(adc_char,3);
//            break;
//        case 0x02: // Thumb Wheel
//            HexToBCD(ADC_Thumb);
//            dispPrint(adc_char,4);
//        default: break;
//
//        }
//
//    }
//}

//void ADC_control(void){
//    if(ADC_Update && ADC_Display){
//        switch(ADC_Channel){
//        case 0x00:
//            HexToBCD(ADC_Left_Detect);
//            dispPrint(adc_char,2);
//            break;
//        case 0x01:
//            HexToBCD(ADC_Right_Detect);
//            dispPrint(adc_char,3);
//            break;
//        case 0x02:
//            HexToBCD(ADC_Thumb);
//            dispPrint(adc_char,4);
//            break;
//        default:
//            break;
//        }
//    }
//}

