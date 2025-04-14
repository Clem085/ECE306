/*
 * wheels.c
 *
 *  Created on: Sep 25, 2024
 *      Author: kaylaradu
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "switches.h"
#include "wheels.h"
#include  <stdio.h>






volatile unsigned char event;  // Event variable from switches.c (e.g., STRAIGHT, CIRCLE)
extern volatile unsigned int event_Counter; //picking the event based on increment

extern unsigned int Last_Time_Sequence;  // To track changes in Time_Sequence
extern unsigned int cycle_time;          // Controls shape timings
extern unsigned int time_change;         // Flag to detect time sequence change
unsigned int Time_Sequence;

unsigned int delay_start;
unsigned int left_motor_count;
unsigned int right_motor_count;
unsigned int segment_count;


unsigned int waitingtostart;
extern volatile unsigned int rdirection = 0;
extern volatile unsigned int ldirection = 0;
extern volatile unsigned int instruction;
volatile unsigned char origevent;
extern volatile unsigned int elapsed_time = 0;
extern volatile unsigned int desiredtime= 0;
extern volatile unsigned int desired_cycles = 0;

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int switchpressed;
extern volatile unsigned int instruction;

extern volatile unsigned int blackleft;
extern volatile unsigned int blackright;
extern volatile unsigned int blacklinefound;

extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;

unsigned int SpincountL;
unsigned int SpincountR;
extern unsigned int FlagSpinL;
extern unsigned int FlagSpinR;
unsigned int DirectionTurn;

volatile unsigned char state;



// This sets the car to start moving toward the line unitl it sees it
void start_movement(void){
    Off_Case();
    LEFT_FORWARD_SPEED = 12000;
    RIGHT_FORWARD_SPEED = 12000;

    strcpy(display_line[0], "INTERCEPT ");
    display_changed = TRUE;

    //        if((ADC_Left_Detect >= BLACK) || (ADC_Right_Detect >= BLACK)){
    //            state = SPIN;
    //        }

            if((ADC_Left_Detect >= 500 ) || (ADC_Right_Detect >= 500)){
                state = DETECTED;
            }

}

void detect_movement(void){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    strcpy(display_line[0], "BLACK LINE ");
    display_changed = TRUE;
    state = SPIN;
}

void spinning_movement(void){
    // now get it off course like a turn
    FlagSpinL = TRUE;
    strcpy(display_line[0], "SPINNING ");
    display_changed = TRUE;
    RIGHT_FORWARD_SPEED = SPINSPEED;
    LEFT_REVERSE_SPEED = SPINSPEED;
    if(SpincountL >= 3){
        state = TRACK;
        SpincountL = 0;
    }
}

void tracking_movement(void){
    strcpy(display_line[0], " TRACKING ");
    display_changed = TRUE;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;

    // IT IS ON THE LINE
    if(ADC_Left_Detect >= BLACKDETECT && ADC_Right_Detect >= BLACKDETECT){
        RIGHT_FORWARD_SPEED = STRAIGHTMOVE;
        LEFT_FORWARD_SPEED = STRAIGHTMOVE;
        // THE LEFT SENSOR IS MISSING, BUT RIGHT SENSOR IS ON THE LINE
    }else if(ADC_Left_Detect < BLACKDETECT && ADC_Right_Detect >= BLACKDETECT){
        LEFT_FORWARD_SPEED = TURNSPEED;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        DirectionTurn = RIGHT;
        // THE RIGHT SENSOR IS MISSING I NEED TO TURN LEFT TOWARD THE LINE
    }else if(ADC_Left_Detect >= BLACKDETECT && ADC_Right_Detect < BLACKDETECT){
        RIGHT_FORWARD_SPEED = TURNSPEED;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        DirectionTurn = LEFT;
        // IT IS WAYYY OFF AND CANNOT FIND THE LINE
    }else if(ADC_Left_Detect < BLACKDETECT && ADC_Right_Detect < BLACKDETECT){
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;

        // GO THE OPPOSITE WAY TI DID EARLIER
        if(DirectionTurn == RIGHT){
            RIGHT_FORWARD_SPEED = TURNSPEED;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            DirectionTurn = LEFT;
        }else if(DirectionTurn == LEFT){
            LEFT_FORWARD_SPEED = TURNSPEED;
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            DirectionTurn = RIGHT;
        }
    }

    FlagSpinR = TRUE;
    if(SpincountR >= FIFTYSEC){
        state = END;
        SpincountR = OFF;
        FlagSpinR = FALSE;
    }

}


void end_state(void){
    strcpy(display_line[0], " STOPPED ");
    update_display = TRUE;
    display_changed = 1;
    Off_Case();
    SpincountL = 0;
    SpincountR = 0;
    state = NONE;

}




void LForward_On(void) {
    P6OUT |= L_FORWARD;   // Turn on the left motor (set pin high)
}
void RForward_On(void) {
    P6OUT |= R_FORWARD;
}

void LForward_Off(void){
    P6OUT &= ~L_FORWARD;  // Turn off the left motor (set pin low)
}
void RForward_Off(void){
    P6OUT &= ~R_FORWARD;  // Turn off the left motor (set pin low)
}
void LReverse_On(void) {
    P6OUT |= L_REVERSE;   // Turn on the left motor (set pin high)
}
void RReverse_On(void){
    P6OUT |= R_REVERSE;
}
void LReverse_Off(void){
    P6OUT &= ~L_REVERSE;  // Turn off the left motor (set pin low)
}
void RReverse_Off(void){
    P6OUT &= ~R_REVERSE;  // Turn off the left motor (set pin low)
}
void Forward_Off(void){
    P6OUT &= ~L_FORWARD;
    P6OUT &= ~R_FORWARD;

}
void Reverse_Off(void){
    P6OUT &= ~L_REVERSE;
    P6OUT &= ~R_REVERSE;

}

void Forward_Move(void) {
    if(ldirection == FORWARD){
        LForward_On();
    }
    if(rdirection == FORWARD){
        RForward_On();
    }

}
void Reverse_Move(void) {
    if(ldirection == REVERSE){
        LReverse_On();
    }
    if(rdirection == REVERSE){
        RReverse_On();
    }
}



void Off_Case(void){
    //    Forward_Off();
    //    Reverse_Off();
    RIGHT_FORWARD_SPEED = WHEEL_OFF;   // P6.1 Right Forward PWM duty cycle


    LEFT_FORWARD_SPEED = WHEEL_OFF;    // P6.2 Right Forward PWM duty cycl


    RIGHT_REVERSE_SPEED = WHEEL_OFF;   // P6.3 Left Forward PWM duty cycle


    LEFT_REVERSE_SPEED = WHEEL_OFF;
}



void motorDirec(void){
    if(((P6IN & L_FORWARD)&&(P6IN & L_REVERSE))||((P6IN & R_FORWARD)&&(P6IN & R_REVERSE))){
        P1OUT |= RED_LED;
        Forward_Off();
        Reverse_Off();
    }

}
