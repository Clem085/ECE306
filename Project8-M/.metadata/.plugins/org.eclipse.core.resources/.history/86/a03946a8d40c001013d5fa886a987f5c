/*
 * timersB0.c
 *
 * Created on: Jan, 2025
 * Author: Molana Torabi
 */

// Includes
#include "LCD.h"
#include "functions.h"
#include "macros.h"
#include "wheels.h"
#include "msp430.h"
#include "ports.h"
#include "switches.h"
#include <string.h>
#include "timersB0.h"
#include <stdio.h>

// Globals
volatile unsigned int Time_Sequence; // Tracks time sequence for system operations
volatile char one_time; // Flag for one-time operations
unsigned int counter_B0; // Counter for Timer B0
unsigned int delay_time; // Delay time for timing operations
extern unsigned int backlight_status; // Backlight status (external)
extern volatile unsigned char backlight_changed; // Flag for backlight change (external)
extern int Switch_Counter1; // Counter for switch presses (external)
extern volatile unsigned char update_display; // Flag to update the display (external)
extern volatile unsigned int backlight; // Backlight control (external)
extern volatile unsigned int debounce_statesw1 = 0; // Debounce state for SW1 (external)
extern volatile unsigned int debounce_statesw2 = 0; // Debounce state for SW2 (external)
extern volatile unsigned int count_debounce_SW1; // Debounce counter for SW1 (external)
extern volatile unsigned int count_debounce_SW2; // Debounce counter for SW2 (external)
unsigned int DimFlag = TRUE; // Flag for dimming the LCD backlight
unsigned int FlagSpinL; // Flag for left spin state
unsigned int FlagSpinR; // Flag for right spin state
unsigned int FlagWait; // Flag for wait state
unsigned int Blink_counter; // Counter for blinking operations

extern unsigned int SpincountL; // Counter for left spin duration (external)
extern unsigned int SpincountR; // Counter for right spin duration (external)
extern unsigned int WaitingCounter; // Counter for wait duration (external)

unsigned int Displaycount = 0; // Counter for display updates

extern char display_line[4][11]; // Buffer for LCD display lines (external)
extern char *display[4]; // Pointers to display lines (external)
extern volatile unsigned char display_changed; // Flag for display change (external)
extern volatile unsigned char update_display; // Flag to update the display (external)

unsigned char dec = ' '; // Thousands place for display
unsigned char one = ' '; // Ones place for display
unsigned char ten = ' '; // Tens place for display
unsigned char hun = ' '; // Hundreds place for display
unsigned int leaveFlag = 1; // Flag to control display updates

// Initialize all timers
void Init_Timers(void) {
    Init_Timer_B0(); // Initialize Timer B0
    Init_Timer_B1(); // Initialize Timer B1
    Init_Timer_B3(); // Initialize Timer B3
}

// Timer B0 initialization
void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK;     // Set Timer B0 clock source to SMCLK
    TB0CTL = TBSSEL__SMCLK;     // Set Timer B0 clock source to SMCLK (redundant)
    TB0CTL |= TBCLR;            // Reset Timer B0 counter and clock divider
    TB0CTL |= MC__CONTINOUS;    // Set Timer B0 to continuous up mode
    TB0CTL |= ID__8;            // Divide clock by 8
    TB0EX0 = TBIDEX__8;         // Divide clock by an additional 8
    TB0CCR0 = TB0CCR0_INTERVAL; // Set CCR0 interval
    TB0CCTL0 |= CCIE;           // Enable CCR0 interrupt
    // TB0CCR1 = TB0CCR1_INTERVAL; // Set CCR1 interval (commented out)
    // TB0CCTL1 |= CCIE; // Enable CCR1 interrupt (commented out)
    // TB0CCR2 = TB0CCR2_INTERVAL; // Set CCR2 interval (commented out)
    // TB0CCTL2 |= CCIE; // Enable CCR2 interrupt (commented out)
    TB0CTL &= ~TBIE;  // Disable Timer B0 overflow interrupt
    TB0CTL &= ~TBIFG; // Clear Timer B0 overflow interrupt flag
}

// Timer B1 initialization (identical to Timer B0 initialization)
void Init_Timer_B1(void) {
    TB0CTL = TBSSEL__SMCLK;     // Set Timer B0 clock source to SMCLK
    TB0CTL = TBSSEL__SMCLK;     // Set Timer B0 clock source to SMCLK (redundant)
    TB0CTL |= TBCLR;            // Reset Timer B0 counter and clock divider
    TB0CTL |= MC__CONTINOUS;    // Set Timer B0 to continuous up mode
    TB0CTL |= ID__8;            // Divide clock by 8
    TB0EX0 = TBIDEX__8;         // Divide clock by an additional 8
    TB0CCR0 = TB0CCR0_INTERVAL; // Set CCR0 interval
    TB0CCTL0 |= CCIE;           // Enable CCR0 interrupt
    // TB0CCR1 = TB0CCR1_INTERVAL; // Set CCR1 interval (commented out)
    // TB0CCTL1 |= CCIE; // Enable CCR1 interrupt (commented out)
    // TB0CCR2 = TB0CCR2_INTERVAL; // Set CCR2 interval (commented out)
    // TB0CCTL2 |= CCIE; // Enable CCR2 interrupt (commented out)
    TB0CTL &= ~TBIE;  // Disable Timer B0 overflow interrupt
    TB0CTL &= ~TBIFG; // Clear Timer B0 overflow interrupt flag
}

