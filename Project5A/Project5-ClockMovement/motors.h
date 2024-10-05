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


#endif /* MOTORS_H_ */
