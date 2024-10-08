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
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "strings.h"

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Init_Timer_B0(void);
void Carlson_StateMachine(void);
void Wheels_Stop(void);
void Wheels_Forward(void);
void Wheels_Reverse(void);
void Wheels_clkw(void);
void Wheels_counterclkw(void);
void Run_Straight(void);
void Run_Circle(void);
void Run_Figure_Eight(void);
void Run_Triangle(void);
  // Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern unsigned int test_value;
extern unsigned int tmr_ms_count;
char chosen_direction;
char change;
unsigned int wheel_move;
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;
extern unsigned int cir_count;
extern unsigned int tri_count;
extern unsigned int okay_to_look_at_switch1;
extern unsigned int sw1_position;
char forward;


unsigned int event;
unsigned int state;


//void main(void){
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
  Init_Timer_B0();
  Wheels_Stop();
//  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
//P2OUT &= ~RESET_LCD;
  // Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "   NCSU   ");
  strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  ECE306  ");
  strcpy(display_line[3], "  GP I/O  ");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);

  wheel_move = 0;
  forward = TRUE;
  //Wheels_Forward();
  //time_change = 1;
  cir_count = 0;
  tri_count = 0;
  state = WAIT;
  event = REVERSE1;
  okay_to_look_at_switch1 = 1;
  sw1_position = 1;
//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
//    Carlson_StateMachine();            // Run a Time Based State Machine
    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
    /*if(Last_Time_Sequence != Time_Sequence){
        Last_Time_Sequence = Time_Sequence;
        cycle_time++;
        time_change = 1;
     }*/
    switch(tmr_ms_count){
         case ONESEC: // 1st second
             strcpy(display_line[0], "          ");
             strcpy(display_line[1], " FORWARDS ");
             strcpy(display_line[2], "ONE SECOND");
             strcpy(display_line[3], "          ");
             display_changed = TRUE;
             Wheels_Stop();
             Wheels_Forward();
             break; //
         case 2*ONESEC: //Figure 8
             strcpy(display_line[0], "          ");
             strcpy(display_line[1], "  PAUSE   ");
             strcpy(display_line[2], "ONE SECOND");
             strcpy(display_line[3], "          ");
             display_changed = TRUE;
             Wheels_Stop();
             break;
         case 3*ONESEC: // Triangle
             strcpy(display_line[0], "          ");
             strcpy(display_line[1], "  REVERSE ");
             strcpy(display_line[2], "TWOSECONDS");
             strcpy(display_line[3], "          ");
             display_changed = TRUE;
             Wheels_Stop();
             Wheels_Reverse();
             break;
         case 5*ONESEC: // Straight
             strcpy(display_line[0], "          ");
             strcpy(display_line[1], "  PAUSE   ");
             strcpy(display_line[2], "ONE SECOND");
             strcpy(display_line[3], "          ");
             display_changed = TRUE;
             Wheels_Stop();
             break; //
         case 6*ONESEC:
             strcpy(display_line[0], "          ");
             strcpy(display_line[1], " FORWARDS ");
             strcpy(display_line[2], "ONE SECOND");
             strcpy(display_line[3], "          ");
             display_changed = TRUE;
             Wheels_Stop();
             Wheels_Forward();
             break;
         case 7*ONESEC:
             strcpy(display_line[0], "          ");
             strcpy(display_line[1], "  PAUSE   ");
             strcpy(display_line[2], "ONE SECOND");
             strcpy(display_line[3], "          ");
             display_changed = TRUE;
             Wheels_Stop();
             break;
         case 8*ONESEC:
             strcpy(display_line[0], "          ");
             strcpy(display_line[1], " CLOCKWISE");
             strcpy(display_line[2], " THREESECS");
             strcpy(display_line[3], "          ");
             display_changed = TRUE;
             Wheels_Stop();
             Wheels_clkw();
             break;
         case 10*ONESEC:
             strcpy(display_line[0], "          ");
             strcpy(display_line[1], "  PAUSE   ");
             strcpy(display_line[2], "TWOSECONDS");
             strcpy(display_line[3], "          ");
             display_changed = TRUE;
             Wheels_Stop();
             break;
         case 12*ONESEC:
             strcpy(display_line[0], "          ");
             strcpy(display_line[1], " CCLKWISE ");
             strcpy(display_line[2], " THREESECS");
             strcpy(display_line[3], "          ");
             display_changed = TRUE;
             Wheels_Stop();
             Wheels_counterclkw();
             break;
         case 14*ONESEC:
             strcpy(display_line[0], "          ");
             strcpy(display_line[1], "  PAUSE   ");
             strcpy(display_line[2], "TWOSECONDS");
             strcpy(display_line[3], "          ");
             display_changed = TRUE;
             Wheels_Stop();
             break;
         default: break;
     }


  }
//------------------------------------------------------------------------------

}





