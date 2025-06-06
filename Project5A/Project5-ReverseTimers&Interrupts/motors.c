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
extern unsigned int backlight_status;

extern unsigned int straight_step;
extern unsigned int circle_step;
extern unsigned int triangle_step;
extern unsigned int figure8_step;

// Globals
extern unsigned char dispEvent;
extern volatile unsigned char display_changed;
extern unsigned char event;
extern char display_line[4][11];


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
    P6OUT  |=  L_REVERSE;
}
void RightRevOFF(void){
    P6OUT  &= ~L_REVERSE;
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
    LRFwdOff();
    LRRevOff();
}


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




// STOP Commands
void LRmotorStop(void){
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
            travel_distance = 10;
            right_count_time = 10;
            left_count_time = 9;
            wheel_count_time = 10;
            run_case();
            break;
        case CIRCLE:
            travel_distance = 190; // 85 Best so Far, Little Under //175
            right_count_time = 1;
            left_count_time = 10;
            wheel_count_time = 10;
            run_case();
            break;
        case TRIANGLE:
            travel_distance = 10;
            right_count_time = 10;
            left_count_time = 10;
            wheel_count_time = 10;
            run_case();
            break;
        case TRIANGLE_CURVE:
            travel_distance = 15;
            right_count_time = 0;
            left_count_time = 10;
            wheel_count_time = 10;
            run_case();
            break; // NEW??
        case FIGURE8C1:
            travel_distance = 105; // 75 Best so Far, Little Under
            right_count_time = 10;
            left_count_time = 0;
            wheel_count_time = 10;
            run_case();
            break;
        case FIGURE8C2:
            travel_distance = 120; // 75 Best so Far, Little Under
            right_count_time = 0;
            left_count_time = 10;
            wheel_count_time = 10;
            run_case();
            break;
        case NONE:
            LRmotorStop();
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
    segment_count = 0;
    LRmotorForward();
    state = RUN;
}



void end_case(void){
    LRmotorStop();
    state = WAIT;
    switch(event){
    case FIGURE8C1:
        if(figure8_step==0){
            event = NONE;
        }else if(figure8_step==1){
            event = FIGURE8C2;
            figure8_step++;

        }else if(figure8_step==2){
            event = NONE;
            figure8_step = 0;
        }
        break;

    case TRIANGLE:
        if(triangle_step==0){
            event = NONE;
        }else if(triangle_step==12){
            event = NONE;
            strcpy(display_line[0], "   NCSU   ");
            strcpy(display_line[1], " WOLFPACK ");
            strcpy(display_line[2], "  ECE306  ");
            strcpy(display_line[3], "  GP I/O  ");
            display_changed = TRUE;
            backlight_status = 1;
            triangle_step = 0;
        }else if(triangle_step % 2){// Even
            event = TRIANGLE_CURVE;
            triangle_step++;
        }else{// Odd
            event = TRIANGLE;
            triangle_step++;
        }
        break;

    case TRIANGLE_CURVE:
        if(triangle_step==0){
            event = NONE;
        }else if(triangle_step==12){
            event = NONE;
            strcpy(display_line[0], "   NCSU   ");
            strcpy(display_line[1], " WOLFPACK ");
            strcpy(display_line[2], "  ECE306  ");
            strcpy(display_line[3], "  GP I/O  ");
            display_changed = TRUE;
            backlight_status = 1;
            triangle_step = 0;
        }else if(triangle_step % 2){// Even
            event = TRIANGLE_CURVE;
            triangle_step++;
        }else{// Odd
            event = TRIANGLE;
            triangle_step++;
        }
        break;

    default:
        event = NONE;
        strcpy(display_line[0], "   NCSU   ");
        strcpy(display_line[1], " WOLFPACK ");
        strcpy(display_line[2], "  ECE306  ");
        strcpy(display_line[3], "  GP I/O  ");
        display_changed = TRUE;
        backlight_status = 1;
        break;
    }
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
        }
        else{
            state = END;
        }
    }
}

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
