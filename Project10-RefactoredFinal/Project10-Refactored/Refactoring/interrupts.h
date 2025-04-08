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
////// ADC Interrupts
unsigned int ADC_Channel;
unsigned int ADC_Left_Detect;
unsigned int ADC_Right_Detect;
unsigned int ADC_thumb;
unsigned char update_display;
unsigned char display_changed;
////// Timer Interrupts
unsigned int display_count;
unsigned char display_changed;
unsigned char update_display;
unsigned int Time_Sequence;
unsigned int CCR0_counter;
unsigned int iot_boot_time;
unsigned int system_initialization;
unsigned int moving;
unsigned int moving_flag;
////// Switch Interrupts
unsigned int baud_toggle;

//// Externally Defined
////// Timer Interrupts
extern char display_line[4][11];
extern unsigned int count_debounce_SW1;
extern unsigned int count_debounce_SW2;
extern unsigned int pause_count;
extern unsigned int ping;
extern unsigned int white_counter;
extern unsigned int white_flag;
extern unsigned int arch_counter;
extern unsigned int motorDrain;
////// Switch Interrupts
extern char display_line[4][11];
extern unsigned int state;
extern unsigned int clear_display;
extern unsigned int switchpressed;




// Macro Definitions



// Function Declarations


#endif /* INTERRUPTS_H_ */
