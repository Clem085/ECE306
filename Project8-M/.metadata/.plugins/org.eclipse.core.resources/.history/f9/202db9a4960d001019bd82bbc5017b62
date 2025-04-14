// File Name : switches.c
//
// Description: This file contains the code for handling switch presses and
// implementing debouncing mechanisms.
//
// Created on: Jan, 2025
// Author: Molana Torabi

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "switches.h"
#include <stdio.h>
#include "timersB0.h"

// Global Variables
extern volatile unsigned int Time_Sequence; // Tracks time sequence for system operations
extern volatile unsigned int switchpressed = OFF; // Flag to indicate if a switch is pressed
extern volatile unsigned int count_debounce_SW1 = OFF; // Counter for debouncing SW1
extern volatile unsigned int count_debounce_SW2 = OFF; // Counter for debouncing SW2
extern volatile unsigned int debounce_statesw1; // State of SW1 debounce process
extern volatile unsigned int debounce_statesw2; // State of SW2 debounce process
extern volatile unsigned char display_changed; // Flag to indicate if the display needs updating
extern char display_line[4][11]; // Buffer for LCD display lines
extern char *display[4]; // Pointers to display lines
extern volatile unsigned char state; // Current system state
volatile unsigned int pressed = 0; // Counter for button presses

extern volatile unsigned int ADC_Left_Detect; // Left sensor detection value
extern volatile unsigned int ADC_Right_Detect; // Right sensor detection value

extern int Left; // Left sensor calibration value
extern int Right; // Right sensor calibration value
volatile unsigned int allow = 0; // Flag to allow certain operations

// ENABLE SWITCHES

// Enable SW1 (Switch 1)
void enable_switch_SW1(void){
    P5OUT |=  SW1; // Set SW1 pin as input with pull-up resistor
    P5DIR &= ~SW1; // Configure SW1 pin as input
}

// Enable SW2 (Switch 2)
void enable_switch_SW2(void){
    P5OUT |=  SW2; // Set SW2 pin as input with pull-up resistor
    P5DIR &= ~SW2; // Configure SW2 pin as input
}

// Enable both switches (SW1 and SW2)
void enable_switches(void){
    enable_switch_SW1();
    enable_switch_SW2();
}

// DISABLE SWITCHES

// Disable SW1 (Switch 1)
void disable_switch_SW1(void){
    P5OUT |=  SW1; // Set SW1 pin as input with pull-up resistor
    P5DIR &= ~SW1; // Configure SW1 pin as input
}

// Disable SW2 (Switch 2)
void disable_switch_SW2(void){
    P5OUT |=  SW2; // Set SW2 pin as input with pull-up resistor
    P5DIR &= ~SW2; // Configure SW2 pin as input
}

// Disable both switches (SW1 and SW2)
void disable_switches(void){
    disable_switch_SW1();
    disable_switch_SW2();
}

// Interrupt Service Routine for Port 4 (SW1)
#pragma vector = PORT4_VECTOR
__interrupt void switchP1_interrupt(void) {
    // Check if SW1 triggered the interrupt
    if (P4IFG & SW1) {
        P4IE &= ~SW1; // Disable Port 4 interrupt for SW1
        P4IFG &= ~SW1; // Clear SW1 interrupt flag

        // Configure Timer B0 for debouncing
        TB0CCTL1 &= ~CCIFG; // Clear SW1 debounce interrupt flag
        TB0CCR1 = TB0CCR1_INTERVAL; // Set debounce interval
        TB0CCTL1 |= CCIE; // Enable Timer B0 CCR1 interrupt

        // SW1 FUNCTIONS:
        switchpressed = ON; // Set switch pressed flag
        allow = 1; // Allow certain operations
        state = WAIT; // Set system state to WAIT
    }
}

// Interrupt Service Routine for Port 2 (SW2)
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
    // Check if SW2 triggered the interrupt
    if (P2IFG & SW2) {
        P2IFG &= ~SW2; // Clear SW2 interrupt flag
        pressed = pressed + 1; // Increment button press counter

        // Handle different press counts
        if (pressed == 1) {
            strcpy(display_line[0], "BLACK CAL "); // Display calibration message
            HexToBCD(Left); // Convert left sensor value to BCD
            adc_line(2, 2); // Display left sensor value on LCD
            HexToBCD(Right); // Convert right sensor value to BCD
            adc_line(3, 3); // Display right sensor value on LCD
        }
        if (pressed == 2) {
            strcpy(display_line[0], " WHITE DET"); // Display white detection message
            HexToBCD(Left + 10); // Adjust and convert left sensor value to BCD
            adc_line(2, 2); // Display adjusted left sensor value on LCD
            HexToBCD(Right + 20); // Adjust and convert right sensor value to BCD
            adc_line(3, 3); // Display adjusted right sensor value on LCD
        }

        disable_switches(); // Disable switches after handling press
        display_changed = TRUE; // Flag to update the display
    }
}

// Function to handle debounce states for switches
void Debounce_State(void){
    // Check if SW1 is debouncing
    if (debounce_statesw1) {
        if (count_debounce_SW1 >= DEBOUNCE_TIME) {
            debounce_statesw1 = OFF; // End debounce state for SW1
            enable_switches(); // Re-enable switches
        }
    }
    // Check if SW2 is debouncing
    if (debounce_statesw2) {
        if (count_debounce_SW2 >= DEBOUNCE_TIME) {
            debounce_statesw2 = OFF; // End debounce state for SW2
            enable_switches(); // Re-enable switches
        }
    }
}
