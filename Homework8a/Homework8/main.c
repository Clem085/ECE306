//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// #include as of 11-4-24
// Header Files
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "motors.h"
#include  "Display.h"
#include  "timers.h"
#include  "switches.h"
#include  "ADC.h"
#include  "IR.h"
#include  "serial.h"

// Libraries
#include  <string.h>
#include  <stdio.h>

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void StateMachine(void);

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
extern char backlight_status;
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;
unsigned char dispEvent;

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

extern short int p3_4_type;
char state;

extern volatile unsigned int ADC_Channel;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
extern volatile unsigned int ADC_Thumb;
extern char IR_status;
extern char IR_changed;
//extern char ADC_Update = TRUE;;

int activateSM;
char state;
//volatile unsigned int State_Sequence;

void main(void)
{
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
    p3_4_type = USE_GPIO;
    Init_Ports();                        // Initialize Ports
    Init_Clocks();                       // Initialize Clock System
    Init_Conditions();            // Initialize Variables and Initial Conditions
    Init_Timers();                       // Initialize Timers
    Init_LCD();                          // Initialize LCD
    Init_ADC();                          // Initialize ADC
    Init_Serial();                       // Initialize Serial


    // Place the contents of what you want on the display, in between the quotes
    // Limited to 10 characters per line
    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    update_display = TRUE;
    display_changed = TRUE;
    //  Display_Update(0,0,0,0);

    //------------------------------------------------------------------------------
    // Beginning of the "While" Operating System
    //------------------------------------------------------------------------------
    //    dispEvent = NONE;
    //    state = WAIT;
    //    motorsOFF();
    //    event = NONE;
    //    straight_step = 0;
    //    activateSM = 0;

    IR_status = ON;
    backlight_status = OFF;
    state = IDLE;

    while (ALWAYS)
    {                      // Can the Operating system run
        P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
        updates();                // Checks all Update Flags

        vrfyDirection(); // Protects against Magic Smoke
    }

    //------------------------------------------------------------------------------

}

void updates(void)
{
    // Updates
    Display_Process();
    StateMachine();

    // Controls
    backlight_control();
    IR_control();
}

void StateMachine(void)
{
    switch (state)
    {
    case IDLE:
        if(!IR_changed){
            strcpy(display_line[0], "   IDLE   ");
        }

        PWM1_BOTH_OFF(); // motorsOFF();
        display_changed = TRUE;
        update_display = TRUE;
        //        ADC_Update = TRUE; = TRUE;
        // WAIT state called when SW1 is Pressed
        break;

    case WAIT:
        strcpy(display_line[0], "   WAIT   ");
        display_changed = TRUE;
        update_display = TRUE;
        //        ADC_Update = TRUE; = TRUE;
        switch (Time_Sequence)
        {   // Time_Sequence-State_Sequence
        case 10: // Wait for 1 Second
            PWM1_BOTH_OFF(); // motorsOFF();
            state = FWD;
            break;
        default:
            break;
        }
        break;

    case FWD:
        PWM1_BOTH_FWD(); // LRFwdON();
        strcpy(display_line[0], "   FWD    ");
        display_changed = TRUE;
        update_display = TRUE;
        //        ADC_Update = TRUE; = TRUE;
        if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM)){
            state = BLACKLINE; // Black Line Detected
            PWM1_BOTH_OFF(); // motorsOFF();
        }
        break;

    case BLACKLINE:
        PWM1_BOTH_OFF(); // motorsOFF();
        strcpy(display_line[0], " BLACKLINE");
        display_changed = TRUE;
        update_display = TRUE;
        //        ADC_Update = TRUE; = TRUE;
        Time_Sequence = 0; //        State_Sequence = Time_Sequence;
        state = WAIT2;
        break;

    case WAIT2:
        PWM1_BOTH_OFF(); // motorsOFF();
        if(Time_Sequence < 30){
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "Black Line");
            strcpy(display_line[2], " Detected ");
            strcpy(display_line[3], "          ");
        }
        display_changed = TRUE;
        update_display = TRUE;
        switch (Time_Sequence){ // Time_Sequence-State_Sequence
        case 30: // Wait for 3 Second
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "          ");
            strcpy(display_line[2], "          ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;
            update_display = TRUE;
            PWM1_BOTH_FWD();
            if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM)){
                Time_Sequence = 0;
                state = TURNL; // Black Line Detected
                PWM1_RIGHT_OFF(); // motorsOFF();
            }
            break;
        default:
            break;
        }

        break;

    case TURNL:
        PWM1_LEFT_FWD(); // LeftFwdON();
        strcpy(display_line[0], " TURN LEFT");
        display_changed = TRUE;
        update_display = TRUE;
        //        ADC_Update = TRUE; = TRUE;
        if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM))
        {
            state = LINE1; // Black Line Detected
            PWM1_BOTH_OFF(); // motorsOFF();
            Time_Sequence = 0;
        }
        break;
    case LINE1:
        strcpy(display_line[0], "   LINE1  ");
        display_changed = TRUE;
        update_display = TRUE;
        //        ADC_Update = TRUE; = TRUE;
        if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM)
                && (Time_Sequence >= 10))
        {
            state = DONE; // Black Line Detected
            Time_Sequence = 0; //  State_Sequence = Time_Sequence;
            PWM1_BOTH_OFF(); // motorsOFF();
        }
    case DONE:
        strcpy(display_line[0], "   DONE   ");
        display_changed = TRUE;
        update_display = TRUE;
        break;
