/* Main Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : main.c
  Description:  This file contains the Main Routine - "While" Operating System
  Programmer: Connor Savugot
  Date Created: Oct 14, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */

// #include as of 11-10-24
//// Header Files
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "motors.h"
#include  "Display.h"
#include  "timers.h"
#include  "interrupts.h"
#include  "switches.h"
#include  "ADC.h"
#include  "IR.h"
#include  "serial.h"
#include  "DAC.h"
#include  "menu.h"
//// Libraries
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

extern volatile unsigned int ADC_Channel;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
extern volatile unsigned int ADC_Thumb;
extern char IR_status;
extern char IR_changed;
extern char ADC_Update;
extern char ADC_Display;
extern char light_percent;
extern char menu;
extern char baud_toggle;


int activateSM;
unsigned int transmit_count;
char state;
unsigned int clear_display;

//volatile unsigned int State_Sequence;



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
    p3_4_type = USE_GPIO;
    Init_Ports();                        // Initialize Ports
    Init_Clocks();                       // Initialize Clock System
    Init_Conditions();            // Initialize Variables and Initial Conditions
    Init_Timers();                       // Initialize Timers
    Init_LCD();                          // Initialize LCD
    Init_ADC();                          // Initialize ADC
    //    Init_DAC();                          // Initialize DAC
    Init_Serial_UCA0();                  // Initialize Serial

    // Place the contents of what you want on the display, in between the quotes
    // Limited to 10 characters per line
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
    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    update_display = TRUE;
    display_changed = TRUE;
    ADC_Update = TRUE;
    ADC_Display = FALSE;
    IR_status = ON;
    state = WAIT;
    light_percent = 80;
    menuType = IDLE;

    baud_toggle = 0;

    wabe();
    while (ALWAYS){                      // Can the Operating system run
        P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
        // Updates
        Display_Process();
        StateMachine(); // Project 8 State Machine

        // Controls
        IR_control();
        PWM_backlight();
        vrfyDirection(); // Protects against Magic Smoke
    }

    //------------------------------------------------------------------------------

}


// Project 8 State Machine
void StateMachine(void){
    if(baud_flag){
        Init_Serial_UCA0();
        baud_flag = 0;
    }

    switch (state) {
    case WAIT:
        strcpy(display_line[0], "  WAITING ");
        strcpy(display_line[1], "          ");
        if (!baud_toggle){
            strcpy(display_line[2], "  115200  ");
        }
        else {
            strcpy(display_line[2], "  460800  ");
        }
        strcpy(display_line[3], "          ");
        display_changed = TRUE;
        break;
    case RECEIVE:
        strcpy(display_line[0], "  RECEIVE ");
        strcpy(display_line[2], "          ");
        //
        unsigned int i = 0;
        unsigned int k = 0;
        for (i = 0; i < sizeof(Rx_display) && k < sizeof(display_line[3]) - 1; i++) {
            // Check if the character is a digit
            if (Rx_display[i] >= '0' && Rx_display[i] <= '9') {
                // Write the digit to display_line[3] in a circular buffer style
                display_line[3][k] = Rx_display[i];
                k = (k + 1) % 16; // Loop back to the beginning after 16 characters
            }

            // Stop if we encounter "CL/" followed by CR or CRLF
            if (Rx_display[i] == 'C' && Rx_display[i + 1] == 'L' && Rx_display[i + 2] == '/' &&
                    (Rx_display[i + 3] == 0x0D || (Rx_display[i + 3] == 0x0D && Rx_display[i + 4] == 0x0A))) {
                // Move index past "CL/" and CR or CRLF and stop processing
                i += (Rx_display[i + 3] == 0x0D && Rx_display[i + 4] == 0x0A) ? 4 : 3;
                break;
            }
        }

        // Null-terminate the line to ensure it�s a valid string
//        display_line[3][k] = '\0';

        display_changed = TRUE;
        break;

    case TRANSMIT:
        strcpy(display_line[0], " TRANSMIT ");
        if(baud_toggle){
            strcpy(display_line[3], Rx_display);
        }else{
            unsigned int j = 0;
            unsigned int l = 0;
            for (j = 0; j < sizeof(Rx_display) && l < sizeof(display_line[3]) - 1; j++) {
                // Check if the character is a digit
                if (Rx_display[j] >= '0' && Rx_display[j] <= '9') {
                    // Write the digit to display_line[1] in a circular buffer style
                    display_line[1][l] = Rx_display[j];
                    l = (l + 1) % 16; // Loop back to the beginning after 16 characters
                }

                // Stop if we encounter "CL/" followed by CR or CRLF
                if (Rx_display[j] == 'C' && Rx_display[j + 1] == 'L' && Rx_display[j + 2] == '/' &&
                        (Rx_display[j + 3] == 0x0D || (Rx_display[j + 3] == 0x0D && Rx_display[j + 4] == 0x0A))) {
                    // Move index past "CL/" and CR or CRLF and stop processing
                    j += (Rx_display[j + 3] == 0x0D && Rx_display[j + 4] == 0x0A) ? 4 : 3;
                    break;
                }
            }

            // Null-terminate the line to ensure it�s a valid string
            display_line[1][l] = '\0';
        }

        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;
        if (transmit_count >= 15){
            state = WAIT;
            transmit_count = 0;
            int ride = 0;
            while (Rx_display[ride] != 0x00){
                Rx_display[ride++] = 0;
            }
        }
        ;
    default: break;
    }

}


