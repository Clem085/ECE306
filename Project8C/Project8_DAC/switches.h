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
volatile int switch1_pressed;  // Declare the variable as external
volatile int switch2_pressed;  // Declare the variable as external

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
volatile char switch_debounce_active;

// Macro Definitions
#define DEBOUNCE_TIME          (6)
#define DEBOUNCE_RESTART       (0)



// Function Declarations
void Switch1_Proj5_Process(void);
void SW1_Project8(void);
void SW1_Project9(void);
void SW1_Homework9(void);
void SW2_Project8(void);
void SW2_Project9(void);
void SW2_Homework9(void);
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
