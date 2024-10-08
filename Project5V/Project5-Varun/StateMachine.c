/*
 * StateMachine.c
 *
 *  Created on: Jan 30, 2024
 *      Author: varun
 */
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"


volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern unsigned int test_value;
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;
char chosen_direction;
char change;

unsigned int state;
unsigned int event;
unsigned int delay_start;
unsigned int right_motor_count;
unsigned int left_motor_count;
unsigned int segment_count;
unsigned int cir_count;
unsigned int tri_count;

void Carlson_StateMachine(void);
void Run_Straight(void);
void wait_case(void);
void run_case(void);
void end_case(void);
void start_case(void);
void run_case_circle_counterclkw(void);
void run_case_circle_clkw(void);
void run_case_triangle_turn(void);
void Run_Figure_Eight(void);
void Run_Triangle(void);

void Wheels_Stop(void);
void Wheels_Forward(void);


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
//            event = CIRCLE;
            break; //
        default: break;
 }
}


void Run_Circle(void){
    switch(state){
        case WAIT: // Begin
            wait_case();
            break; //
        case START: // Begin
            start_case();
            break; //
        case RUN: // Run
            run_case_circle_counterclkw();
            break; //
        case END: // End
            end_case();
            break; //
        default: break;
 }
}


void Run_Figure_Eight(void){
    switch(state){
        case WAIT:
            wait_case();
            break;
        case START:
            start_case();
            break;
        case RUN:
            if (cir_count == 0){
                run_case_circle_counterclkw();
                break;
            }
            else{
                run_case_circle_clkw();
                break;
            }
            break;
        case END:
            if (cir_count == 0){
                state = START;
                cir_count++;
                break;
            }
            else{
                end_case();
                cir_count = 0;
                break;
            }
            break;
        default: break;
    }
}


void Run_Triangle(void){
    switch(state){
            case WAIT: // Begin
                wait_case();
                break; //
            case START: // Begin
                start_case();
                break; //
            case RUN: // Run
                if (tri_count%2 == 0){
                    run_case();
                     //
                }
                else{

                    run_case_triangle_turn();
                     //
                }
                break;
            case END: // End
                if (tri_count == 9){
                    end_case();
                    tri_count = 0;
                }
                else{
                    state = START;
                    tri_count++;
                }
                break; //
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
    Wheels_Forward();
    segment_count = 0;
    state = RUN;
}

void run_case(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= TRAVEL_DISTANCE){
            if(right_motor_count++ >= RIGHT_COUNT_TIME){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= LEFT_COUNT_TIME){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time >= WHEEL_COUNT_TIME){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Wheels_Forward();
            }
        }else{
            state = END;
        }
    }
}

void run_case_circle_counterclkw(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= CIRCLE_TRAVEL_DISTANCE){
            if(right_motor_count++ >= COUNTERCLKW_RIGHT_COUNT_TIME){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= COUNTERCLKW_LEFT_COUNT_TIME){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time >= WHEEL_COUNT_TIME){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Wheels_Forward();
            }
        }else{
      state = END;
        }
    }
}

void run_case_circle_clkw(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= CIRCLE_TRAVEL_DISTANCE){
            if(right_motor_count++ >= CLKW_RIGHT_COUNT_TIME){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= CLKW_LEFT_COUNT_TIME){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time >= WHEEL_COUNT_TIME){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Wheels_Forward();
            }
        }else{
      state = END;
        }
    }
}

void run_case_triangle_turn(void){
    if(time_change){
            time_change = 0;
            if(segment_count <= TRIANGLE_TURN_DISTANCE){
                if(right_motor_count++ >= TRITURN_RIGHT_COUNT_TIME){
                    P6OUT &= ~R_FORWARD;
                }
                if(left_motor_count++ >= TRITURN_LEFT_COUNT_TIME){
                    P6OUT &= ~L_FORWARD;
                }
                if(cycle_time >= WHEEL_COUNT_TIME){
                    cycle_time = 0;
                    right_motor_count = 0;
                    left_motor_count = 0;
                    segment_count++;
                    Wheels_Forward();
                }
            }else{
          state = END;
            }

    }
}

void end_case(void){
    Wheels_Stop();
    state = WAIT;
    event = NONE;
}


