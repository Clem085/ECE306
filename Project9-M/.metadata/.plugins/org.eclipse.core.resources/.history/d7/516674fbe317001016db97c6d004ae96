/* ============================================================================
 * Timer Control Header
 *
 * File        : timers.h
 * Description : Defines timer configurations and interfaces
 *               - Declares timer-related variables and constants
 *               - Specifies timer interrupt vectors
 *               - Provides timer initialization prototypes
 *
 * Author      : Molana Torabi
 * Created     : January 2025
 * ============================================================================
 */

#ifndef TIMERSB0_H_
#define TIMERSB0_H_

/* ============================================================================
 * Global Variables
 * ============================================================================*/

// Internal Timer Variables
volatile char one_time;        // Flag for one-time timer events
unsigned int counter_B0;       // Timer B0 general purpose counter
unsigned int delay_time;       // Delay duration storage

// External System Variables
extern volatile unsigned int Time_Sequence;  // Main system time counter
extern char backlight_status;               // LCD backlight state
extern int Switch_Counter1;                 // Switch debounce counter
extern volatile unsigned char update_display; // Display refresh flag
extern int activateSM;                      // State machine activation flag
extern char display_line[4][11];            // LCD display buffer
extern volatile unsigned char display_changed; // Display content changed flag

// Debounce Control Variables
extern char debounce_Status_SW1;    // SW1 debounce state
extern char debounce_Status_SW2;    // SW2 debounce state
extern unsigned int count_debounce_SW1; // SW1 debounce counter
extern unsigned int count_debounce_SW2; // SW2 debounce counter

/* ============================================================================
 * Macro Definitions
 * ============================================================================*/

// Timer B0 Configuration
#define TB0CCR0_INTERVAL (25000)  // CCR0 interval for 200ms @ 8MHz/64
#define TB0CCR1_INTERVAL (25000)  // CCR1 interval (match CCR0)
#define TB0CCR2_INTERVAL (25000)  // CCR2 interval (match CCR0)

// Timer B3 Configuration
#define TB3CCR0_INTERVAL (5000)   // CCR0 interval for PWM generation

// Timer Interrupt Vectors
#define TIMER_B0_0_VECTOR (TIMER0_B0_VECTOR)    // CCR0 interrupt
#define TIMER_B0_1_OVFL_VECTOR (TIMER0_B1_VECTOR) // CCR1-2/overflow
#define TIMER_B3_0_VECTOR (TIMER0_B3_VECTOR)    // B3 CCR0 interrupt

/* ============================================================================
 * Function Declarations
 * ============================================================================*/

// ADC Functions (shared dependencies)
void Init_ADC(void);          // Initialize Analog-to-Digital Converter
void HexToBCD(int);           // Hexadecimal to Binary-Coded Decimal conversion

// Timer Initialization Functions
void Init_Timers(void);       // Initialize all system timers
void Init_Timer_B0(void);     // Configure Timer B0 for system timing
void Init_Timer_B1(void);     // Configure Timer B1 (reserved)
void Init_Timer_B2(void);     // Configure Timer B2 (reserved)
void Init_Timer_B3(void);     // Configure Timer B3 for PWM generation

// Utility Functions
void half_sec_delay(void);    // Creates 500ms delay using timer

#endif /* TIMERSB0_H_ */
