/* ============================================================================
 * Switch Handling Module
 *
 * File        : switches.c
 * Description : Implements switch debouncing and interrupt handling
 *               - Manages two switches (SW1 and SW2) with hardware debouncing
 *               - Uses Timer B0 for precise debounce timing
 *               - Provides clean switch press detection
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

// Switch Control Flags
volatile unsigned char backlight_on = 1;    // Backlight state (1=on, 0=off)
volatile unsigned char debounce_active = 0; // Global debounce status flag

// Debounce Timers
volatile unsigned int debounce_count1;      // SW1 debounce counter
volatile unsigned int debounce_count2;      // SW2 debounce counter
volatile char switch_debounce_active = 0;   // Bitmask for active debounce

// Switch State Flags
volatile int switch1_pressed;              // SW1 pressed state (1=pressed)
volatile int switch2_pressed;              // SW2 pressed state (1=pressed)

// System References
extern volatile unsigned int localCounter;  // System tick counter

// Timing Constants
#define increment 3277;                    // Debounce timer interval (~100ms)

/* ============================================================================
 * Function Implementations
 * ============================================================================*/

/**
 * Disables switch interrupts
 * Prevents switch interrupts during critical operations
 */
void disable_switch_interrupts(void) {
    P4IE &= ~SW1;  // Disable SW1 (Port 4) interrupt
    P2IE &= ~SW2;  // Disable SW2 (Port 2) interrupt
}

/**
 * Enables switch interrupts
 * Restores normal switch operation after critical sections
 */
void enable_switch_interrupts(void) {
    P4IE |= SW1;   // Enable SW1 (Port 4) interrupt
    P2IE |= SW2;   // Enable SW2 (Port 2) interrupt
}

/**
 * SW1 Interrupt Handler (Port 4)
 * Triggered on SW1 press, initiates debounce sequence
 */
#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
    if ((P4IFG & SW1) && (switch_debounce_active == 0)) {
        switch1_pressed = 1;            // Set SW1 pressed flag
        localCounter++;                 // Increment system counter

        // Initialize debounce sequence
        debounce_count1 = 0;            // Reset debounce counter
        switch_debounce_active = 1;     // Set debounce active flag

        // Configure debounce timer
        TB0CCR1 = TB0R + increment;     // Set debounce interval (~100ms)
        TB0CCTL1 |= CCIE;               // Enable compare interrupt

        P4IFG &= ~SW1;                  // Clear interrupt flag
        disable_switch_interrupts();     // Prevent additional interrupts
    }
}

/**
 * SW2 Interrupt Handler (Port 2)
 * Triggered on SW2 press, initiates debounce sequence
 */
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void) {
    if ((P2IFG & SW2) && (switch_debounce_active == 0)) {
        switch2_pressed = 1;            // Set SW2 pressed flag
        localCounter++;                 // Increment system counter

        // Initialize debounce sequence
        debounce_count2 = 0;            // Reset debounce counter
        switch_debounce_active = 1;     // Set debounce active flag

        // Configure debounce timer
        TB0CCR2 = TB0R + increment;     // Set debounce interval (~100ms)
        TB0CCTL2 |= CCIE;               // Enable compare interrupt

        P2IFG &= ~SW2;                  // Clear interrupt flag
    }
}

/**
 * Timer B0 CCR1/CCR2 Interrupt Handler
 * Handles debounce timing for both switches
 */
#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR(void) {
    switch (TB0IV) {
        case TB0IV_TBCCR1:  // SW1 debounce complete
            debounce_count1++;
            if (debounce_count1 >= 12) {  // ~1.2 second debounce period
                switch1_pressed = 0;      // Clear pressed flag
                switch_debounce_active = 0; // End debounce
                TB0CCTL1 &= ~CCIE;       // Disable interrupt
                TB0CCTL0 |= CCIE;        // Restore normal timer operation
            }
            break;

        case TB0IV_TBCCR2:  // SW2 debounce complete
            debounce_count2++;
            if (debounce_count2 >= 12) {  // ~1.2 second debounce period
                switch2_pressed = 0;      // Clear pressed flag
                switch_debounce_active = 0; // End debounce
                TB0CCTL2 &= ~CCIE;       // Disable interrupt
                TB0CCTL0 |= CCIE;        // Restore normal timer operation
            }
            break;

        default:
            break;
    }
}
