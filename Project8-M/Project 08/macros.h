/*
 * macros.h - System-wide Constants and Definitions
 *
 * Description: Contains all global macros used throughout the project
 *              - LED and switch definitions
 *              - System states and flags
 *              - Timing constants
 *
 * Created on: Jan, 2025
 * Author: Molana Torabi
 */

#ifndef MACROS_H_
#define MACROS_H_

// Basic Constants ============================================================
#define ALWAYS      (1)       // Constant true value for infinite loops
#define RESET_STATE (0)       // Default/reset value for state variables
#define TRUE       (0x01)     // Boolean true value
#define FALSE      (0x00)     // Boolean false value

// Hardware Definitions =======================================================
// LED Control
#define RED_LED    (0x01)     // P1.0 - Red LED pin mask
#define GRN_LED    (0x40)     // P6.6 - Green LED pin mask
#define TEST_PROBE (0x01)     // Test point/probe pin mask

// System States ==============================================================
#define MOVE       ('M')      // State: System in motion
#define STOP       ('S')      // State: System stopped

// Switch Handling ============================================================
#define PRESSED    (0)        // Switch physical pressed state
#define RELEASED   (1)        // Switch physical released state
#define NOT_OKAY   (0)        // Status: Operation not allowed
#define OKAY       (1)        // Status: Operation allowed
#define WAITING2START (500)   // Initialization delay count (ms)

// Power Control ==============================================================
#define OFF        ('0')      // Generic OFF state
#define ON         ('1')      // Generic ON state

// Port Configuration =========================================================
#define P4PUD      (P4OUT)    // Port 4 pull-up/down control alias
#define USE_GPIO   (0x00)     // Set pin to GPIO mode
#define USE_SMCLK  (0x01)     // Set pin to SMCLK output mode

// Timing Constants ===========================================================
#define HALF_SEC    (500)     // 500ms delay constant
#define COUNTER_RESET (0)     // Counter initialization value

// Deprecated/Commented Macros ================================================
//#define DEBOUNCE_TIME       (12)   // Old debounce duration
//#define DEBOUNCE_RESTART    (0)    // Old debounce reset value
//#define SW1 (0x02)                // Old switch 1 definition

#endif /* MACROS_H_ */
