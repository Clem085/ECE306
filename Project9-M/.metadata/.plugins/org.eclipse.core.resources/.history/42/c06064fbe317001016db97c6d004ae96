/*
 * ADC.c
 *
 *  Created on: Mar 2, 2025
 *      Author: torab
 */

//// Header Files
#include  "msp430.h"          // MSP430 microcontroller header
#include  "functions.h"       // Custom functions
#include  "LCD.h"             // LCD display control
#include  "ports.h"          // Port configurations
#include  "macros.h"         // Macro definitions
#include  "motors.h"         // Motor control functions
#include  "Display.h"        // Display handling
#include  "timers.h"         // Timer functions
#include  "interrupts.h"     // Interrupt handlers
#include  "switches.h"       // Switch handling
#include  "ADC.h"           // ADC declarations
#include  "IR.h"            // Infrared sensor handling
#include  "serial.h"        // Serial communication
#include  "menu.h"          // Menu system
//// Libraries
#include  <string.h>        // String manipulation
#include  <stdio.h>         // Standard I/O

// Global Variables declared and referenced in Header file

/**
 * Initializes ADC (Analog-to-Digital Converter) for:
 * - Left detector (A2/P2.0)
 * - Right detector (A3/P2.1)
 * - Thumbwheel (A5/P2.5)
 * Configures ADC registers and enables interrupts
 */
void Init_ADC(void){
    //-----------------------------------------------------------------------------
    // Pin assignments:
    // V_DETECT_L      (0x04) // Pin 2 A2
    // V_DETECT_R      (0x08) // Pin 3 A3
    // V_THUMB         (0x20) // Pin 5 A5
    //-----------------------------------------------------------------------------

    // ADCCTL0 Register Configuration
    ADCCTL0 = 0;                        // Reset register
    ADCCTL0 |=  ADCSHT_2;               // 16 ADC clock cycles for sample-and-hold
    ADCCTL0 |=  ADCMSC;                 // Enable multiple sample-and-conversion
    ADCCTL0 |=  ADCON;                  // Turn ADC on

    // ADCCTL1 Register Configuration
    ADCCTL1 = 0;                // Reset register
    ADCCTL1 |=  ADCSHS_0;       // ADCSC bit triggers sampling
    ADCCTL1 |=  ADCSHP;         // Use sampling timer for sample-and-hold
    ADCCTL1 &= ~ADCISSH;        // Don't invert sample-and-hold signal
    ADCCTL1 |=  ADCDIV_0;       // Clock divider = 1 (no division)
    ADCCTL1 |=  ADCSSEL_0;      // Use MODCLK as clock source
    ADCCTL1 |=  ADCCONSEQ_0;    // Single-channel, single-conversion mode
    // Note: ADCBUSY bit indicates active conversion

    // ADCCTL2 Register Configuration
    ADCCTL2 = 0;                // Reset register
    ADCCTL2 |= ADCPDIV0;        // Pre-divider setting
    ADCCTL2 |= ADCRES_2;        // 12-bit resolution
    ADCCTL2 &= ~ADCDF;          // Disable data format adjustment
    ADCCTL2 &= ~ADCSR;          // No special resolution setting

    // ADCMCTL0 Register Configuration
    ADCMCTL0 |=  ADCSREF_0;     // Use AVCC/AVSS as voltage reference
    ADCMCTL0 |=  ADCINCH_2;     // Start with channel 2 (A2) as input
    ADCIE    |=  ADCIE0;        // Enable ADC interrupt
    ADCCTL0  |=  ADCENC;        // Enable conversions
    ADCCTL0  |=  ADCSC;         // Start first conversion

    ADC_Prev = 8192;            // Initialize previous ADC value to mid-range (12-bit)
}

/**
 * Converts hexadecimal ADC value to BCD format for display
 * @param hex_value The raw ADC value to convert
 * Output stored in global adc_char array as 4-digit string
 */
void HexToBCD(int hex_value){
    int value;
    int i;
    // Initialize all digits to '0'
    for(i=0; i < 4; i++) {
        adc_char[i] = '0';
    }

    // Calculate thousands place
    value = 0;
    while (hex_value > 999){
        hex_value = hex_value - 1000;
        value = value + 1;
        adc_char[0] = 0x30 + value;  // Convert to ASCII
    }

    // Calculate hundreds place
    value = 0;
    while (hex_value > 99){
        hex_value = hex_value - 100;
        value = value + 1;
        adc_char[1] = 0x30 + value;  // Convert to ASCII
    }

    // Calculate tens place
    value = 0;
    while (hex_value > 9){
        hex_value = hex_value - 10;
        value = value + 1;
        adc_char[2] = 0x30 + value;  // Convert to ASCII
    }

    // Units place
    adc_char[3] = 0x30 + hex_value;  // Convert to ASCII
}
