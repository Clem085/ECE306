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
#include  "macros.h"

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
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;

// New Global Variables for Button Switch & Movement
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;

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
    strcpy(display_line[0], "   NCSU   ");
    strcpy(display_line[1], " WOLFPACK ");
    strcpy(display_line[2], "  ECE306  ");
    strcpy(display_line[3], "  GP I/O  ");
    display_changed = TRUE;
    //  Display_Update(0,0,0,0);


    //------------------------------------------------------------------------------
    // Beginning of the "While" Operating System
    //------------------------------------------------------------------------------
    //    LRmotorStop();
    while(ALWAYS) {                      // Can the Operating system run
        Carlson_StateMachine();            // Run a Time Based State Machine
        //    Switches_Process();                // Check for switch state change
        Display_Process();                 // Update Display
        P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF


        //        New Code
        //        if switch is pressed, go to switches
        if(Last_Time_Sequence != Time_Sequence){
            Last_Time_Sequence = Time_Sequence;
            cycle_time++;
            time_change = 1;
        }

        switch(event){
        case  STRAIGHT:                       // Straight
            Run_Straight();
            break;

        case  CIRCLE:                         // Circle
            Run_Circle();
            break;

        case TRIANGLE:
            Run_Triangle();
            break;

        case FIGURE8:
            Run_FIGURE8();
            break;

        default: break;
        }

    }



    //------------------------------------------------------------------------------



}








void Run_Straight(void){
    switch(state){
    case  WAIT:
        wait_case();
        // Begin
        break;
    case  START:
        start_case();
        break;
    case  RUN:
        //        Run actual code to go straight

        break;
    case  END:
        end_case();
        break;
    default: break;
    }
}



void Run_Circle(void){
    switch(state){
    case  WAIT:
        wait_case();
        // Begin
        break;
    case  START:
        start_case();
        break;
    case  RUN:
        //        Run actual code to go straight
        circle();
        break;
    case  END:
        end_case();
        break;
    default: break;
    }
}


void Run_Triangle(void){
    switch(state){
    case  WAIT:
        wait_case();
        // Begin
        break;
    case  START:
        start_case();
        break;
    case  RUN:
        //        Run actual code to go straight
        triangle();
        break;
    case  END:
        end_case();
        break;
    default: break;
    }
}


void Run_Figure8(void){
    switch(state){
    case  WAIT:
        wait_case();
        // Begin
        break;
    case  START:
        start_case();
        break;
    case  RUN:
        //        Run actual code to go straight
        figure8();
        break;
    case  END:
        end_case();
        break;
    default: break;
    }
}


circle(void){
    // Circle Turn (Right)
    TRAVEL_DISTANCE = 2
            RIGHT_COUNT_TIME = 3
            LEFT_COUNT_TIME = 7
            WHEEL_COUNT_TIME = 10
            run_case();
}

triangle(void){
    for(i = 0; i < 3; i++){
        //      Triangle  Turn
        TRAVEL_DISTANCE = 2
                RIGHT_COUNT_TIME = 7
                LEFT_COUNT_TIME = 0
                WHEEL_COUNT_TIME = 10
                run_case();

        //      Triangle Straight
        TRAVEL_DISTANCE = 2
                RIGHT_COUNT_TIME = 7
                LEFT_COUNT_TIME = 0
                WHEEL_COUNT_TIME = 10
                run_case();
    }
}

void figure8(void){
    //    Right Circle
    TRAVEL_DISTANCE = 2
            RIGHT_COUNT_TIME = 7
            LEFT_COUNT_TIME = 5
            WHEEL_COUNT_TIME = 10
            run_case();




















    //CarlsonCode
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
                P1OUT |= RED_LED;            // Change State of LED 4
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
                // lcd_4line();
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





