#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "reset.h"

void Reset(void);

void both_forward(void);


extern volatile unsigned int requested_move;

// Globals as required
void StateMachine(void){
    static int number = 1;
//------------------------------------------------------
 switch(requested_move){
     case STILL: // IDLE
         Reset();
         requested_move = MOVE_FORWARD;
         //break;
     case MOVE_FORWARD: // Move Forward Start

         both_forward();

         //requested_move = TURN;
         break;
     case TURN: // Adjust Forward
         if (ADC_Left_Detect == 7 || ADC_Right_Detect == 7 ){

         }
         requested_move = STOP;
         //break;
     case STOP: // Adjust Forward
         RIGHT_FORWARD_SPEED = WHEEL_OFF;
         LEFT_FORWARD_SPEED = WHEEL_OFF;
         //break;
         default:
       break;
 }
}

void both_forward(void){
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;

    RIGHT_FORWARD_SPEED = SLOW;
    LEFT_FORWARD_SPEED = SLOW;
}
//-----------------------------------------------------

