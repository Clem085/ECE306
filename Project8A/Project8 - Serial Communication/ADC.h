//===========================================================================
//  File Name: ADC.h
//  Description: Header file for ADC.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Oct 14, 2024
//===========================================================================
#ifndef ADC_H_
#define ADC_H_

// Global Variables
//// Internally Defined
volatile unsigned int ADC_Channel;
volatile unsigned int ADC_Left_Detect;
volatile unsigned int ADC_Right_Detect;
volatile unsigned int ADC_Thumb;
int ADC_Prev;
int ADC_Temp;
char ADC_Changed;
char ADC_Update;
char ADC_Display;
char adc_char[10];
//// Externally Defined



// Macro Definitions



// Function Declarations
void Init_ADC(void);
void HexToBCD(int);
//void ADC_Process(void);
//void ADC_control(void);





#endif /* ADC_H_ */
