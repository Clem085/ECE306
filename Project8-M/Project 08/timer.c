/* ============================================================================
 * Timer Control Module
 *
 * File        : timers.c
 * Description : Initializes and manages Timer B peripherals
 *               - Configures Timer B0 for system timing and interrupts
 *               - Sets up Timer B3 for PWM generation (motor control and backlight)
 *               - Provides precise timing for system operations
 *
 * Author      : Molana Torabi
 * Created     : January 2025
 * ============================================================================
 */

// System includes
#include "msp430.h"
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"
#include "motors.h"
#include "Display.h"
#include "timers.h"
#include "interrupts.h"
#include "switches.h"
#include "ADC.h"
#include "IR.h"
#include "serial.h"
#include "menu.h"
#include <string.h>
#include <stdio.h>

/* ============================================================================
 * Global Variables
 * ============================================================================*/

extern volatile unsigned int Time_Sequence;  // System time counter
volatile unsigned int LCD_Update;           // LCD refresh timer
volatile unsigned int pause_counter;        // General pause counter
volatile unsigned int pause_counter1;       // Secondary pause counter

/* ============================================================================
 * Function Implementations
 * ============================================================================*/

/**
 * Initializes all system timers
 * Calls individual timer initialization functions
 */
void Init_Timers(void) {
    Init_Timer_B0();  // Initialize system timer
    Init_Timer_B3();  // Initialize PWM timer
}

/**
 * Initializes Timer B0 for system timing
 * Configuration:
 * - SMCLK clock source (typically 1MHz)
 * - Continuous up-counting mode
 * - Clock divider: 2 (primary) + 8 (extended) = 16 total
 * - CCR0 interrupt enabled for periodic events
 */
void Init_Timer_B0(void) {
    // Configure timer control register
    TB0CTL = TBSSEL__SMCLK;   // Select SMCLK as clock source
    TB0CTL |= TBCLR;          // Reset timer and clear settings
    TB0CTL |= MC__CONTINOUS;  // Continuous counting mode
    TB0CTL |= ID__2;          // Input divider: divide by 2

    // Configure extended timer control
    TB0EX0 = TBIDEX__8;       // Additional divider: divide by 8

    // Configure Capture/Compare Register 0
    TB0CCR0 = TB0CCR0_INTERVAL; // Set interrupt interval (from macros.h)
    TB0CCTL0 |= CCIE;          // Enable CCR0 interrupt

    // Configure overflow settings
    TB0CTL &= ~TBIE;          // Disable timer overflow interrupt
    TB0CTL &= ~TBIFG;         // Clear overflow interrupt flag
}

/**
 * Initializes Timer B3 for PWM generation
 * Configuration:
 * - SMCLK clock source
 * - Up-counting mode
 * - 5 PWM channels with reset/set output mode
 * - Channels control:
 *   - CCR1: LCD backlight brightness
 *   - CCR2: Right motor forward speed
 *   - CCR3: Left motor forward speed
 *   - CCR4: Right motor reverse speed
 *   - CCR5: Left motor reverse speed
 */
void Init_Timer_B3(void) {
    // Configure timer control register
    TB3CTL = TBSSEL__SMCLK;  // Select SMCLK as clock source
    TB3CTL |= MC__UP;        // Up-counting mode
    TB3CTL |= TBCLR;         // Reset timer and clear settings

    // Set PWM period (from macros.h)
    PWM_PERIOD = WHEEL_PERIOD; // Typically 50005 counts

    // Configure PWM channels (reset/set mode)
    TB3CCTL1 = OUTMOD_7;     // CCR1: LCD backlight control
    LCD_BACKLITE_DIMING = PERCENT_80; // Default brightness (80%)

    TB3CCTL2 = OUTMOD_7;     // CCR2: Right motor forward
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // Initialize stopped

    TB3CCTL3 = OUTMOD_7;     // CCR3: Left motor forward
    LEFT_FORWARD_SPEED = WHEEL_OFF; // Initialize stopped

    TB3CCTL4 = OUTMOD_7;     // CCR4: Right motor reverse
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // Initialize stopped

    TB3CCTL5 = OUTMOD_7;     // CCR5: Left motor reverse
    LEFT_REVERSE_SPEED = WHEEL_OFF; // Initialize stopped
}
