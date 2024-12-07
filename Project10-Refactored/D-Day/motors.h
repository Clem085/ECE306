/*
 * motors.h
 *
 *  Created on: Oct 2, 2024
 *      Author: jammi
 */

#ifndef MOTORS_H_
#define MOTORS_H_


// Global Variables
//// Internally Defined
unsigned char LeftDir;
unsigned char RightDir;
unsigned int secTime; // Define in NEW Clocks File

//// Externally Defined
// Globals
extern char state;
extern unsigned char event;
extern char display_line[4][11];
extern unsigned int travel_distance;
extern unsigned int right_count_time;
extern unsigned int left_count_time;
extern unsigned int wheel_count_time;

extern unsigned int time_change;
extern unsigned int delay_start;
extern unsigned int cycle_time;
extern unsigned int right_motor_count;
extern unsigned int left_motor_count;
extern unsigned int segment_count;
extern char backlight_status;

extern unsigned int straight_step;
extern unsigned int circle_step;
extern unsigned int triangle_step;
extern unsigned int figure8_step;

extern unsigned char dispEvent;
extern volatile unsigned char display_changed;
extern unsigned char event;
extern char display_line[4][11];



// Macro Definitions
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
#define PWM_WHEEL_PERIOD    (50005)
#define PWM_WHEEL_OFF       (0)
#define PWM_SLOW_LEFT       (10000)
#define PWM_SLOW_RIGHT      (10000)
#define PWM_FAST            (50000)
#define PWM_PERCENT_100     (50000)
#define PWM_PERCENT_80      (45000)
#define PWM_PERCENT_0       (0)



// Function Declarations
// ----- BASIC MOVEMENT -----
// Forward
    // Left
void LeftFwdON(void);
void LeftFwdOFF(void);
    // Right
void RightFwdON(void);
void RightFwdOFF(void);
    // Both
void LRFwdON(void);
void LRFwdOFF(void);

// Reverse
    // Left
void LeftRevON(void);
void LeftRevOFF(void);
    // Right
void RightRevON(void);
void RightRevOFF(void);
    // Both
void LRRevON(void);
void LRRevOFF(void);

// Both
void motorsOFF(void);

// ----- MOVEMENT SEQUENCES -----
// PROJECT 5 - MOVEMENT SEQUENCE
void moveSeq_proj5(void);

// vrfyDirectiom
// Ensures wheels are not moving in both the forward and reverse direction
void vrfyDirection(void);

// ----- STATES -----
void wait_case_proj5(void);
void start_case_proj5(void);
void run_case_proj5(void);
void end_case_proj5(void);

// ----- EVENTS -----
// Project 4
void Move_Shape(void);
void straight(void);
void circle(void);
void triangle(void);
void figure8(void);

// Project5
void forward_proj5(void);

// Motor Direction Compare
void motorDirection(void);


//---------- PWM ------------//
// PWM Functions
// FORWARD
void PWM1_LEFT_FWD(void);
void PWM1_RIGHT_FWD(void);
void PWM1_BOTH_FWD(void);

// STOP
void PWM1_LEFT_OFF(void);
void PWM1_RIGHT_OFF(void);
void PWM1_BOTH_OFF(void);

// REVERSE
void PWM1_LEFT_REV(void);
void PWM1_RIGHT_REV(void);
void PWM1_BOTH_REV(void);





#endif /* MOTORS_H_ */
