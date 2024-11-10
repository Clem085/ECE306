//===========================================================================
//  File Name: interrupts.h
//  Description: Header file for interrupts.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Oct 21, 2024
//===========================================================================
#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

// Global Variables
//// Internally Defined
volatile unsigned int Time_Sequence;
char DAC_overflow_counter;
//// Externally Defined
extern volatile unsigned char update_display;
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern unsigned int count_debounce_SW1;
extern unsigned int count_debounce_SW2;
extern volatile unsigned int ADC_Channel;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
extern volatile unsigned int ADC_Thumb;
extern char state;
extern char adc_char[10];
extern char IR_status;
extern char IR_changed;
extern char ADC_Update;
extern char ADC_Display;
extern unsigned int DAC_data;



// Macro Definitions



// Function Declarations



#endif /* INTERRUPTS_H_ */
