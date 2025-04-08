/*
 * motors.h
 *
 *  Created on: Oct 2, 2024
 *      Author: jammi
 */

#ifndef MOTORS_H_
#define MOTORS_H_


// Globals
extern unsigned char dispEvent;
extern volatile unsigned char display_changed;
extern unsigned char event;
extern char display_line[4][11];

extern unsigned int straight_step;
extern unsigned int circle_step;
extern unsigned int circle_step2;
extern unsigned int triangle_step;
extern unsigned int figure8_step;

extern unsigned char state;

// TEST
extern int Switch1_Pressed;
extern int Switch2_Pressed;
extern int Switch_Counter1;
extern int Switch_Counter2;
int okay_to_look_at_switch1=1;
int count_debounce_SW1;
int sw1_position=1;
int okay_to_look_at_switch2=1;
int count_debounce_SW2;
int sw2_position=1;
extern volatile unsigned int debounce_count1;
extern volatile unsigned int debounce_count2;
extern unsigned int backlight_status;





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
void



#endif /* MOTORS_H_ */
