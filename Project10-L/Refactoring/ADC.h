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
int ADC_Prev;
int ADC_Temp;
char adc_char[4];

//// Externally Defined
extern char *display[4];
extern char display_line[4][11];


// Macro Definitions



// Function Declarations
void Init_ADC(void);
void HexToBCD(int);





#endif /* ADC_H_ */
