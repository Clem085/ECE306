/*
 * wheels.c
 *
 *  Created on: Feb 4, 2025
 *      Author: price
 */

#include  "msp430.h"
#include  <string.h>
#include  "include\functions.h"
#include  "Include\LCD.h"
#include  "include\ports.h"
#include "macros.h"
#include "include\shapes.h"

extern unsigned char display_changed;
extern char display_line[4][11];

unsigned char requested_move = IDLE;
extern unsigned int time_change;
extern unsigned int delay_start;
extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern volatile unsigned int Time_Sequence;

void wheels_off (void){
    P6OUT &= ~L_FORWARD; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_FORWARD; // Set Port pin Low [Wheel Off]
    P6OUT &= ~L_REVERSE; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_REVERSE; // Set Port pin Low [Wheel Off]
}

void wheels_forward (void){
    P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
    P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]
    P6OUT &= ~L_REVERSE; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_REVERSE; // Set Port pin Low [Wheel Off]
}

void wheels_CCW (void){
    P6OUT &= ~L_FORWARD; // Set Port pin High [Wheel On]
    P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]
    P6OUT &= ~L_REVERSE; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_REVERSE; // Set Port pin Low [Wheel Off]
}

void wheels_CW (void){
    P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
    P6OUT &= ~R_FORWARD; // Set Port pin High [Wheel On]
    P6OUT &= ~L_REVERSE; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_REVERSE; // Set Port pin Low [Wheel Off]
}

//R_FORWARD off - 5.25 V TP1 - 5.26 V TP3
//L_FORWARD off - 5.25 V TP5 - 5.25 V TP7
//R_REVERSE on - 1.2 V TP2 - 0 V TP4
//L_REVERSE on - 1.21 V TP6 - 0 V TP8

//R_FORWARD on - 1.2 V TP1 - 0 V TP3
//L_FORWARD on - 1.2 V TP5 - 0 V TP7
//R_REVERSE off - 5.2 V TP2 - 5.2 V TP4
//L_REVERSE off - 5.2 V TP6 - 5.2 V TP8

void wheels_reverse (void){
    P6OUT &= ~L_FORWARD; // Set Port pin High [Wheel On]
    P6OUT &= ~R_FORWARD; // Set Port pin High [Wheel On]
    P6OUT |= L_REVERSE; // Set Port pin Low [Wheel Off]
    P6OUT |= R_REVERSE; // Set Port pin Low [Wheel Off]
}

void Wheels_Process(void){
//------------------------------------------------------
 switch(requested_move){
     case IDLE: // IDLE
         Idle_case();
         break;
//     case Configure_Wheel_Speeds: // Configure Movement
//         Configure_Wheel_Speeds_case();
//         break;
     case Forward_Move_Start: // Move Forward Start
         Forward_Move_Start_case();
         break;
     case Wait:
         Wait_case();
         break;
     case Readjust:
         Readjust_case();
         break;
//   case Forward_Adjust: // Adjust Forward
//         Forward_Adjust_case();
//         break;
//     case Reverse_Move_Start: // Move Reverse Start
//         Reverse_Move_Start_case();
//         break;
//     case Reverse_Adjust : // Adjust Reverse
//         Reverse_Adjust_case();
//         break;
//     case Initiate_Stop: // Begin Stop Process
//         Initiate_Stop_case();
//         break;
     case Stop: // Look for End of Stop Time
         Stop_case();
         break;
    default: break;
     }
    }

void Idle_case(void){
    if (time_change < 60){
        strcpy(display_line[3], "  WAITING ");
        display_changed = TRUE;

        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;
    } else if (time_change >= 60){
        time_change = 0;
        requested_move = Forward_Move_Start;
    }
}

void Forward_Move_Start_case(void){
    if ((ADC_Right_Detect < 69) || (ADC_Left_Detect < 69)){
        strcpy(display_line[3], "  SEARCHING ");
        display_changed = TRUE;

        RIGHT_FORWARD_SPEED = SLOWER;
        LEFT_FORWARD_SPEED = SLOWER;
    }
    else {
        strcpy(display_line[3], "  FOUND    ");
        display_changed = TRUE;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        requested_move = Wait;
            }
        }

void Wait_case(void){
    if (time_change < 177){
        strcpy(display_line[3], "  WAITING ");
        display_changed = TRUE;

        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;
    }
    else if (time_change >= 177){
        time_change = 0;
        requested_move = Stop;
        time_change = 0;
        requested_move = Readjust;
    }
}

void Readjust_case(void){
    if ((140 < ADC_Right_Detect < 160) && (140 < ADC_Left_Detect < 160)){
        strcpy(display_line[3], "  TURNING  ");
        display_changed = TRUE;

        RIGHT_FORWARD_SPEED = SLOWER;
        LEFT_REVERSE_SPEED = SLOWER;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
     }
    if ((ADC_Right_Detect >=167) && (ADC_Left_Detect >= 184)) {
        requested_move = Stop;
     }
}

void Stop_case(void){
    if (requested_move == Stop){
        strcpy(display_line[3], "    DONE   ");
        display_changed = TRUE;

        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;
        time_change = 0;
    }
}


//void Configure_Wheel_Speeds_case(void){
//}
//void Reverse_Move_Start_case(void){
//}
//void Reverse_Adjust_case(void){
//}
//void Initiate_Stop_case(void){
//}
