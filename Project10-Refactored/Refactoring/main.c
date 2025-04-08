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
char command;
unsigned int state;
unsigned int clear_display;
unsigned int iot_start_count;

//// Externally Defined
extern unsigned int display_clear_flag;
extern unsigned int SW1_Okay;
extern unsigned int debounce_Status_SW1;
extern unsigned int SW2_Okay;
extern unsigned int debounce_Status_SW2;

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
    motors_off();
    SW1_Okay = 1;
    debounce_Status_SW1 = 1;
    SW2_Okay = 1;
    debounce_Status_SW2 = 1;
    iot_boot_time = 0;
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

