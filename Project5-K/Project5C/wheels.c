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





volatile unsigned int state;   // To manage the state machine
volatile unsigned char event;  // Event variable from switches.c (e.g., STRAIGHT, CIRCLE)
extern volatile unsigned int event_Counter; //picking the event based on increment

extern unsigned int Last_Time_Sequence;  // To track changes in Time_Sequence
extern unsigned int cycle_time;          // Controls shape timings
extern unsigned int time_change;         // Flag to detect time sequence change

unsigned int delay_start;
unsigned int left_motor_count;
unsigned int right_motor_count;
unsigned int segment_count;

unsigned int wheel_Counttime;
unsigned int right_Counttime;
unsigned int left_Counttime;
unsigned int travel_Distance;
unsigned int waitingtostart;
extern volatile unsigned int triangleset = 0;
extern volatile unsigned int figureeightset = 0;
extern volatile unsigned int rdirection = 0;
extern volatile unsigned int ldirection = 0;
extern volatile unsigned int instruction;
volatile unsigned char origevent;

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;

void Wheel_Move(void) {
// This is going to be moving the car
switch(event){
                case STRAIGHT: // Straight
                    wheel_Counttime = 10;
                    right_Counttime = 7;
                    left_Counttime = 8;
                    if(!instruction){
                        travel_Distance = 3;
                    }

                Run_Shape();
                break;
                case CIRCLE: // Circle
                    wheel_Counttime = 18;
                    right_Counttime = 1;
                    left_Counttime = 20;
                    travel_Distance = 75;

                    Run_Shape();
                break;
                case TRIANGLE: //THIS IS THE 30 DEGREE TURN
                    wheel_Counttime = 15;
                    right_Counttime = 0;
                    left_Counttime = 9;
                    travel_Distance = 11;

                    Run_Shape();

                break;
                case TRIANGLES:
                    wheel_Counttime = 11;
                    right_Counttime = 11;
                    left_Counttime = 9;
                    travel_Distance = 10;

                    Run_Shape();
                break;
                case FIGUREEIGHT:
                    wheel_Counttime = 18;
                    right_Counttime = 0;
                    left_Counttime = 20;
                    if(figureeightset == 3){
                        travel_Distance = 21;
                    }
                    else if(figureeightset == 5){
                        travel_Distance = 21;
                    }
                    else{
                        travel_Distance = 11;
                    }

                    Run_Shape();
                    break;
                case FIGUREEIGHTT:
                    wheel_Counttime = 18;
                    right_Counttime = 20;
                    left_Counttime = 0;
                    if(figureeightset == 4){
                        travel_Distance = 23;
                    }
                    else{
                        travel_Distance = 22;
                    }

                    Run_Shape();
                break;
                case PROJECTFIVE:
                    wheel_Counttime = 10;
                    right_Counttime = 7;
                    left_Counttime = 8;
                    if(!instruction){
                        travel_Distance = 10;
                    }
                    Run_Shape();
                    break;
                case SPINNING:
                wheel_Counttime = 10;
                if(!instruction){
                    travel_Distance = 10;
                    right_Counttime = 7;
                    left_Counttime = 8;
                }
                Run_Shape();

                default: break;
      }
}

void Run_Shape(void) {
          switch(state){
              case WAIT: // Begin
                  wait_case();
                  break;
              case START: // Begin
                  start_case();
                  break;
              case RUN: // Run
                  run_case();
                  break;
              case END: // End
                  end_case();
                  break; //
              default: break;
          }


      }
void Forward_On(void) {
    P6OUT |= L_FORWARD;   // Turn on the left motor (set pin high)
    P6OUT |= R_FORWARD;
}

void Forward_Off(void){
    P6OUT &= ~L_FORWARD;  // Turn off the left motor (set pin low)
    P6OUT &= ~R_FORWARD;  // Turn off the right motor (set pin low)
}
void Reverse_On(void) {
    P6OUT |= L_REVERSE;   // Turn on the left motor (set pin high)
    P6OUT |= R_REVERSE;
}
void Reverse_Off(void){
    P6OUT &= ~L_REVERSE;  // Turn off the left motor (set pin low)
    P6OUT &= ~R_REVERSE;  // Turn off the right motor (set pin low)
}

void Forward_Move(void) {
    Forward_On();
}
void Reverse_Move(void) {
    Reverse_On();
}


//The first state “WAIT” allows for the button to be pressed and time to move out of the way.
void wait_case(void){
    if(time_change){
        time_change = 0;
        if(!instruction){
            waitingtostart = WAITING2START;
        }
        if(delay_start++ >= waitingtostart){
            delay_start = 0;
            state = START;
        }
    }
}

//The second state “START” sets the initial condition.
void start_case(void){
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    segment_count = 0;
    state = RUN;
}

