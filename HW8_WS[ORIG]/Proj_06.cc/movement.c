//------------------------------------------------------------------------------
//  Description: Runs car straight
//
//  Corinne McGuire
//  Date: 9/30 Fall 2024
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

// Run time is the overall running time, based off of the value that wheeltime counts to
// if roff is 10 and wheel time is 14, right wheel is on for 10 of the 14 counts
// turning the wheels off is how you control speed, off for longer, less speed. Use that to turn.
// With wheel time as 14, that means that if run time is 20, wheeltime must count up to 14, 20 times before the movement ends


#include "functions.h"
#include "LCD.h"
#include "macros.h"
#include "ports.h"
#include  "msp430.h"
#include  <string.h>

volatile char slow_input_down;
unsigned char display_mode;
unsigned int test_value;
char chosen_direction;
char change;
unsigned int wheel_move;
char forward;
char start_moving;
unsigned char moving;
int Time_Increment;
volatile unsigned int Last_Time_Sequence;                                    // a variable to identify Time_Sequence has changed
unsigned int cycle_time;                                             // is a new time base used to control making shapes
unsigned int time_change;                                            // is an identifier that a change has occurred
int delay_start = 0;
int right_motor_count = 0;
int left_motor_count = 0;
int segment_count = 0;
volatile int Time_Delay = 0;
volatile char event = NONE;
volatile char state = NONE;


void Run_Straight(int runtime, int loff, int roff, int wheeltime){
 switch(state){
 case WAIT: // Begin
 wait_case();
 break; //
 case START: // Begin
 start_case();
 break; //
 case RUN: // Run
 run_case(runtime, loff, roff, wheeltime);
 break; //
 case END: // End
 end_case();
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
 Forward_On();
 segment_count = 0;
 state = RUN;
}

void end_case(void){
 Forward_Off();
 state = WAIT;
 event = NONE;
 cycle_time = 0;
 finished = 1;
}

void run_case(int runtime, int loff, int roff, int wheeltime){
    if(event == STRAIGHT){
      if(time_change){
        time_change = 0;
        if(segment_count <= runtime){
          if(right_motor_count++ >= loff){
            P6OUT &= ~R_FORWARD;
          }
          if(left_motor_count++ >= roff){
            P6OUT &= ~L_FORWARD;
          }
          if(cycle_time >= wheeltime){
            cycle_time = 0;
            right_motor_count = 0;
            left_motor_count = 0;
            segment_count++;
            Forward_On();
          }
        }
        else{
           state = END;
        }
     }
   }
}
