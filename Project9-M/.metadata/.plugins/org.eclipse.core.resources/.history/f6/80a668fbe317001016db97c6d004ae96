//===========================================================================
// File Name : Display.h
//
// Description: Display control interface for LCD module
//              - Manages backlight control (PWM and on/off)
//              - Handles text display formatting
//              - Provides ADC value display functions
//
// Created on: Jan, 2025
// Author: Molana Torabi
//===========================================================================

#ifndef DISPLAY_H_
#define DISPLAY_H_

// Global Variables
//// Internally Defined
char backlight_status;       // Current state of backlight (ON/OFF)
char light_percent;         // Backlight brightness level (0-100%)
int middleChar(char * string); // Calculates center position for text (deprecated)

//// Externally Defined (from other modules)
extern char display_line[4][11];       // Display buffer (4 lines, 10 chars + null)
extern volatile unsigned char display_changed;  // Flag indicating display content changed
extern volatile unsigned char update_display;   // Flag requesting display refresh
extern int Switch_Counter1;           // Switch debounce counter
extern char adc_char[10];             // Buffer for ADC values (BCD format)

// Macro Definitions
// PWM Backlight Control
#define LCD_BACKLITE_DIMING (TB3CCR1)  // Timer B3 CCR1 register for PWM control
#define PERCENT_100 (50000)            // PWM value for 100% brightness
#define PERCENT_80  (45000)            // PWM value for 80% brightness

// Function Declarations
/**
 * Controls backlight on/off state (deprecated - use PWM_backlight instead)
 */
void backlight_control(void);

/**
 * Prints and centers text on specified display line
 * @param line Text string to display (max 10 chars)
 * @param lineToUpdate Line number (1-4) to update
 */
void dispPrint(char *line, char lineToUpdate);

/**
 * Inserts ADC values into specific display position
 * @param line Display line number (1-4)
 * @location Starting character position (0-9)
 */
void adc_line(char line, char location);

/**
 * Controls backlight brightness using PWM
 * Uses light_percent global variable (0-100%)
 */
void PWM_backlight(void);

#endif /* DISPLAY_H_ */
