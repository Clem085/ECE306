//===========================================================================
// File Name : init.c
//
// Description: System initialization routines
//              - Initializes display buffers
//              - Configures default hardware states
//              - Sets up interrupt handling
//              - Initializes system variables
//
// Created on: Jan, 2025
// Author: Molana Torabi
//===========================================================================

//// Header Files
#include  "msp430.h"          // MSP430 MCU headers
#include  "functions.h"       // Common utility functions
#include  "LCD.h"            // LCD display control
#include  "ports.h"          // Port and pin definitions
#include  "macros.h"         // System macros and constants
#include  "motors.h"         // Motor control functions
#include  "Display.h"        // Display handling
#include  "timers.h"         // Timer configurations
#include  "interrupts.h"     // Interrupt handling
#include  "switches.h"       // Switch debouncing
#include  "ADC.h"           // Analog-to-Digital Converter
#include  "IR.h"            // Infrared sensor interface
#include  "serial.h"        // Serial communication
#include  "menu.h"          // Menu system interface
//// Libraries
#include  <string.h>        // String manipulation
#include  <stdio.h>         // Standard I/O functions

// External Variable Declarations
extern char display_line[4][11];          // Display buffer (4 lines x 10 chars + null)
extern char *display[4];                 // Display line pointers
extern volatile unsigned char update_display;  // Display update flag
extern volatile unsigned char display_changed; // Display content changed flag
unsigned char display_mode;              // Current display mode
extern unsigned int count_debounce_SW1;  // Switch 1 debounce counter
extern unsigned int count_debounce_SW2;  // Switch 2 debounce counter
extern char IR_status;                  // Infrared sensor status
extern char ADC_Update;                 // ADC data ready flag
extern char ADC_Display;                // ADC display control flag
extern char state;                      // System state machine state
extern char light_percent;              // Backlight brightness level
extern char DAC_overflow_counter;       // DAC overflow counter

/**
 * Initializes system to default conditions:
 * - Clears display buffers
 * - Sets up display line pointers
 * - Enables global interrupts
 * - Initializes hardware states
 * - Resets system variables
 */
void Init_Conditions(void){
//------------------------------------------------------------------------------
  // Initialize display buffers
  int i;
  for(i=0;i<11;i++){
    display_line[0][i] = RESET_STATE;  // Clear line 0
    display_line[1][i] = RESET_STATE;  // Clear line 1
    display_line[2][i] = RESET_STATE;  // Clear line 2
    display_line[3][i] = RESET_STATE;  // Clear line 3
  }
  // Null-terminate each display line
  display_line[0][10] = 0;
  display_line[1][10] = 0;
  display_line[2][10] = 0;
  display_line[3][10] = 0;

  // Set up display line pointers
  display[0] = &display_line[0][0];  // Pointer to line 0
  display[1] = &display_line[1][0];  // Pointer to line 1
  display[2] = &display_line[2][0];  // Pointer to line 2
  display[3] = &display_line[3][0];  // Pointer to line 3

  update_display = 0;  // Clear display update flag

  // Enable global interrupts (disabled by default on startup)
  enable_interrupts();

  // Initialize system variables
  count_debounce_SW1 = 0;  // Reset switch 1 debounce counter
  count_debounce_SW2 = 0;  // Reset switch 2 debounce counter
  DAC_overflow_counter = 0; // Reset DAC overflow counter

  // Initialize hardware states
  P1OUT  &= ~RED_LED; // Turn Red LED off
  P6OUT  &= ~GRN_LED; // Turn Green LED off
//------------------------------------------------------------------------------
}
