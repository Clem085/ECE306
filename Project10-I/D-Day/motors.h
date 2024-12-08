//===========================================================================
//  File Name: motors.h
//  Description: Header file for motors.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Nov 10, 2024
//===========================================================================

#ifndef MOTORS_H_
#define MOTORS_H_

#define WHEEL_COUNT_TIME (10)
#define RIGHT_COUNT_TIME (7)
#define LEFT_COUNT_TIME (8)
//#define TRAVEL_DISTANCE (2)
#define WAITING2START (50)
#define ONESEC (5)


#define NONE ('N')
//#define FORWARD ('F')
//#define REVERSE ('R')
//
//
//#define FALSE (0)
//#define OFF (0)

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


#endif /* MOTORS_H_ */
