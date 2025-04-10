/*
 * shapes.c
 *
 *  Created on: Feb 11, 2025
 *      Author: price
 */
// STATES ======================================================================

#include  "msp430.h"
#include  <string.h>
#include  "include\functions.h"
#include  "Include\LCD.h"
#include  "include\ports.h"
#include "macros.h"
#include "include\shapes.h"


unsigned int left_motor_count;
unsigned int right_motor_count;
unsigned int segment_count;
unsigned int time_change;
unsigned int cycle_time;
unsigned int delay_start;
unsigned int event;
unsigned char state = WAIT;
unsigned int circle_completed;
extern unsigned int start_pressed;
unsigned int tri_count;
extern unsigned int Motor_Count;

//DONT CHANGE
void Run_Circle(void){
    switch(state){
    case WAIT: // Begin
        wait_case();
        break;
    case START: // Begin
        start_case();
        break;
    case RUN: // Run
        run_circle_case();
        break;
    case END: // End
        end_case();
        break; //
    default: break;
 }
}
//DONT CHANGE
void run_circle_case (void){
    if(time_change){
            time_change = 0;
        if(segment_count <= (TRAVEL_DISTANCE_CIRCLE *2)){
             if(right_motor_count++ >= RIGHT_COUNT_TIME_CIRCLE){
                P6OUT &= ~R_FORWARD;
             }
             if(left_motor_count++ >= LEFT_COUNT_TIME_CIRCLE){
                 P6OUT &= ~L_FORWARD;
             }
             if(cycle_time >= WHEEL_COUNT_TIME_CIRCLE){ //DONT CHANGE
                 cycle_time = 0;
                 right_motor_count = 0;  //how long its on in the cycle
                 left_motor_count = 0;
                 segment_count++;
                 wheels_forward();
             }
         }else{
             state = END;
         }
        }
}
//DONT CHANGE
void Run_Figure8(void){
    switch(state){
    case WAIT: // Begin
        wait_case();
        break;
    case START: // Begin
        start_case();
        break;
    case RUN: // Run
        run_figure8_case();
        break;
    case END: // End
        end_case();
        break; //
    default: break;
 }
}
//DONT CHANGE
void run_figure8_case (void){
    if(time_change){
         time_change = 0;
         // CIRCLE ONE
         if (circle_completed == 0) {

            if(segment_count <= (TRAVEL_DISTANCE_FIGURE8_1)){
                 if(right_motor_count++ >= RIGHT_COUNT_TIME_FIGURE8_1){
                    P6OUT &= ~R_FORWARD;
                 }
                 if(left_motor_count++ >= LEFT_COUNT_TIME_FIGURE8_1){
                     P6OUT &= ~L_FORWARD;
                 }
                 if(cycle_time >= WHEEL_COUNT_TIME_FIGURE8){ //DONT CHANGE
                     cycle_time = 0;
                     right_motor_count = 0;  //how long its on in the cycle
                     left_motor_count = 0;
                     segment_count++;
                     wheels_forward();
                 }
             }
                 else {
                     circle_completed ++;
                     state = START;
                     segment_count = 0;
                 }
         }
            //CIRCLE TWO
         if (circle_completed == 1) {
            if(segment_count <= (TRAVEL_DISTANCE_FIGURE8_2)){
                 if(right_motor_count++ >= RIGHT_COUNT_TIME_FIGURE8_2){
                    P6OUT &= ~R_FORWARD;
                 }
                 if(left_motor_count++ >= LEFT_COUNT_TIME_FIGURE8_2){
                     P6OUT &= ~L_FORWARD;
                 }
                 if(cycle_time >= WHEEL_COUNT_TIME_FIGURE8){ //DONT CHANGE
                     cycle_time = 0;
                     right_motor_count = 0;  //how long its on in the cycle
                     left_motor_count = 0;
                     segment_count++;
                     wheels_forward();
                 }
             }
            else{
                if (circle_completed == 3) {
                    circle_completed = 0;
                    state = END;
                }
                else {
                circle_completed++;
                segment_count = 0;
                }
             }
         }

         //CIRCLE THREE
         if (circle_completed == 2) {

            if(segment_count <= (TRAVEL_DISTANCE_FIGURE8_3)){
                 if(right_motor_count++ >= RIGHT_COUNT_TIME_FIGURE8_3){
                    P6OUT &= ~R_FORWARD;
                 }
                 if(left_motor_count++ >= LEFT_COUNT_TIME_FIGURE8_3){
                     P6OUT &= ~L_FORWARD;
                 }
                 if(cycle_time >= WHEEL_COUNT_TIME_FIGURE8){ //DONT CHANGE
                     cycle_time = 0;
                     right_motor_count = 0;  //how long its on in the cycle
                     left_motor_count = 0;
                     segment_count++;
                     wheels_forward();
                 }
             }
                 else {
                     circle_completed ++;
                     state = START;
                     segment_count = 0;
                 }
         }
         //CIRCLE FOUR
         if (circle_completed == 3) {
            if(segment_count <= (TRAVEL_DISTANCE_FIGURE8_4)){
                 if(right_motor_count++ >= RIGHT_COUNT_TIME_FIGURE8_4){
                    P6OUT &= ~R_FORWARD;
                 }
                 if(left_motor_count++ >= LEFT_COUNT_TIME_FIGURE8_4){
                     P6OUT &= ~L_FORWARD;
                 }
                 if(cycle_time >= WHEEL_COUNT_TIME_FIGURE8){ //DONT CHANGE
                     cycle_time = 0;
                     right_motor_count = 0;  //how long its on in the cycle
                     left_motor_count = 0;
                     segment_count++;
                     wheels_forward();
                 }
             }
            else{
                if (circle_completed == 3) {
                    circle_completed = 0;
                    state = END;
                }
             }
         }
    }
}

