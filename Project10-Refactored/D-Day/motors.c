/* Motors Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : motors.c
  Description:  This file contains the code to control the motors using PWM
  Programmer: Connor Savugot
  Date Created: Sep 20, 2024
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

// Global Variables declared and referenced in Header file



// Magic Smoke Detector
void vrfyDirection(void){
    if(((P6IN & L_FORWARD) && (P6IN & L_REVERSE)) || ((P6IN & R_FORWARD) && (P6IN & R_REVERSE))){
        // ISSUE: Both Left Forward and Reverse are on
        P1OUT |= RED_LED;
        motorsOFF(); // Turns off both left and right motor for the forward and reverse direction
        strcpy(display_line[0], " Error!!! ");
        strcpy(display_line[1], "MagicSmoke");
        strcpy(display_line[2], "Move Fwd &");
        strcpy(display_line[3], "Move Rev! ");
        display_changed = TRUE;
        backlight_status = 1;
    }
}


// --------------------------------------------------------------
// PWM // ALWAYS SLOW
//// FORWARD
void PWM_LEFT_FWD(void){
    LEFT_REVERSE_SPEED = PWM_WHEEL_OFF;
    LEFT_FORWARD_SPEED = PWM_SLOW_LEFT;
//    PWM_RIGHT_OFF();
}
void PWM_RIGHT_FWD(void){
    RIGHT_REVERSE_SPEED = PWM_WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PWM_SLOW_RIGHT;
//    PWM_LEFT_OFF();
}
void PWM_BOTH_FWD(void){
    LEFT_REVERSE_SPEED = PWM_WHEEL_OFF;
    RIGHT_REVERSE_SPEED = PWM_WHEEL_OFF;
    LEFT_FORWARD_SPEED = PWM_SLOW_LEFT;
    RIGHT_FORWARD_SPEED = PWM_SLOW_RIGHT;
}

// OFF
void PWM_LEFT_OFF(void){
    LEFT_REVERSE_SPEED = PWM_WHEEL_OFF;
    LEFT_FORWARD_SPEED = PWM_WHEEL_OFF;
//    PWM_RIGHT_OFF();
}
void PWM_RIGHT_OFF(void){
    RIGHT_REVERSE_SPEED = PWM_WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PWM_WHEEL_OFF;
//    PWM_LEFT_OFF();
}
void PWM_BOTH_OFF(void){
    LEFT_REVERSE_SPEED = PWM_WHEEL_OFF;
    LEFT_FORWARD_SPEED = PWM_WHEEL_OFF;
    RIGHT_REVERSE_SPEED = PWM_WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PWM_WHEEL_OFF;
}

// REVERSE
void PWM_LEFT_REV(void){
    LEFT_FORWARD_SPEED = PWM_WHEEL_OFF;
    LEFT_REVERSE_SPEED = PWM_SLOW_LEFT;
//    PWM_RIGHT_OFF();
}
void PWM_RIGHT_REV(void){
    RIGHT_FORWARD_SPEED = PWM_WHEEL_OFF;
    RIGHT_REVERSE_SPEED = PWM_SLOW_RIGHT;
//    PWM_LEFT_OFF();
}
void PWM_BOTH_REV(void){
    LEFT_FORWARD_SPEED = PWM_WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PWM_WHEEL_OFF;
    LEFT_REVERSE_SPEED = PWM_SLOW_LEFT;
    RIGHT_REVERSE_SPEED = PWM_SLOW_RIGHT;
}




// --------------------------------------------------------------
// BASIC MOVEMENT
// Forward
// Left
void LeftFwdON(void){
    P6OUT  |=  L_FORWARD;
}
void LeftFwdOFF(void){
    P6OUT  &= ~L_FORWARD;
}
// Right
void RightFwdON(void){
    P6OUT  |=  R_FORWARD;
}
void RightFwdOFF(void){
    P6OUT  &= ~R_FORWARD;
}
// Both
void LRFwdON(void){
    LeftFwdON();
    RightFwdON();
}
void LRFwdOFF(void){
    LeftFwdOFF();
    RightFwdOFF();
}


// Reverse
// Left
void LeftRevON(void){
    P6OUT  |=  L_REVERSE;
}
void LeftRevOFF(void){
    P6OUT  &= ~L_REVERSE;
}
// Right
void RightRevON(void){
    P6OUT  |=  R_REVERSE;
}
void RightRevOFF(void){
    P6OUT  &= ~R_REVERSE;
}
// Both
void LRRevON(void){
    LeftRevON();
    RightRevON();
}
void LRRevOFF(void){
    LeftRevOFF();
    RightRevOFF();
}

// Both
// NO Function to turn both Forward and Reverse on at the same time because I don't want my car to blow up
void motorsOFF(void){
    LRFwdOFF();
    LRRevOFF();
    //    ForwardOff();
    //    ReverseOff();
}
//--------------------------------------------------------------
// END OF BASIC MOVEMENT
