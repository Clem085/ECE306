//===========================================================================
//  File Name: motors.h
//  Description: Header file for motors.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Nov 10, 2024
//===========================================================================

#ifndef MOTORS_H_
#define MOTORS_H_

// Global Variables
//// Internally Defined
////// Arch Variables
unsigned int arch_counter;
unsigned int archNext;
unsigned int archState;
unsigned int nextState;
unsigned int motorDrain;
unsigned int init_cmd_state;
////// Black Line
unsigned int following;

//// Externally Defined
extern char command;
extern unsigned int run_time;
extern unsigned int run_time_flag;
extern unsigned int movement;
extern unsigned int setTime;




// Macro Definitions
//// Old #defines
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
#define TWOMIN (2400)


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


// Function Declarations
//// PWM
void motors_off(void);
void fwd_off(void);
void rev_off(void);
void spin_left_off(void);
void spin_right_off(void);
void fwd_slow(void);
void rev_slow(void);
void fwd_fast(void);
void rev_fast(void);
void left_fast(void);
void right_fast(void);
void spin_left(void);
void spin_right(void);
//// Project 10 Movements
void arch_movement(void);
void blacklinemachine(void);
void initialMovementBL(void);
//// Magic Smoke Detection
void vrfyDirection(void);






#endif /* MOTORS_H_ */
