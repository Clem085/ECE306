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

// Connor's Custom Functions
void backlightControl(int action);
void LRmotorForward(void);
void LRmotorStop(void);


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
    strcpy(display_line[1], "  Circle  ");
    strcpy(display_line[2], "  ECE306  ");
    strcpy(display_line[3], "  GP I/O  ");
    display_changed = TRUE;

    //  Display_Update(0,0,0,0);


    //------------------------------------------------------------------------------
    // Beginning of the "While" Operating System
    //------------------------------------------------------------------------------
    //    LRmotorStop();

//    //    Circle Code
//    wait();
//    circle();
//    LRmotorStop();
//
//
//    //    Triangle Code
//    wait();
//    triangle();
//
//
//    // Figure8
//    wait();
//    figure8();

    while(ALWAYS) {                      // Can the Operating system run
        backlightControl(1);
//        Carlson_StateMachine();            // Run a Time Based State Machine
        //    Switches_Process();                // Check for switch state change
        Display_Process();                 // Update Display
        P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

    }



    //------------------------------------------------------------------------------



}




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
        LRmotorStop();
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


// Wait Code
void wait(void){
    backlightControl(0);
    int mainCount = 0;
    int mainCount2 = 0;
    while(mainCount2 < 32000){
        mainCount = 0;
        while(mainCount < 500){
            mainCount++;
        }
        mainCount2++;
    }
    backlightControl(1);
}


