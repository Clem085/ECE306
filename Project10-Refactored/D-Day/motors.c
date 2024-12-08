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


// Motor // PWM // START //
// --------------------------------------------------------------
//// PWM Off
void motors_off(void){
    fwd_off();
    rev_off();
}
void fwd_off(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}
void rev_off(void){
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}
void spin_left_off(void){
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}
void spin_RIGHT_off(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}
//// Slow PWM Values
void fwd_slow(void){
    rev_off();
    RIGHT_FORWARD_SPEED = SLOW_FWD_RIGHT;
    LEFT_FORWARD_SPEED = SLOW_FWD_LEFT;
}
void rev_slow(void){
    fwd_off();
    RIGHT_REVERSE_SPEED = SLOW_FWD_RIGHT;
    LEFT_REVERSE_SPEED = SLOW_FWD_LEFT;
}
//// Fast PWM Values
void fwd_fast(void){
    rev_off();
    RIGHT_FORWARD_SPEED = FAST_FWD_RIGHT;
    LEFT_FORWARD_SPEED = FAST_FWD_LEFT;
}
void rev_fast(void){
    fwd_off();
    RIGHT_REVERSE_SPEED = FAST_FWD_RIGHT;
    LEFT_REVERSE_SPEED = FAST_FWD_LEFT;
}
void left_fast(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; 
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = FAST_FWD_RIGHT; 
}
void right_fast(void){
    LEFT_FORWARD_SPEED = WHEEL_OFF; 
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = FAST_FWD_RIGHT; 
}
//// Spin Speeds On
void spin_left(void){
    spin_right_off();
    RIGHT_REVERSE_SPEED = SLOW_SPIN;
    LEFT_FORWARD_SPEED = SLOW_SPIN;
}
void spin_right(void){
    spin_left_off();
    RIGHT_FORWARD_SPEED = SLOW_SPIN;
    LEFT_REVERSE_SPEED = SLOW_SPIN;
}
//--------------------------------------------------------------
// Motor // PWM // END //



// Magic Smoke Detector
void vrfyDirection(void){
    if(((P6IN & L_FORWARD) && (P6IN & L_REVERSE)) || ((P6IN & R_FORWARD) && (P6IN & R_REVERSE))){
        // ISSUE: Both Left Forward and Reverse are on
        P1OUT |= RED_LED;
        motors_off(); // Turns off both left and right motor for the forward and reverse direction
        strcpy(display_line[0], " Error!!! ");
        strcpy(display_line[1], "MagicSmoke");
        strcpy(display_line[2], "Move Fwd &");
        strcpy(display_line[3], "Move Rev! ");
        display_changed = TRUE;
        backlight_status = 1;
    }
}
