/* Main Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : main.c
  Description:  This file contains the Main Routine - "While" Operating System
  Programmer: Connor Savugot
  Date Created: Oct 14, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
*/

// #include as of 11-27-24
//// Header Files
#include "ADC.h"
#include "DAC.h"
#include "Display.h"
#include "functions.h"
#include "interrupts.h"
#include "IR.h"
#include "IOT.h"
#include "LCD.h"
#include "macros.h"
#include "menu.h"
#include "motors.h"
#include "msp430.h"
#include "ports.h"
#include "serial.h"
#include "switches.h"
#include "timers.h"
//// Libraries
#include <stdio.h>
#include <string.h>



// Global Variables
//// Internally Defined
char slow_input_down;
char forward;
char command;
unsigned int wheel_move;
unsigned int state;
unsigned int clear_display;
unsigned int iot_start_count;
char process_buffer[25];

//// Externally Defined
extern char display_line[4][11];
extern unsigned char display_changed;
extern unsigned int display_clear_flag;
extern unsigned int sw1_okay;
extern unsigned int sw1_position;
extern unsigned char update_display;
extern char transmit_state;  // Corrected and added

void main(void){
    //    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //------------------------------------------------------------------------------
    // Main Program
    // This is the main routine for the program. Execution of code starts here.
    // The operating system is Back Ground Fore Ground.
    //------------------------------------------------------------------------------
    PM5CTL0 &= ~LOCKLPM5;
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    Init_Ports();                        // Initialize Ports
    Init_Clocks();                       // Initialize Clock System
    Init_Conditions();                   // Initialize Variables and Initial Conditions
    Init_Timers();                       // Initialize Timers
    Init_LCD();                          // Initialize LCD
    Init_ADC();                          // Initialize ADC
    Init_Serial_UCA0();                  // Initialize UCA0 Serial Port
    Init_Serial_UCA1();                  // Initialize UCA1 Serial Port
    Init_DAC();                          // Initialize DAC

    Clear_Display();
    USCI_A0_transmit();
    transmit_state = WAIT;
    motors_off();
    wheel_move = 0;
    forward = TRUE;
    sw1_okay = 1;
    sw1_position = 1;
    iot_on_time = 0;
    command = WAIT;
    state = WAIT; // For Black Line
    P2OUT |= IR_LED; // Turn on IR Led

    //------------------------------------------------------------------------------
    // Begining of the "While" Operating System
    //------------------------------------------------------------------------------
    while(ALWAYS){
        Display_Process();                 // Update Display
        // P3OUT ^= TEST_PROBE; // TEST_PROBE is no longer used (for previous homework)
        if(display_clear_flag == 1){
            Clear_Display();
        }
        vrfyDirection(); // Protects against Magic Smoke

        iot_process();
    } // End of while()
} // End of main()
//------------------------------------------------------------------------------

