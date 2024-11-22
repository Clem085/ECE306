#include "macros.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "msp430.h"
#include  <string.h>
#include "DAC.h"

unsigned int DAC_data = 4000;

void Init_DAC(void){
     //unsigned int DAC_data = 4000;  // Local variable
     //DAC_data = 4000; // Value between 0x000 and 0x0FFF
     SAC3DAT = DAC_data; // Initial DAC data
     SAC3DAC = DACSREF_1; // Select int Vref as DAC reference
     SAC3DAC |= DACLSEL_0; // DAC latch loads when DACDAT written
     SAC3DAC |= DACEN; // Enable DAC

     SAC3OA = NMUXEN; // SAC Negative input MUX controL
     SAC3OA |= PMUXEN; // SAC Positive input MUX control
     SAC3OA |= PSEL_1; // 12-bit reference DAC source selected
     SAC3OA |= NSEL_1; // Select negative pin input
     SAC3OA |= OAPM; // Select low speed and low power mode
     SAC3PGA = MSEL_1; // Set OA as buffer mode
     SAC3OA |= SACEN; // Enable SAC
     SAC3OA |= OAEN; // Enable OA
}
