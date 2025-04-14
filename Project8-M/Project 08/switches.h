/* ============================================================================
 * Switch Interface Header
 *
 * File        : switches.h
 * Description : Defines switch handling interface and debounce parameters
 *               - Declares switch-related variables and functions
 *               - Configures debounce timing constants
 *               - Provides switch control functions
 *
 * Author      : Molana Torabi
 * Created     : January 2025
 * ============================================================================
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

/* ============================================================================
 * Global Variables
 * ============================================================================*/

// Internal Variables
unsigned int count_debounce_SW1;  // SW1 debounce counter (in timer ticks)
unsigned int count_debounce_SW2;  // SW2 debounce counter (in timer ticks)

// External Variables (from other modules)
extern unsigned char dispEvent;           // Display event flag
extern volatile unsigned char display_changed; // Display update flag
extern unsigned char event;               // System event flag
extern char display_line[4][11];          // LCD display buffer
extern unsigned int straight_step;        // Straight motion step counter
extern unsigned int circle_step;          // Circle motion step counter
extern unsigned int circle_step2;         // Secondary circle step counter
extern unsigned int triangle_step;        // Triangle motion step counter
extern unsigned int figure8_step;         // Figure-8 motion step counter
extern char backlight_status;             // LCD backlight state
extern volatile unsigned int Time_Sequence; // System timing counter
extern int activateSM;                    // State machine activation flag
extern char baud_flag;                    // Baud rate configuration flag

/* ============================================================================
 * Macro Definitions
 * ============================================================================*/

#define DEBOUNCE_TIME          (6)    // Number of timer cycles for debounce (~50ms)
#define DEBOUNCE_RESTART       (0)    // Value to reset debounce counters

/* ============================================================================
 * Function Declarations
 * ============================================================================*/

// Project 8 Specific Handlers
void SW1_Project8(void);  // Handler for SW1 press in Project 8 context
void SW2_Project8(void);  // Handler for SW2 press in Project 8 context

// Switch Enable Functions
void enable_switch_SW1(void);  // Enables interrupts for SW1 only
void enable_switch_SW2(void);  // Enables interrupts for SW2 only
void enable_switches(void);    // Enables both SW1 and SW2 interrupts

// Switch Disable Functions
void disable_switch_SW1(void); // Disables interrupts for SW1 only
void disable_switch_SW2(void); // Disables interrupts for SW2 only
void disable_switches(void);   // Disables both SW1 and SW2 interrupts

// Debounce Functions
void debounce(void);     // General debounce handler for both switches
void debounceSW1(void);  // Specific debounce handler for SW1
void debounceSW2(void);  // Specific debounce handler for SW2

#endif /* SWITCHES_H_ */
