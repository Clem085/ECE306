/*
 * motors.h
 *
 *  Created on: Oct 2, 2024
 *      Author: jammi
 */

#ifndef MOTORS_H_
#define MOTORS_H_

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


// PWM Motor Controls
#define PWM_PERIOD          (TB3CCR0)
#define LCD_BACKLITE_DIMING (TB3CCR1)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define LEFT_FORWARD_SPEED  (TB3CCR3)
#define RIGHT_REVERSE_SPEED (TB3CCR4)
#define LEFT_REVERSE_SPEED  (TB3CCR5)

// PWM1 Values
// REMEMBER
    // Reverse or Forward must be 0 when the other is not 0.
    // DESIRED ON AMOUNT must be a value less than WHEEL_PERIOD
    // If WHEEL_PERIOD is 50,005 then
    //DESIRED ON AMOUNT of 20,000 would be ON NEAR 0%
#define PWM1_WHEEL_PERIOD    (50005)
#define PWM1_WHEEL_OFF       (0)
#define PWM1_SLOW_LEFT       (10000)
#define PWM1_SLOW_RIGHT      (10000)
#define PWM1_FAST            (50000)
#define PWM1_PERCENT_100     (50000)
#define PWM1_PERCENT_80      (45000)



#endif /* MOTORS_H_ */
