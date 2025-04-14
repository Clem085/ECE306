/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : interrupts.h
  Description:  Header file for interrupt service routines (ISRs)
                - Declares global variables shared between ISRs and main program
                - Defines system-wide interrupt-related constants
  Programmer: Molana
  Date Created: March, 2025
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

// Global Variables
//// Internally Defined
volatile unsigned int Time_Sequence;    // System time counter (incremented by timer ISR)
char DAC_overflow_counter;             // Counter for DAC overflow events

//// Externally Defined (from other modules)
extern volatile unsigned char update_display;  // Flag to trigger display refresh
extern char display_line[4][11];              // Display buffer (4 lines x 10 chars + null)
extern volatile unsigned char display_changed; // Flag indicating display content changed
extern unsigned int count_debounce_SW1;       // Switch 1 debounce counter
extern unsigned int count_debounce_SW2;       // Switch 2 debounce counter

// ADC-related variables (commented out but kept for reference)
//extern volatile unsigned int ADC_Channel;
//extern volatile unsigned int ADC_Left_Detect;
//extern volatile unsigned int ADC_Right_Detect;
//extern volatile unsigned int ADC_Thumb;

extern char state;            // System state machine state
extern char adc_char[10];     // Buffer for ADC values in string format
extern char IR_status;        // Current IR sensor status (ON/OFF)
extern char IR_changed;       // Flag indicating IR status change
extern char ADC_Update;       // Flag to enable ADC display updates
extern char ADC_Display;      // Flag to control ADC display visibility
extern unsigned int DAC_data; // Current DAC output value
extern char menu;            // Current menu selection
extern char menuType;        // Type of menu being displayed
extern char songInit;        // Flag for song initialization
extern unsigned int clear_display; // Flag to clear display

// Serial communication variables (commented out but kept for reference)
//extern volatile char Rx_display[16];
//extern char baud_flag;

// Macro Definitions
// (Placeholder for any interrupt-related macros)

// Function Declarations
// (Placeholder for any interrupt-related function prototypes)

#endif /* INTERRUPTS_H_ */
