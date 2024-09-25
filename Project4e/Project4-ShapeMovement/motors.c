/* Program Information Header
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : motors.c
  Description:  This file contains the code to control the motors
      >>> Motors Forward
          Motors Stop
          Circle
          Triangle
          Figure8

  Programmer: Connor Savugot
  Date: Sep 20, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */
// Includes
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "msp430.h"

// Globals
extern unsigned char state;
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


/* Functions Included in this File
    LRmotorForward
    LmotorForward
    RmotorForward

    LRmotorStop
    LmotorStop
    RmotorStop
 */

// FORWARD Commands
void LRmotorForward(void){
    //  Select and Turn Off Backlight
    //    backlightControl(0);

    //  Turn ON Motors
    P6SEL0 &= ~R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6OUT  |=  R_FORWARD;
    P6DIR  |=  R_FORWARD;

    P6SEL0 &= ~L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6OUT  |=  L_FORWARD;
    P6DIR  |=  L_FORWARD;
}

void LmotorForward(void){
    //  Select and Turn Off Backlight
    //    backlightControl(0);

    //  Turn ON Left Motor
    P6SEL0 &= ~L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6OUT  |=  L_FORWARD;
    P6DIR  |=  L_FORWARD;
}

void RmotorForward(void){
    //  Select and Turn Off Backlight
    //    backlightControl(0);

    //  Turn ON Right Motor
    P6SEL0 &= ~R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6OUT  |=  R_FORWARD;
    P6DIR  |=  R_FORWARD;
}



// STOP Commands
void LRmotorStop(void){
    //  Select and Turn On Backlight
    //    backlightControl(1);

    //  Turn OFF Motors
    P6SEL0 &= ~R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6OUT  &= ~R_FORWARD;
    P6DIR  &= ~R_FORWARD;

    P6SEL0 &= ~L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6OUT  &= ~L_FORWARD;
    P6DIR  &= ~L_FORWARD;
}

void LmotorStop(void){
    //  Select and Turn On Backlight
    //    backlightControl(1);

    //  Turn OFF Left Motor
    P6SEL0 &= ~L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6OUT  &= ~L_FORWARD;
    P6DIR  &= ~L_FORWARD;
}

void RmotorStop(void){
    //  Select and Turn On Backlight
    //    backlightControl(1);

    //  Turn OFF Right Motors
    P6SEL0 &= ~R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6OUT  &= ~R_FORWARD;
    P6DIR  &= ~R_FORWARD;
}



// Movement Cases
void Move_Shape(void){
    switch(state){
    case  WAIT:
        wait_case();
        // Begin
        break;
    case  START:
        start_case();
        break;
    case  RUN:
        // Run actual code to make Shape
        switch(event){
        case STRAIGHT:
            straight();
            break;
        case CIRCLE:
            circle();
            break;
        case TRIANGLE:
            triangle();
            break;
        case FIGURE8:
            figure8();
            break;
        case NONE:
            break;
        default:
            break;
        }
        break;
        case  END:
            end_case();
            break;
        default: break;
    }
}

void wait_case(void){
    if(time_change){
        time_change = 0;
        if(delay_start++ >= WAITING2START){
            delay_start = 0;
            state = START;
        }
    }
}

void start_case(void){
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    LRmotorForward();
    segment_count = 0;
    state = RUN;
}

void run_case(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= travel_distance){
            if(right_motor_count++ >= right_count_time){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= left_count_time){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time >= wheel_count_time){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                LRmotorForward();
            }
        }else{
            state = END;
        }
    }
}

void end_case(void){
    LRmotorStop();
    state = WAIT;
    event = NONE;
}



// Shape Commands
// Straight:
void straight(void){
    travel_distance = 2;
    right_count_time = 5;
    left_count_time = 5;
    wheel_count_time = 10;
    run_case();
}

// Circle:
//    For Right Turn, LEFT_COUNT_TIME > RIGHT_COUNT_TIME
//    For Left Turn, RIGHT_COUNT_TIME > LEFT_COUNT_TIME
void circle(void){
    travel_distance = 2;
    right_count_time = 3;
    left_count_time = 7;
    wheel_count_time = 10;
    run_case();
}


void triangle(void){
    unsigned int i;
    for(i = 3; i > 0; i--){
        // Triangle Straight
        travel_distance = 2;
        right_count_time = 5;
        left_count_time = 5;
        wheel_count_time = 10;
        run_case();

        // Triangle  Turn
        travel_distance = 2;
        right_count_time = 7;
        left_count_time = 0;
        wheel_count_time = 10;
        run_case();
    }
}


void figure8(void){
    //    Right Circle
    travel_distance = 2;
    right_count_time = 7;
    left_count_time = 5;
    wheel_count_time = 10;
    run_case();

    //    Left Circle
    travel_distance = 2;
    right_count_time = 5;
    left_count_time = 7;
    wheel_count_time = 10;
    run_case();
}
