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

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

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


// Shape Commands
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


// Circle:
//    For Right Turn, LEFT_COUNT_TIME > RIGHT_COUNT_TIME
//    For Left Turn, RIGHT_COUNT_TIME > LEFT_COUNT_TIME
void circle(void){
    TRAVEL_DISTANCE = 2;
    RIGHT_COUNT_TIME = 3;
    LEFT_COUNT_TIME = 7;
    WHEEL_COUNT_TIME = 10;
    run_case();
}


void triangle(void){
    for(i = 0; i < 3; i++){
        // Triangle Straight
        TRAVEL_DISTANCE = 2;
        RIGHT_COUNT_TIME = 5;
        LEFT_COUNT_TIME = 5;
        WHEEL_COUNT_TIME = 10;
        run_case();

        // Triangle  Turn
        TRAVEL_DISTANCE = 2;
        RIGHT_COUNT_TIME = 7;
        LEFT_COUNT_TIME = 0;
        WHEEL_COUNT_TIME = 10;
        run_case();
    }
}


void figure8(void){
    //    Right Circle
    TRAVEL_DISTANCE = 2;
    RIGHT_COUNT_TIME = 7;
    LEFT_COUNT_TIME = 0;
    WHEEL_COUNT_TIME = 10;
    run_case();

    //    Left Circle
    TRAVEL_DISTANCE = 2;
    RIGHT_COUNT_TIME = 7;
    LEFT_COUNT_TIME = 0;
    WHEEL_COUNT_TIME = 10;
    run_case();
}
