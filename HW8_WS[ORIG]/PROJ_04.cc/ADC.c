#include  "macros.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "msp430.h"
#include  <string.h>

// Add this to initialize ADC_Channel if used globally
//extern volatile unsigned int ADC_Channel = 0;  // Initialize to 0

// Inside Init_ADC
//ADCMCTL0 |= ADCINCH_5;

//ADCMCTL0 |= ADCINCH_5;

// Set the ADC conversion start logic only once

void Init_ADC(void){
//------------------------------------------------------------------------------
// V_DETECT_L (0x04) // Pin 2 A2
// V_DETECT_R (0x08) // Pin 3 A3
// V_THUMB (0x20) // Pin 5 A5
//------------------------------------------------------------------------------

 //ADCMEM0 = ???
 //ADC_Channel = ???

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
 ADCMCTL0 |= ADCSREF_0; // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
 ADCMCTL0 |= ADCINCH_5; // V_THUMB (0x20) Pin 5 A5
 ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
 ADCCTL0 |= ADCENC; // ADC enable conversion.
 ADCCTL0 |= ADCSC; // ADC start conversion.
}