// Timer B3 initialization
void Init_Timer_B3(void) {
    //-----------------------------------------------------------------------------
    // SMCLK source, up count mode, PWM Right Side
    // TB3.1 P6.0 LCD_BACKLITE
    // TB3.2 P6.1 R_FORWARD
    // TB3.3 P6.2 R_REVERSE
    // TB3.4 P6.3 L_FORWARD
    // TB3.5 P6.4 L_REVERSE
    //-----------------------------------------------------------------------------
    TB3CTL = TBSSEL__SMCLK; // Set Timer B3 clock source to SMCLK
    TB3CTL |= MC__UP;       // Set Timer B3 to up mode
    TB3CTL |= TBCLR;        // Clear Timer B3 counter

    PWM_PERIOD = WHEEL_PERIOD;         // Set PWM period

    TB3CCTL1 = OUTMOD_7;               // Set CCR1 output mode to reset/set
    LCD_BACKLITE_DIMING = PERCENT_80;  // Set LCD backlight duty cycle to 80%

    TB3CCTL2 = OUTMOD_7;               // Set CCR2 output mode to reset/set
    RIGHT_FORWARD_SPEED = WHEEL_OFF;   // Set right forward wheel speed to off

    TB3CCTL3 = OUTMOD_7;               // Set CCR3 output mode to reset/set
    LEFT_FORWARD_SPEED = WHEEL_OFF;    // Set left forward wheel speed to off

    TB3CCTL4 = OUTMOD_7;               // Set CCR4 output mode to reset/set
    RIGHT_REVERSE_SPEED = WHEEL_OFF;   // Set right reverse wheel speed to off

    TB3CCTL5 = OUTMOD_7;               // Set CCR5 output mode to reset/set
    LEFT_REVERSE_SPEED = WHEEL_OFF;    // Set left reverse wheel speed to off
    //-----------------------------------------------------------------------------
}

// Timer B0 ISR (Interrupt Service Routine)
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    if(leaveFlag){
        Displaycount += 2.5; // Increment display counter
    }
    if(Displaycount >= 9998){
        Displaycount = 0; // Reset display counter if it exceeds 9998
    }
    timerdisplay(); // Update display with current time
    update_display = TRUE; // Set flag to update display

    Time_Sequence++; // Increment time sequence counter

    if(DimFlag == TRUE){
        if (Blink_counter++ >= 20){
            Blink_counter = 0;
            LCD_BACKLITE_DIMING = PERCENT_80; // Toggle LCD backlight brightness
        }
    }

    if(FlagSpinL == TRUE){
        SpincountL++; // Increment left spin counter
    }
    if(FlagSpinR == TRUE){
        SpincountR++; // Increment right spin counter
    }
    if(FlagWait == TRUE){
        WaitingCounter++; // Increment wait counter
    }
}

// Timer B1 ISR (Debounce Timer)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
    switch(__even_in_range(TB0IV,14)){
    case  0: break;                    // No interrupt
    case  2:                           // CCR1 Used for SW1 Debounce
        count_debounce_SW1++; // Increment SW1 debounce counter
        if (count_debounce_SW1 >= DEBOUNCE_TIME){
            count_debounce_SW1 = 0; // Reset SW1 debounce counter
            TB0CCTL1 &= ~CCIE; // Disable CCR1 interrupt
            P4IFG &= ~SW1; // Clear SW1 interrupt flag
            P4IE  |= SW1; // Re-enable SW1 interrupt
            TB0CCTL0 |= CCIE; // Enable CCR0 interrupt
        }
        break;

    case  4:                           // CCR2 Used for SW2 Debounce
        count_debounce_SW2++; // Increment SW2 debounce counter
        if (count_debounce_SW2 >= DEBOUNCE_TIME){
            count_debounce_SW2 = 0; // Reset SW2 debounce counter
            TB0CCTL2 &= ~CCIE; // Disable CCR2 interrupt
            P2IFG &= ~SW2; // Clear SW2 interrupt flag
            P2IE  |=  SW2; // Re-enable SW2 interrupt
            TB0CCTL0 |= CCIE; // Enable CCR0 interrupt
        }
        break;

    case 14:                           // Overflow available for greater than 1 second timer
        break;
    default: break;
    }
}

// Timer B3 ISR (Unused in this code)
#pragma vector = TIMER0_B3_VECTOR
__interrupt void Timer0_B3_ISR(void){
    TB0CCR2 += TB0CCR2_INTERVAL; // Increment CCR2 by interval
}

// Function to convert display counter to time and update display
void timerdisplay(void) {
    int x = Displaycount; // Get current display counter value
    int time_in_ms = x * 2; // Convert to milliseconds (each increment is 2 ms)

    // Separate into thousands, hundreds, tens, and ones
    dec = (time_in_ms / 1000) % 10 + '0'; // Thousands place (converted to ASCII)
    hun = (time_in_ms / 100) % 10 + '0';  // Hundreds place (converted to ASCII)
    ten = (time_in_ms / 10) % 10 + '0';   // Tens place (converted to ASCII)
    one = (time_in_ms % 10) + '0';        // Ones place (converted to ASCII)
    Displaytime(); // Update display with the calculated time
}
