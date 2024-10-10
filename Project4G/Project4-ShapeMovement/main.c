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
unsigned int backlight_status;
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;
unsigned char dispEvent;
unsigned char state;
unsigned char event;

unsigned int travel_distance;
unsigned int right_count_time;
unsigned int left_count_time;
unsigned int wheel_count_time;

unsigned int time_change;
unsigned int delay_start;
unsigned int cycle_time;
unsigned int right_motor_count;
unsigned int left_motor_count;
unsigned int segment_count;

unsigned int straight_step;
unsigned int circle_step;
unsigned int circle_step2;
unsigned int triangle_step;
unsigned int figure8_step;


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
    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "Press  SW1");
    strcpy(display_line[2], " To Begin ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;
    //  Display_Update(0,0,0,0);


    //------------------------------------------------------------------------------
    // Beginning of the "While" Operating System
    //------------------------------------------------------------------------------
    backlight_status = 0;
    dispEvent = NONE;
    state = WAIT;
    LRmotorStop();
    event = NONE;
    straight_step = 0;

    while(ALWAYS) {                      // Can the Operating system run
        Carlson_StateMachine();            // Run a Time Based State Machine
        Display_Process();                 // Update Display
        P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
        backlight_control();
        if(Last_Time_Sequence != Time_Sequence){
            Last_Time_Sequence = Time_Sequence;
            cycle_time++;
            time_change = 1;
        }

        Switch1_Process();
        Switch2_Process();

        Move_Shape(); //Nothing will happen UNTIL proper buttons are pressed, stored in motors.c
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





