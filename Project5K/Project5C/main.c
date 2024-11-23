//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

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
void Carlson_StateMachine(void);

  // Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned char backlight = 0;
extern volatile unsigned char backlight_changed = 0;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;
unsigned int wheel_move;
char forward;

extern unsigned int Last_Time_Sequence = 0;  // To track changes in Time_Sequence
extern unsigned int cycle_time = 0;          // Controls shape timings
extern unsigned int time_change = 0;         // Flag to detect time sequence change
extern volatile unsigned int Time_Sequence; // Already existing
volatile unsigned char event;  // Event variable from switches.c (e.g., STRAIGHT, CIRCLE)
extern volatile unsigned int event_Counter = 0; //picking the event based on increment
volatile unsigned int state;   // To manage the state machine

extern volatile unsigned int i;
extern volatile unsigned int instruction = 0;



void main(void){
    event = NONE;
    state = WAIT;
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings
  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
// Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "   NCSU   ");
  strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  ECE306  ");
  strcpy(display_line[3], "  GP I/O  ");
  display_changed = TRUE;
  Display_Update(0,0,0,0);
  display_changed = FALSE;

  wheel_move = 0;
  forward = TRUE;

  backlight_changed = TRUE;
  backlight = TRUE;
  Backlight_Process();               // Turn Backlight on/off


//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run

      Switches_Process();                // Check for switch state change
//      Carlson_StateMachine();            // Run a Time Based State Machine
      Display_Process();                 // Update Display
      P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

      // Time Sequence Handling
      if(Last_Time_Sequence != Time_Sequence){
          Last_Time_Sequence = Time_Sequence;
          cycle_time++;
          time_change = 1;            // Flag to indicate a time change occurred
      }

      Wheel_Move();
      motorDirec();
//      if(!instruction){
//      Display_Changing();
//      }

      }
      // This will make it move


//------------------------------------------------------------------------------

}


void Carlson_StateMachine(void){
    switch(Time_Sequence){
      case 250:                        //
        if(one_time){
          Init_LEDs();
          lcd_BIG_mid();
          display_changed = 1;
          one_time = 0;
        }
        Time_Sequence = 0;             //
        break;
      case 200:                        //
        if(one_time){
//          P1OUT &= ~RED_LED;            // Change State of LED 4
          P6OUT |= GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 150:                         //
        if(one_time){
          P1OUT |= RED_LED;            // Change State of LED 4
          P6OUT &= ~GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 100:                         //
        if(one_time){
//          lcd_4line();
          lcd_BIG_bot();
          P6OUT |= GRN_LED;            // Change State of LED 5
          display_changed = 1;
          one_time = 0;
        }
        break;
      case  50:                        //
        if(one_time){
          one_time = 0;
        }
        break;                         //
      default: break;
    }


}