void Run_Triangle(void){
    switch(state){
    case WAIT: // Begin
        wait_case();
        break;
    case START: // Begin
        start_case();
        break;
    case RUN: // Run
        run_triangle_case();
        break;
    case END: // End
        end_case();
        break; //
    default: break;
 }
}

void run_triangle_case(void) {
    if (tri_count == 0){
    run_straight_case();
    }

    else if (tri_count == 1){
    run_pivot_case();
    }

    else if (tri_count == 2){
    run_straight_case();
    }

    else if (tri_count == 3){
    run_pivot_case();
    }

    else if (tri_count == 4){
    run_straight_case();
    }

    else if (tri_count == 5){
    run_pivot_case();
    }

    else if (tri_count == 6){
    run_straight_case();
    }

    else if (tri_count == 7){
    run_pivot_case();
    }

    else if (tri_count == 8){
    run_straight_case();
    }

    else if (tri_count == 9){
    run_pivot_case();
    }

    else if (tri_count == 10){
    run_straight_case();
    }

    else if (tri_count == 11){
    run_pivot_case();
    }

    else if (tri_count == 12){
    run_straight_case();
    }

    else
        state = END;
}

//DONT CHANGE
void run_straight_case(void){
    if(time_change){
        time_change = 0;
    if(segment_count <= TRAVEL_DISTANCE_TRIANGLE){
         if(right_motor_count++ >= RIGHT_COUNT_TIME_TRIANGLE){
            P6OUT &= ~R_FORWARD;
         }
         if(left_motor_count++ >= LEFT_COUNT_TIME_TRIANGLE){
             P6OUT &= ~L_FORWARD;
         }
         if(cycle_time >= WHEEL_COUNT_TIME_TRIANGLE){
             cycle_time = 0;
             right_motor_count = 0;
             left_motor_count = 0;
             segment_count++;
             wheels_forward();
         }
     }
    else{
         tri_count++;
         segment_count = 0;
     }
    }
}

void run_pivot_case(void){
    if(time_change){
        time_change = 0;
    if(segment_count <= TRAVEL_DISTANCE_PIVOT){
         if(right_motor_count++ >= RIGHT_COUNT_TIME_PIVOT){
            P6OUT &= ~R_FORWARD;
         }
         if(left_motor_count++ >= LEFT_COUNT_TIME_PIVOT){
             P6OUT &= ~L_FORWARD;
         }
         if(cycle_time >= WHEEL_COUNT_TIME_PIVOT){
             cycle_time = 0;
             right_motor_count = 0;
             left_motor_count = 0;
             segment_count++;
             wheels_forward();
         }
     }
    else{
        tri_count++;
        segment_count = 0;
     }
    }
}


//DONT CHANGE
void wait_case(void){
    if(time_change){
        time_change = 0;
        if(delay_start++ >= WAITING2START){
            delay_start = 0;
            state = START;
        }
    }
}
//DONT CHANGE
void start_case(void){
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    wheels_forward();
    segment_count = 0;
    state = RUN;
}

//DONT CHANGE
void end_case(void){
    wheels_off();
    state = WAIT;
    event = NONE;
    start_pressed = 0;
}

void Run_Straight(void){
    switch(state){
    case WAIT: // Begin
        wait_case();
        break; //
    case START: // Begin
        start_case();
        break; //
    case RUN: // Run
        run_case();
        break; //
    case END: // End
        end_case();
        break; //
    default: break;
 }
}


void PRJ5_Sequence(){
    switch(Motor_Count){
        case 1:
            Display_Forward();
            wheels_forward();
            break;
        case 200: // CCR1 not used
            Display_Wait();
            wheels_off();
            break;
        case 400: // CCR1 not used
            Display_Reverse();
            wheels_reverse();
            break;
        case 800: // CCR2 not used
            Display_Wait();
            wheels_off();
            break;
        case 1000: // CCR1 not used
            Display_Forward();
            wheels_forward();
            break;
        case 1200: // CCR1 not used
            Display_Wait();
            wheels_off();
            break;
        case 1400: // CCR1 not used
            Display_CCW();
            wheels_CCW();
            break;
        case 2000: // CCR1 not used
            Display_Wait();
            wheels_off();
            break;
        case 2400: // CCR1 not used
            Display_CW();
            wheels_CW();
            break;
        case 3000: // overflow
            Display_Wait();
            wheels_off();
            break;
        default: break;
    }
}

