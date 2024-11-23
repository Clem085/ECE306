

int lcd_shape = 0;

#include "macros.h"
#include "msp430.h"
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include <string.h>

void Display_Process(void);

void HEXtoBCD(int hex_value);
void adc_line(char line, char location);

volatile char adc_char[4];

int ADC_Channel;

// Global variables
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned int display_changed;
extern volatile unsigned int update_display;
extern volatile unsigned int update_display_count;
char change;

//------------------------------------------------------------------------------
// Name: Display_Process()
// Description: Updates LCD Display text
//------------------------------------------------------------------------------
void Display_Process(void){ //Function aimed to properly update display
    if(update_display){
        //      switch(lcd_shape){
        //      case 1:
        //          strcpy(display_line[0], "          ");
        //          strcpy(display_line[1], " SWITCH 1 ");
        //          strcpy(display_line[2], "          ");
        //          strcpy(display_line[3], "          ");
        //      break;
        //      case 2:
        //          strcpy(display_line[0], "          ");
        //          strcpy(display_line[1], " SWITCH 2 ");
        //          strcpy(display_line[2], "          ");
        //          strcpy(display_line[3], "          ");
        //      break;
        //      default: break;
        //      }
        update_display = 0;
    }
    if(display_changed){
        display_changed = 0;
        Display_Update(0,0,0,0);
    }
}

void Display_Change(lcdstate){
    update_display = 1;
    lcd_shape = lcdstate;
    display_changed = TRUE;
    Display_Process();
}

int ADC_Channel;

#pragma vector=ADC_VECTOR
interrupt void ADC_ISR(void){
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
                ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
                ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3

                ADCCTL0 |= ADCENC; // Enable Conversions
                ADCCTL0 |= ADCSC; // Start next sample
                break;
            case 0x01: // Channel A3 Interrupt
                ADC_Right_Detect = ADCMEM0; // Move result into Global Values
                ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A2
                ADCMCTL0 |= ADCINCH_5; // Enable Next channel 5

                ADCCTL0 |= ADCENC; // Enable Conversions
                ADCCTL0 |= ADCSC; // Start next sample
                break;
            case 0x02: // Channel 5 Interrupt
                ADC_Thumb = ADCMEM0; // Move result into Global Values
                ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A?
                ADCMCTL0 |= ADCINCH_2; // Enable Next [First] channel 2
                ADC_Thumb = ADC_Thumb >> 2; // Divide the result by 4
                ADC_Channel=0;
                break;
        default:
        break;
    }
    default:
    break;
} // end switch
}// end interrupt

void DAC_Procress(void){
    Display_ADC();
}


//-----------------------------------------------------------------
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//-----------------------------------------------------------------
void HEXtoBCD(int hex_value){
    int value;
    int i;
    for (i = 0; i < 4; i++) {
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
        display_changed = TRUE;
    }
}
//-----------------------------------------------------------------

// Will find get the current ADC left, right, and thumb value
// recorded by the interrupt and then display it
void Display_ADC(void){
    HEXtoBCD(ADC_Right_Detect);
    adc_line(0, 0);
    HEXtoBCD(ADC_Left_Detect);
    adc_line(0, 0);
    HEXtoBCD(ADC_Thumb);
    adc_line(0, 0);
}