//    case DONE:
//        LeftFwdON();
//        strcpy(display_line[0], "   LINE2  ");
//        display_changed = TRUE;
//        update_display = TRUE;
//        //        ADC_Update = TRUE; = TRUE;
//        if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM)
//                && (Time_Sequence >= 10))
//        {
//            Time_Sequence = 0; //  State_Sequence = Time_Sequence;
//            state = LINE1; // Black Line Detected
//            motorsOFF();
//        }

    default:
        break;
    }
}

// Project 5
//void StateMachine(void){
//    if(activateSM){
//        switch(Time_Sequence){
//        case 10:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], " Forward  ");
//            strcpy(display_line[2], "   1 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            LRFwdON();
//            break;
//        case 15:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "    Wait  ");
//            strcpy(display_line[2], "   1 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            LRFwdOFF();
//            motorsOFF();
//            break;
//        case 20:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], " Reverse  ");
//            strcpy(display_line[2], "   2 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            LRRevON();
//            break;
//        case 30:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "    Wait  ");
//            strcpy(display_line[2], "   1 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            LRRevOFF();
//            motorsOFF();
//            break;
//        case 35:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], " Forward  ");
//            strcpy(display_line[2], "   1 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            LRFwdON();
//            break;
//        case 40:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "    Wait  ");
//            strcpy(display_line[2], "   1 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            motorsOFF();
//            break;
//        case 45:
//            strcpy(display_line[0], "Clockwise ");
//            strcpy(display_line[1], "    Spin  ");
//            strcpy(display_line[2], "   3 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            P6OUT  |=  R_FORWARD;
//            P6OUT  |=  L_REVERSE;
//            break;
//        case 60:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "    Wait  ");
//            strcpy(display_line[2], "   2 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            motorsOFF();
//            break;
//        case 70:
//            strcpy(display_line[0], "CounterCW ");
//            strcpy(display_line[1], "    Spin  ");
//            strcpy(display_line[2], "   3 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            P6OUT  |=  L_FORWARD;
//            P6OUT  |=  R_REVERSE;
//            break;
//        case 85:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "    Wait  ");
//            strcpy(display_line[2], "   2 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            motorsOFF();
//            break;
//        case 95:
//            strcpy(display_line[0], "    State ");
//            strcpy(display_line[1], "   Machine");
//            strcpy(display_line[2], " Complete ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            motorsOFF();
//            break;
//        default: break;
//        }
//    }
//}