//The third state “RUN” controls the movement.
void run_case(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= travel_Distance){
            if(right_motor_count++ >= right_Counttime){
                if(rdirection == FORWARD){
                    P6OUT &= ~R_FORWARD; //stop right motor
                }
                else {
                    P6OUT &= ~R_REVERSE; //stop right motor
                }
            }
            if(left_motor_count++ >= left_Counttime){
                if(ldirection == FORWARD){
                    P6OUT &= ~L_FORWARD; //stop left motor
                }
                else {
                    P6OUT &= ~L_REVERSE; //stop left motor
                }

            }
            if(cycle_time >= wheel_Counttime){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                if(rdirection == FORWARD || ldirection == FORWARD){
                    Forward_Move();  // Trigger forward movement
                }
                else if(rdirection == REVERSE || ldirection == REVERSE){
                    Reverse_Move();
                }
            }
        }else{
            state = END;
        }
    }
}









//The last state “END” clears the state machine back to NONE so it ends and movement over.
void end_case(void){
    Reverse_Off();
    Forward_Off();
    state = WAIT;
    switch (triangleset){
    case 0:
        if(figureeightset){
          event = NONE;
        }
        break;
    case 1:
        event = TRIANGLES;
        triangleset++;
        break;
    case 2:
        event = TRIANGLE;
        triangleset++;
        break;
    case 3:
        event = TRIANGLES;
        triangleset++;
        break;
    case 4:
        event = TRIANGLE;
        triangleset++;
        break;
    case 5:
        event = TRIANGLES;
        triangleset++;
        break;
    case 6:
        event = TRIANGLE;
        triangleset++;
        break;
    case 7:
        event = TRIANGLES;
        triangleset++;
        break;
    case 8:
        event = TRIANGLE;
        triangleset++;
        break;
    case 9:
        event = TRIANGLES;
        triangleset++;
        break;
    case 10:
        event = TRIANGLE;
        triangleset++;
        break;
    case 11:
        event = TRIANGLES;
        triangleset++;
        break;
    case 12:
        event = TRIANGLE;
        triangleset++;
        break;
    case 13:
        event = NONE;
        triangleset = 0;
        break;
    default: break;
    }
    switch (figureeightset){
        case 0:
            if(!triangleset){
              event = NONE;
            }
            break;
        case 1:
            event = FIGUREEIGHTT;
            figureeightset++;
            break;
        case 2:
            event = FIGUREEIGHT;
            figureeightset++;
            break;
        case 3:
            event = FIGUREEIGHTT;
            figureeightset++;
            break;
        case 4:
            event = FIGUREEIGHT;
            figureeightset++;
        break;
        case 5:
            event = NONE;
            figureeightset = 0;
            break;
        default: break;
        if(origevent == PROJECTFIVE){
            projectfive();
        }
        }


}

void projectfive(void){
    switch(instruction -1){
    case 0:
        rdirection = REVERSE;
        ldirection = REVERSE;
        travel_Distance = 10;
        waitingtostart = 50;
        instruction++;
        event = PROJECTFIVE;
        break;
    case 1:
        rdirection = FORWARD;
        ldirection = FORWARD;
        travel_Distance = 10;
        waitingtostart = 50;
        instruction++;
        event = PROJECTFIVE;
        break;
    case 2:
        rdirection = FORWARD;
        ldirection = REVERSE;
        travel_Distance = 10;
        right_Counttime = 7;
        left_Counttime = 8;
        instruction++;
        event = SPINNING;
        break;
    case 3:
        rdirection = REVERSE;
        ldirection = FORWARD;
        travel_Distance = 10;
        right_Counttime = 7;
        left_Counttime = 8;
        instruction++;
        event = SPINNING;
        break;
    case 4:
        event = NONE;
        instruction = 0;
        break;
    default: break;
    }
}

void motorDirec(void){
    if(((P6IN & L_FORWARD)&&(P6IN & L_REVERSE))||((P6IN & R_FORWARD)&&(P6IN & R_REVERSE))){
        P1OUT |= RED_LED;
        Forward_Off();
        Reverse_Off();
    }

}


void Display_Changing(void){
        strcpy(display_line[0], "          ");
        strcpy(display_line[1], "          ");

        char newlinner[11]; // 10 characters
        char newlin[11]; // 10 characters
        char dire = "_";
        if(rdirection == FORWARD && ldirection == REVERSE){
            dire = "C";
        }
        else if(rdirection == REVERSE && ldirection == FORWARD){
            dire = ")";
        }
        sprintf(newlinner, "     %c     ", event); // Fill with 5 spaces, the event character, and 5 more spaces
        strncpy(display_line[2], newlinner, 10); // Copy exactly 10 characters into display_line[2]


        sprintf(newlin, "     %c     ", dire); // Fill with 5 spaces, the event character, and 5 more spaces
        strncpy(display_line[3], newlin, 10); // Copy exactly 10 characters into display_line[2]

        display_changed = TRUE;

}

