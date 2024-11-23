/*
 * wheels.c
 *
 *  Created on: Jan 31, 2024
 *      Author: varun
 */


#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"


void Wheels_Stop(void);
void Wheels_Forward(void);
void Wheels_Reverse(void);
void Wheels_clkw(void);
void Wheels_counterclkw(void);
void motor_run_forward(void);
void motor_run_backward(void);
void motor_run_right(void);
void motor_run_left(void);



void motor_off (void) {
  RIGHT_FORWARD_SPEED = 0;
  RIGHT_REVERSE_SPEED = 0;
  LEFT_FORWARD_SPEED = 0;
  LEFT_REVERSE_SPEED = 0;
}

void motor_run_forward (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
//    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

//    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = FAST; // P6.1 Right Forward PWM ON amount

   RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
//    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM ON amount

//    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
    LEFT_FORWARD_SPEED = FAST; // P6.3 Left Forward PWM ON amount

//    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM ON amount
}

void motor_run_backward (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
//    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

   RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
//    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM ON amount

//    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
    RIGHT_REVERSE_SPEED = FAST; // P6.2 Right Reverse PWM ON amount

    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
//    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM ON amount

//    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
    LEFT_REVERSE_SPEED = FAST; // P6.4 Left Reverse PWM ON amount
}

void motor_run_right (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
//    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
//   RIGHT_FORWARD_SPEED = WHEEL_OFF;; // P6.1 Right Forward PWM ON amount

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
//    RIGHT_REVERSE_SPEED = FAST; // P6.2 Right Reverse PWM ON amount
    // ^^ USED TO BE FAST

//    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
    LEFT_FORWARD_SPEED = FAST; // P6.3 Left Forward PWM ON amount

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
//    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM ON amount
}

void motor_run_left (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
//    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

//    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = FAST; // P6.1 Right Forward PWM ON amount
    // ^^ USED TO BE FAST

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
//   RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM ON amount

    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
//    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM ON amount

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
//    LEFT_REVERSE_SPEED = FAST; // P6.4 Left Reverse PWM ON amount
}

void motor_check (void) {
  if ((RIGHT_FORWARD_SPEED > 0) && (RIGHT_REVERSE_SPEED > 0)) {
    motor_off();
    P1OUT |= RED_LED;
  }
  if ((LEFT_FORWARD_SPEED > 0) && (LEFT_REVERSE_SPEED > 0)) {
    motor_off();
    P1OUT |= RED_LED;
  }
}

void Wheels_Stop(void){//code to stop wheels
    P6OUT &= ~L_FORWARD; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_FORWARD; // Set Port pin Low [Wheel Off]
    P6OUT &= ~L_REVERSE; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_REVERSE; // Set Port pin Low [Wheel Off]
}

void Wheels_Forward(void){// code to move forwards
    P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
    P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]
}


void Wheels_Reverse(void){//code to move in reverse
    P6OUT |= L_REVERSE;
    P6OUT |= R_REVERSE;
}

void Wheels_clkw(void){
    P6OUT |= L_FORWARD;
    P6OUT |= R_REVERSE;
}

void Wheels_counterclkw(void){
    P6OUT |= R_FORWARD;
    P6OUT |= L_REVERSE;
}
