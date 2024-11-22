//------------------------------------------------------------------------------
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "ADC.h"
#include  "DAC.h"

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Init_Clocks(void);
void Init_Timer_B0(void);
void Init_Timer_B3(void);
void Init_LCD(void);


void HEXtoBCD(int hex_value);
void adc_line(char line, char location);

char adc_char[200];

int ADC_Left_Detect;
int ADC_Right_Detect;
int ADC_Forward_Detect;

int ADC_Channel;

extern volatile unsigned int Time_Sequence = 0;


void main(void){
//    enable_interrupts();

    Time_Sequence = 0;

//    ADC_Left_Detect = 0;
//    ADC_Right_Detect = 0;
//    ADC_Forward_Detect = 0;

    //WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer

    Init_Ports();

    P6OUT |= LCD_BACKLITE;

    Init_Conditions();
//    Init_Clocks();
    Init_Timer_B0();
    Init_Timer_B3();
    Init_LCD();

    strcpy(display_line[0], "  ECE306  ");
    strcpy(display_line[1], "  CORINNE ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;

    while(ALWAYS){
        P6OUT |= LCD_BACKLITE;

        P1OUT |= RED_LED;
        Display_Process();

    }
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
    strcpy(display_line[0], display_line[0]);
}
//-----------------------------------------------------------------


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
                     ADC_Left_Detect = ADCMEM0; // Move result into Global
                     ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4
                     HEXtoBCD(ADC_Left_Detect); // Convert result to String
                     adc_line(0, 0); // Place String in Display
                     ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
                     ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
                     break;
                 case 0x01:
                     ADC_Right_Detect = ADCMEM0; // Move result into Global Values
                     ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A2
                     HEXtoBCD(ADC_Right_Detect); // Convert result to String
                     adc_line(0, 0); // Place String in Display
                     // Could be wrong on this
                     ADCMCTL0 |= ADCINCH_4; // Enable Next channel ?
                     break;
                 //I Could be wrong about this:
                 case 0x02:
                     //I could be wrong about this
                     ADC_Forward_Detect = ADCMEM0; // Move result into Global Values
                 // I could be wrong about this
                     HEXtoBCD(ADC_Forward_Detect); // Convert result to String
                      adc_line(0, 0); // Place String in Display
                      ADCMCTL0 &= ~ADCINCH_1; // Disable Last channel A?
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


//------------------------------------------------------------------------------
