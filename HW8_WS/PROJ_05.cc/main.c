//------------------------------------------------------------------------------
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);

  // Global Variables
volatile char slow_input_down;
unsigned char display_mode;
unsigned int test_value;
char chosen_direction;
char change;
volatile int shape = 0;
int runtime = 0;
int roff = 0;
int loff = 0;
int wheeltime = 0;
int changed = 0;
int sectioner = 0;
int stopcheck = 0;
int stopcheck2 = 0;
volatile int finished = 1;

void main(void){
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
  strcpy(display_line[0], "  ECE306  ");
  strcpy(display_line[1], "  CORINNE ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);


//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------
event = NONE;
state = NONE;

while(ALWAYS) {                        // Can the Operating system run
    Carlson_StateMachine();            // Run a Time Based State Machine
    Display_Process();
    Switch_Process();

    if(sw1_position == PRESSED && finished == 1){
        shape = sw2_counter;
        sw1_position = RELEASED;
    }
    switch (event) {
    case STRAIGHT: // Straight
    Run_Straight(runtime, loff, roff, wheeltime);
    break; //
//    case CIRCLE: // Circle
//    Run_Circle(runtime, loff, roff, wheeltime);
//    break; //
    default: break;
    }

    if (finished == 1) {
    switch (shape) {
    case 1: //circle
        changed = 1;
        Display_Change(changed);
        shape = 0;
        finished = 0;
        event = STRAIGHT;
        state = WAIT;
        runtime = 54;
        roff = 2;
        loff = 10;
        wheeltime = 12;
        sw1_position = RELEASED;
        break;

     case 2: //figure 8
         if(sectioner == 0){
             if (sectioner == 0 && stopcheck == 1){
                 changed = 2;
                 Display_Change(changed);
             }
             finished = 0;
             event = STRAIGHT;
             state = WAIT;
             runtime = 20;
             roff = 1;
             loff = 19;
             wheeltime = 20;
             sectioner += 1;
             break;
         } else if (sectioner == 1){
             finished = 0;
             event = STRAIGHT;
             state = WAIT;
             runtime = 22;
             roff = 19;
             loff = 1;
             wheeltime = 22;
             sectioner += 1;
             if(sectioner == 2 && stopcheck == 0){
                 sectioner = 0;
                 stopcheck = 1;
                 break;
             }
             break;
         } else if (sectioner == 2){
             stopcheck = 0;
             sectioner = 0;
             shape = 0;
             event = NONE;
             state = NONE;
             sw1_position = RELEASED;
             break;
         }
         break;
     case 3: //triangle
         if(sectioner == 0 || sectioner == 2 || sectioner == 4){
             finished = 0;
             if (sectioner == 0){
             changed = 3;
             Display_Change(changed);
             }
             event = STRAIGHT;
             state = WAIT;
             runtime = 14;
             roff = 3;
             loff = 3;
             wheeltime = 9;
             sectioner += 1;
             break;
         } else if(sectioner == 1 || sectioner == 3 || sectioner == 5){
                 finished = 0;
                 event = STRAIGHT;
                 state = WAIT;
                 runtime = 12;
                 roff = 0;
                 loff = 9;
                 wheeltime = 10;
                 sectioner += 1;
                 if(sectioner == 6 && stopcheck == 0){
                     sectioner = 0;
                     stopcheck = 1;
                     break;
                 }
                 break;
         } else if (sectioner == 6 && stopcheck == 1){
             stopcheck = 0;
             sectioner = 0;
             shape = 0;
             event = NONE;
             state = NONE;
             sw1_position = RELEASED;
             break;
         }
         break;

     }
     }

    if(Last_Time_Sequence != Time_Sequence){
        Last_Time_Sequence = Time_Sequence;
        cycle_time++;
        time_change = 1;
    }

}
}
//------------------------------------------------------------------------------
