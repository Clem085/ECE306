//===========================================================================
//  File Name: motors.h
//  Description: Header file for motors.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Oct 2, 2024
//===========================================================================

#ifndef MOTORS_H_
#define MOTORS_H_


// Global Variables
////// Internally Defined
//unsigned char LeftDir;
//unsigned char RightDir;
//unsigned int secTime; // Define in NEW Clocks File
//
////// Externally Defined
//// Globals
//extern char display_line[4][11];
//extern unsigned int travel_distance;
//extern unsigned int right_count_time;
//extern unsigned int left_count_time;
//extern unsigned int wheel_count_time;
//
//extern unsigned int time_change;
//extern unsigned int delay_start;
//extern unsigned int cycle_time;
//extern unsigned int right_motor_count;
//extern unsigned int left_motor_count;
//extern unsigned int segment_count;
//extern char backlight_status;
//
//extern unsigned char dispEvent;
//extern volatile unsigned char display_changed;
//
//volatile unsigned char event;  // Event variable from switches.c (e.g., STRAIGHT, CIRCLE)
//extern volatile unsigned int event_Counter; //picking the event based on increment
//
//extern unsigned int Last_Time_Sequence;  // To track changes in Time_Sequence
//extern unsigned int cycle_time;          // Controls shape timings
//extern unsigned int time_change;         // Flag to detect time sequence change
//extern volatile unsigned int Time_Sequence;

//unsigned int delay_start;
//unsigned int left_motor_count;
//unsigned int right_motor_count;
//unsigned int segment_count;
//
//
//unsigned int waitingtostart;
//extern volatile unsigned int rdirection = 0;
//extern volatile unsigned int ldirection = 0;
//extern volatile unsigned int instruction;
//volatile unsigned char origevent;
//extern volatile unsigned int elapsed_time = 0;
//extern volatile unsigned int desiredtime= 0;
//extern volatile unsigned int desired_cycles = 0;
//
//extern char display_line[4][11];
//extern char *display[4];
//extern volatile unsigned char display_changed;
//extern volatile unsigned char update_display;
//extern volatile unsigned int switchpressed;
//extern volatile unsigned int instruction;
//
//extern volatile unsigned int blackleft;
//extern volatile unsigned int blackright;
//extern volatile unsigned int blacklinefound;
//
//extern volatile unsigned int ADC_Left_Detect;
//extern volatile unsigned int ADC_Right_Detect;
//
//unsigned int SpincountL = 0;
//unsigned int SpincountR = 0;
//unsigned int Waitcount = 0;
//extern unsigned int FlagSpinL;
//extern unsigned int FlagSpinR;
//extern unsigned int FlagWait;
//unsigned int DirectionTurn = 0;
//
//volatile unsigned char state;
//
//unsigned int lostCounter = 0;
//unsigned int lostflg = 0;
//
//extern unsigned int black_low;
//extern unsigned int black_high;



// Macro Definitions
#define WHEEL_COUNT_TIME (10)
#define RIGHT_COUNT_TIME (7)
#define LEFT_COUNT_TIME (8)
#define WAITING2START (50)
#define ONESEC (5)
#define NONE ('N')
#define RIGHT (1)
#define LEFT (0)
#define PAUSE (200) // originally 20, changed to 200 for TA Demo
#define FORWARDSPEED (12000)
#define SPINSPEED (15000)
#define TURNSPEED (10000)
#define SPEED1 (10000)
#define STRAIGHTMOVE (10000)
#define RIGHTARCHFWD (26000) //26000
#define LEFTARCHFWD (20000)  //20000
#define RSLOWCIRCLE (35000) // 35000
#define LSLOWCIRCLE (25000) // 25000


// PWM Motor Controls
#define PWM_PERIOD          (TB3CCR0)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define LEFT_FORWARD_SPEED  (TB3CCR3)
#define RIGHT_REVERSE_SPEED (TB3CCR4)
#define LEFT_REVERSE_SPEED  (TB3CCR5)
// PWD Display Control moved to Display.h

// PWM1 Values
// REMEMBER
    // Reverse or Forward must be 0 when the other is not 0.
    // DESIRED ON AMOUNT must be a value less than WHEEL_PERIOD
    // If WHEEL_PERIOD is 50,005 then
    //DESIRED ON AMOUNT of 20,000 would be ON NEAR 0%
//// PWM Off
#define WHEEL_OFF      (00000)

//// Slow PWM Values
#define SLOW_FWD_RIGHT (20000)
#define SLOW_FWD_LEFT  (20000)
#define SLOW_REV_RIGHT (20000)
#define SLOW_REV_LEFT  (20000)
#define SLOW_SPIN      (15000)

//// Fast PWM Values
#define FAST_FWD_RIGHT (35000)
#define FAST_FWD_LEFT  (35000)
#define FAST_REV_RIGHT (35000)
#define FAST_REV_LEFT  (35000)





// For Backlight, defined ESLEWHERE
// #define PWM_PERCENT_100     (50000)
// #define PWM_PERCENT_80      (45000)
// #define PWM_PERCENT_0       (0)



// Function Declarations


#endif /* MOTORS_H_ */
