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

unsigned int ADC_Left_Detect;
unsigned int ADC_Right_Detect;
unsigned int Lucas_counter;
unsigned int flag_lucas = FALSE;
unsigned int Varun_counter;
unsigned int Flag_varun = FALSE;
unsigned int dawg_counter;
unsigned int flag_schnawg = FALSE;
volatile unsigned char display_changed;
volatile unsigned char update_display;
unsigned int flag_opp = FALSE;
unsigned int flag_poo =  FALSE;
unsigned int flag_happy;
extern unsigned int line_flag;
extern char track_state;
char display_line[4][11];
extern unsigned int detect_time;
unsigned int detect_flag;




void Wheels_Stop(void);
void Wheels_Forward(void);
void Wheels_Reverse(void);
void Wheels_clkw(void);
void Wheels_counterclkw(void);
void motor_run_forward(void);
void motor_run_backward(void);
void motor_run_right(void);
void motor_run_left(void);

void start_moving(){
    motor_off();
    LEFT_FORWARD_SPEED = SPEED3;
    RIGHT_FORWARD_SPEED = SPEED2;
    strcpy(display_line[0], "BL START  ");

    display_changed = TRUE;
    update_display = TRUE;

    if((ADC_Left_Detect >= 350) && (ADC_Right_Detect >= 350)){
        track_state = DETECT;
    }
}

void detect_function(){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    strcpy(display_line[0], "intercept ");
    display_changed = TRUE;

}

void line_turn(){
        strcpy(display_line[0], " LINETURN ");
        update_display = TRUE;
        display_changed = TRUE;
        if (ADC_Left_Detect <= 300) {
            P6OUT  ^= GRN_LED;
            P1OUT ^=  RED_LED;
            flag_lucas = TRUE;
            if(Lucas_counter >= 100){
                RIGHT_FORWARD_SPEED = SPEED3;
                LEFT_REVERSE_SPEED = SPEED3;
                flag_lucas = FALSE;
            }


        }else {

            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            RIGHT_FORWARD_SPEED = WHEEL_OFF;

            Flag_varun = TRUE;
            if(Varun_counter >= 200){
                track_state = TRACK;
                Flag_varun = FALSE;

            }
        }

    }


void track_line(){
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    if (ADC_Left_Detect >= 260 && ADC_Right_Detect >= 260){
                LEFT_FORWARD_SPEED = SPEED3;
                RIGHT_FORWARD_SPEED = SPEED3;
    }else if (ADC_Left_Detect < 260 && ADC_Right_Detect >= 260){
                RIGHT_FORWARD_SPEED = SPEED2;
                LEFT_FORWARD_SPEED = 0;
                flag_opp = TRUE;
                flag_happy = TRUE;
    }else if (ADC_Left_Detect >= 260 && ADC_Right_Detect < 260){
                LEFT_FORWARD_SPEED = 0;
                RIGHT_FORWARD_SPEED = SPEED2;
                flag_poo = TRUE;
    }else if (ADC_Left_Detect < 260 && ADC_Right_Detect < 260){

        LEFT_FORWARD_SPEED = 0;
        RIGHT_FORWARD_SPEED = 0;
        if (flag_happy == TRUE){
            LEFT_FORWARD_SPEED = SPEED2;
            flag_happy == FALSE;
        }
        else if (flag_poo == TRUE){
            RIGHT_FORWARD_SPEED = SPEED2;
            flag_poo == FALSE;
        }

    }


}



void motor_off (void) {
  RIGHT_FORWARD_SPEED = 0;
  RIGHT_REVERSE_SPEED = 0;
  LEFT_FORWARD_SPEED = 0;
  LEFT_REVERSE_SPEED = 0;
}

void motor_run_forward (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = FAST; // P6.1 Right Forward PWM ON amount

   RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM ON amount

    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
    LEFT_FORWARD_SPEED = FAST; // P6.3 Left Forward PWM ON amount

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM ON amount
}

void motor_run_backward (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

   RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = WHEEL_OFF;; // P6.1 Right Forward PWM ON amount

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
    RIGHT_REVERSE_SPEED = FAST; // P6.2 Right Reverse PWM ON amount

    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM ON amount

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
    LEFT_REVERSE_SPEED = FAST; // P6.4 Left Reverse PWM ON amount
}

void motor_run_right (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
   RIGHT_FORWARD_SPEED = WHEEL_OFF;; // P6.1 Right Forward PWM ON amount

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
    RIGHT_REVERSE_SPEED = FAST; // P6.2 Right Reverse PWM ON amount

    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
    LEFT_FORWARD_SPEED = FAST; // P6.3 Left Forward PWM ON amount

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM ON amount
}

void motor_run_left (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = FAST; // P6.1 Right Forward PWM ON amount

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
   RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM ON amount

    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM ON amount

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
    LEFT_REVERSE_SPEED = FAST; // P6.4 Left Reverse PWM ON amount
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
