//------------------------------------------------------------------------------
// Description: This file contains the Main Routine - "While" Operating System
// Author: Jim Carlson
// Jan 2023
// Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "switches.h"
#include "wheels.h"
#include  <stdio.h>

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_Project07_SM(void);
void Init_DAC(void);

// Global Variables
volatile char slow_input_down; // Flag for slow input detection
extern char display_line[4][11]; // Buffer for LCD display lines
extern char *display[4]; // Pointers to display lines
unsigned char display_mode; // Current display mode
extern volatile unsigned char display_changed; // Flag to indicate display update
extern volatile unsigned char update_display; // Flag to update the display
extern volatile unsigned int update_display_count; // Counter for display updates
extern volatile unsigned char backlight = 0; // Backlight status
extern volatile unsigned char backlight_changed = 0; // Flag for backlight change
extern volatile unsigned int Time_Sequence; // Time sequence counter
extern volatile char one_time; // Flag for one-time operations
unsigned int test_value; // Test value for debugging
char chosen_direction; // Selected movement direction
char change; // Flag for change detection
unsigned int wheel_move; // Counter for wheel movement
char forward; // Flag for forward movement
extern volatile unsigned char state; // Current system state

extern unsigned int Last_Time_Sequence = 0;  // To track changes in Time_Sequence
extern unsigned int cycle_time = 0;          // Controls shape timings
extern unsigned int time_change = 0;         // Flag to detect time sequence change
volatile unsigned char event;  // Event variable from switches.c (e.g., STRAIGHT, CIRCLE)
extern volatile unsigned int event_Counter = 0; // Counter for picking the event based on increment

extern volatile unsigned int i; // General-purpose counter
extern volatile unsigned int ADC_Left_Detect; // Left sensor detection value
extern volatile unsigned int ADC_Right_Detect; // Right sensor detection value

extern unsigned char dec; // Thousands place for display
extern unsigned char one; // Ones place for display
extern unsigned char ten; // Tens place for display
extern unsigned char hun; // Hundreds place for display
extern volatile unsigned int pressed; // Flag for button press
extern volatile unsigned int allow; // Flag to allow certain operations

// Main Function
void main(void){
    // Disable the watchdog timer
    // WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //------------------------------------------------------------------------------
    // Main Program
    // This is the main routine for the program. Execution of code starts here.
    // The operating system is Back Ground Fore Ground.
    //------------------------------------------------------------------------------
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
    Init_Ports();          // Initialize Ports
    Init_Clocks();         // Initialize Clock System
    Init_Conditions();     // Initialize Variables and Initial Conditions
    Init_Timers();         // Initialize Timers
    Init_LCD();            // Initialize LCD
    Init_ADC();            // Initialize ADC

    display_changed = TRUE; // Set flag to update display

    wheel_move = 0; // Initialize wheel movement counter
    forward = TRUE; // Set initial movement direction to forward

    P2OUT |= IR_LED; // Turn on IR LED

    //------------------------------------------------------------------------------
    // Beginning of the "While" Operating System
    //------------------------------------------------------------------------------
    while(ALWAYS) { // Infinite loop for the operating system

        // Update Display
        Display_Process();
        Backlight_Process(); // Handle backlight control
        P3OUT ^= TEST_PROBE; // Toggle TEST_PROBE state

        // Time Sequence Handling
        if(Last_Time_Sequence != Time_Sequence){
            Last_Time_Sequence = Time_Sequence; // Update last time sequence
            cycle_time++; // Increment cycle time
            time_change = 1; // Set flag to indicate a time change occurred
        }

        // Motor and State Machine Logic
        SaveMeFets(); // Handle motor direction logic
        Project07_SM(); // Run the state machine

        Debounce_State(); // Handle switch debouncing

        // Update ADC values on the display if allowed
        if(allow){
            HexToBCD(ADC_Left_Detect); // Convert left sensor value to BCD
            adc_line(2,2); // Display left sensor value on LCD
            HexToBCD(ADC_Right_Detect); // Convert right sensor value to BCD
            adc_line(3,3); // Display right sensor value on LCD
        }
    }
}

// Function to display ADC values (currently empty)
void ADC_Display(void){
    // Placeholder for ADC display logic
}

// State Machine Function
void Project07_SM(void){
    switch(state){
        case WAIT:
            Off_Case(); // Stop all motors
            state = START; // Transition to START state
            break;
        case START:
            start_movement(); // Start movement toward the line
            break;
        case DETECTED:
            detect_movement(); // Handle detection of the black line
            break;
        case SPIN:
            spinning_movement(); // Handle spinning movement
            break;
        case TRACKWAIT:
            track_wait(); // Wait during tracking
            break;
        case TRACK:
            tracking_movement(); // Handle tracking movement
            break;
        case TURNSTOP:
            turn_stop(); // Stop during a turn
            break;
        case TURNN:
            turning(); // Handle turning movement
            break;
        case STRAIGHTSTOP:
            straightstop(); // Stop during straight movement
            break;
        case STRAIGHTL:
            straightline(); // Move straight
            break;
        case END:
            end_state(); // Handle the end state
            break;
        default: break; // Default case (do nothing)
    }
}

// Function to display time on the LCD
void Displaytime(void){
    char temp[11]; // Temporary buffer to format the string
    // Format the string manually
    temp[0] = ' ';
    temp[1] = ' ';
    temp[2] = ' ';
    temp[3] = dec; // Thousands place
    temp[4] = hun; // Hundreds place
    temp[5] = ten; // Tens place
    temp[6] = '.'; // Decimal point
    temp[7] = one; // Ones place
    temp[8] = ' ';
    temp[9] = ' ';
    temp[10] = '\0'; // Null terminator

    // Copy the formatted string into the display line
    strcpy(display_line[3], temp);
    display_changed = TRUE; // Set flag to update display
}
