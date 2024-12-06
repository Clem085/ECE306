//===========================================================================
//  File Name: switches.h
//  Description: Header file for switches.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Sep 20, 2024
//===========================================================================
#ifndef SWITCHES_H_
#define SWITCHES_H_



// Global Variables
//// Internally Defined
unsigned int count_debounce_SW1;
unsigned int count_debounce_SW2;

//volatile int switch1_pressed;  // Declare the variable as external
//volatile int switch2_pressed;  // Declare the variable as external



/// /// /// TEMP PROJECT 8 CODE /// /// ///
// Commented all of these out because switches.h is not included is Source switches.c
// Global Variables declared and referenced in Header file
/// /// /// TEMP PROJECT 8 CODE /// /// ///
// Global variables for debounce
// SW1 debounce counter
volatile unsigned int debounce_count1;
volatile unsigned int debounce_count2;


// Commented Out, Caused Errors


extern volatile unsigned int localCounter;



// Increment for the debounce interval for the switches
#define increment 3277;
extern volatile int switch1_pressed;  // Declare the variable as external
extern volatile int switch2_pressed;  // Declare the variable as external
extern volatile unsigned char update_display;
extern volatile unsigned int Time_Sequence;

/// /// /// TEMP PROJECT 8 CODE /// /// ///

//// Externally Defined
extern unsigned char dispEvent;
extern volatile unsigned char display_changed;
extern unsigned char event;
extern char display_line[4][11];
extern unsigned int straight_step;
extern unsigned int circle_step;
extern unsigned int circle_step2;
extern unsigned int triangle_step;
extern unsigned int figure8_step;
extern char backlight_status;
extern volatile unsigned int Time_Sequence;
extern int activateSM;
extern char baud_flag;

// PREJECT8 TEMP
//volatile char switch_debounce_active;

// Macro Definitions
#define DEBOUNCE_TIME          (6)
#define DEBOUNCE_RESTART       (0)



// Function Declarations
void Switch1_Proj5_Process(void);
void SW1_Project8(void);
void SW2_Project8(void);
void SW1_Homework9(void);
void SW2_Homework9(void);
void SW1_Project9(void);
void SW2_Project9(void);
//void SW1_Project9_TEMP(void);
//void SW2_Project9_TEMP(void);
void disable_switch_interrupts(void);

// Enable Switches
void enable_switch_SW1(void);
void enable_switch_SW2(void);
void enable_switches(void);
// Disable Switches
void disable_switch_SW1(void);
void disable_switch_SW2(void);
void disable_switches(void);
// Debounce
void debounce(void);
void debounceSW1(void);
void debounceSW2(void);


#endif /* SWITCHES_H_ */