// // Project 7 State Machine
//void StateMachine(void)
//{
//    switch (state){
//    case IDLE:
//        if(!IR_changed){
//            strcpy(display_line[0], "   IDLE   ");
//        }
//
//        PWM1_BOTH_OFF(); // motorsOFF();
//        display_changed = TRUE;
//        update_display = TRUE;
//        //        ADC_Update = TRUE; = TRUE;
//        // WAIT state called when SW1 is Pressed
//        break;
//
//    case WAIT:
//        strcpy(display_line[0], "   WAIT   ");
//        display_changed = TRUE;
//        update_display = TRUE;
//        //        ADC_Update = TRUE; = TRUE;
//        switch (Time_Sequence)        {   // Time_Sequence-State_Sequence
//        case 10: // Wait for 1 Second
//            PWM1_BOTH_OFF(); // motorsOFF();
//            state = FWD;
//            break;
//        default:
//            break;
//        }
//        break;
//
//        case FWD:
//            PWM1_BOTH_FWD(); // LRFwdON();
//            strcpy(display_line[0], "   FWD    ");
//            display_changed = TRUE;
//            update_display = TRUE;
//            //        ADC_Update = TRUE; = TRUE;
//            if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM)){
//                state = BLACKLINE; // Black Line Detected
//                PWM1_BOTH_OFF(); // motorsOFF();
//            }
//            break;
//
//        case BLACKLINE:
//            PWM1_BOTH_OFF(); // motorsOFF();
//            strcpy(display_line[0], " BLACKLINE");
//            display_changed = TRUE;
//            update_display = TRUE;
//            //        ADC_Update = TRUE; = TRUE;
//            Time_Sequence = 0; //        State_Sequence = Time_Sequence;
//            state = WAIT2;
//            break;
//
//        case WAIT2:
//            PWM1_BOTH_OFF(); // motorsOFF();
//            if(Time_Sequence < 20){
//                strcpy(display_line[0], "          ");
//                strcpy(display_line[1], "Black Line");
//                strcpy(display_line[2], " Detected ");
//                strcpy(display_line[3], "          ");
//            }
//            display_changed = TRUE;
//            update_display = TRUE;
//            switch (Time_Sequence){ // Time_Sequence-State_Sequence
//            case 20: // Wait for 3 Second
//                strcpy(display_line[0], "          ");
//                strcpy(display_line[1], "          ");
//                strcpy(display_line[2], "          ");
//                strcpy(display_line[3], "          ");
//                display_changed = TRUE;
//                update_display = TRUE;
//                PWM1_BOTH_FWD();
//                if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM)){
//                    Time_Sequence = 0;
//                    state = TURNL; // Black Line Detected
//                    PWM1_RIGHT_OFF(); // motorsOFF();
//                }
//                state = TURNL; // Black Line Detected
//                break;
//            default:
//                break;
//            }
//
//            break;
//
//        case TURNL:
//            PWM1_LEFT_FWD(); // LeftFwdON();
//            strcpy(display_line[0], " TURN LEFT");
//            display_changed = TRUE;
//            update_display = TRUE;
//            //        ADC_Update = TRUE; = TRUE;
//            if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM))
//            {
//                state = LINE1; // Black Line Detected
//                PWM1_BOTH_OFF(); // motorsOFF();
//                Time_Sequence = 0;
//            }
//            break;
//        case LINE1:
//            strcpy(display_line[0], "   LINE1  ");
//            display_changed = TRUE;
//            update_display = TRUE;
//            //        ADC_Update = TRUE; = TRUE;
//            if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM)
//                    && (Time_Sequence >= 10))
//            {
//                state = DONE; // Black Line Detected
//                Time_Sequence = 0; //  State_Sequence = Time_Sequence;
//                PWM1_BOTH_OFF(); // motorsOFF();
//            }
//        case DONE:
//            strcpy(display_line[0], "   DONE   ");
//            display_changed = TRUE;
//            update_display = TRUE;
//            if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//                // Currently on Black Line
//                PWM1_RIGHT_FWD(); //Get off Black Line
//            }else{
//                PWM1_BOTH_OFF();
//                state = RIGHT_SIDE; // Car found White on Left side of Line
//                //                    Time_Sequence = 0;
//            }
//            break;
//            //            case LEFT_SIDE:
//            //                if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//            //                    // Currently on White
//            //                    strcpy(display_line[0], " LEFT IF  ");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_BOTH_OFF();
//            ////                    PWM1_RIGHT_FWD(); //Go accross black line
//            //                    state = RIGHT_SIDE;
//            ////                    Time_Sequence = 0;
//            //                }else{
//            //                    strcpy(display_line[0], " LEFT_SIDE");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_RIGHT_OFF();
//            //                    PWM1_LEFT_FWD();
//            //                }
//            //            case RIGHT_SIDE:
//            //                if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//            //                    // Currently on White
//            //                    strcpy(display_line[0], " RIGHT IF ");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_BOTH_OFF();
//            ////                    PWM1_LEFT_FWD(); //Go accross black line
//            //                    state = LEFT_SIDE;
//            ////                    Time_Sequence = 0;
//            //                }else{
//            //                    strcpy(display_line[0], "RIGHT_SIDE");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_LEFT_OFF();
//            //                    PWM1_RIGHT_FWD();
//            //                }
//
//
//
//            //            case LEFT_SIDE:
//            //                if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//            //                    // Currently on BLACK
//            //                    strcpy(display_line[0], " LEFT IF  ");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_BOTH_OFF();
//            //                    //                      PWM1_RIGHT_FWD(); //Go accross black line
//            //                    state = MIDDLE_L;
//            //                    Time_Sequence = 0;
//            //                }else{
//            //                    strcpy(display_line[0], " LEFT_SIDE");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_RIGHT_OFF();
//            //                    PWM1_LEFT_FWD();
//            //                }
//            //            case MIDDLE_L:
//            //                if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM)){
//            //                    PWM1_BOTH_OFF(); // motorsOFF();
//            //                    // Car is on White
//            //                    state = RIGHT_SIDE; // Black Line Detected
//            //                }else{// Otherwise, car still moving Left from previous state
//            //                    strcpy(display_line[0], " MIDDLE_L ");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_RIGHT_OFF();
//            //                    PWM1_LEFT_FWD();
//            //                }
//        case RIGHT_SIDE:
//            if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//                strcpy(display_line[0], "RIGHT_SIDE");
//                display_changed = TRUE;
//                update_display = TRUE;
//                PWM1_LEFT_OFF();
//                PWM1_RIGHT_FWD();
//                state = LEFT_SIDE;
//                Time_Sequence = 0;
//            }else{
//                // Currently on Black Line
//                strcpy(display_line[0], " RIGHT_BLK");
//                display_changed = TRUE;
//                update_display = TRUE;
//                PWM1_BOTH_FWD();
//                if(Time_Sequence > 50){
//                    Time_Sequence = 0;
//                    state = LEFT_SIDE;
//                }
//
//            }
//        case LEFT_SIDE:
//            if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//                strcpy(display_line[0], " LEFT_SIDE");
//                display_changed = TRUE;
//                update_display = TRUE;
//                PWM1_LEFT_OFF();
//                PWM1_RIGHT_FWD();
//                state = RIGHT_SIDE;
//                Time_Sequence = 0;
//            }else{
//                // Currently on Black Line
//                strcpy(display_line[0], " LEFT_BLK ");
//                display_changed = TRUE;
//                update_display = TRUE;
//                //                    PWM1_RIGHT_OFF();
//                PWM1_BOTH_FWD();
//                //                    PWM1_LEFT_FWD(); //Go accross black line
//                //                    state = LEFT_SIDE;
//                if(Time_Sequence > 50){
//                    Time_Sequence = 0;
//                    state = RIGHT_SIDE;
//                }
//            }
//            //            case MIDDLE_R:
//            //                if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM)){
//            //                    PWM1_BOTH_OFF(); // motorsOFF();
//            //                    // Car is on White
//            //                    state = LEFT_SIDE; // Black Line Detected
//            //                }else{// Otherwise, car still moving Left from previous state
//            //                    strcpy(display_line[0], " MIDDLE_R ");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_LEFT_OFF();
//            //                    PWM1_RIGHT_FWD();
//            //                }
//        default:
//            break;
//    }
//}

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

