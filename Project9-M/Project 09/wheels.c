/*
 * wheels.c
 *
 *  Created on: Nov 21, 2024
 *      Author: Kayla Radu
 */


#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "strings.h"
#include "wheels.h"
#include "Timers.h"
//#include  "DAC.h"
#include "switches.h"



void Wheels_Stop(void);
void Wheels_Forward(void);
void Wheels_Reverse(void);
void Wheels_clkw(void);
void Wheels_counterclkw(void);
void motor_run_forward(void);
void motor_run_backward(void);
void motor_run_right(void);
void motor_run_left(void);



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

unsigned int SpincountL = 0;
unsigned int SpincountR = 0;
unsigned int Waitcount = 0;
extern unsigned int FlagSpinL;
extern unsigned int FlagSpinR;
extern unsigned int FlagWait;
unsigned int DirectionTurn = 0;

volatile unsigned char state;

unsigned int lostCounter = 0;
unsigned int lostflg = 0;

extern unsigned int black_low;
extern unsigned int black_high;


void motor_off (void) {
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}

void motor_run_forward (void) {
//    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
    //    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

    //    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = FASTRIGHT; // P6.1 Right Forward PWM ON amount

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
    //    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM ON amount

    //    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
    LEFT_FORWARD_SPEED = FASTLEFT; // P6.3 Left Forward PWM ON amount

    //    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM ON amount
}

void motor_run_backward (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
    //    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
    //    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM ON amount

    //    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
    RIGHT_REVERSE_SPEED = FASTRIGHT; // P6.2 Right Reverse PWM ON amount

    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
    //    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM ON amount

    //    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
    LEFT_REVERSE_SPEED = FASTLEFT; // P6.4 Left Reverse PWM ON amount
}

void motor_run_right (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
    //    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

    //    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
    RIGHT_FORWARD_SPEED = FAST; // P6.1 Right Forward PWM ON amount
    // ^^ USED TO BE FAST

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
    //   RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM ON amount

    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
    //    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM ON amount

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
    //    LEFT_REVERSE_SPEED = FAST; // P6.4 Left Reverse PWM ON amount
}

void motor_run_left (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF
    //    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM ON amount

    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF
    //   RIGHT_FORWARD_SPEED = WHEEL_OFF;; // P6.1 Right Forward PWM ON amount

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF
    //    RIGHT_REVERSE_SPEED = FAST; // P6.2 Right Reverse PWM ON amount
    // ^^ USED TO BE FAST

    //    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF
    LEFT_FORWARD_SPEED = FAST; // P6.3 Left Forward PWM ON amount

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
    //    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM ON amount

}

void motor_check (void) {
    if ((RIGHT_FORWARD_SPEED > 0) && (RIGHT_REVERSE_SPEED > 0)) {
        motor_off();
        //        P1OUT |= RED_LED;
    }
    if ((LEFT_FORWARD_SPEED > 0) && (LEFT_REVERSE_SPEED > 0)) {
        motor_off();
        //        P1OUT |= RED_LED;
    }
}

void Wheels_Stop(void){//code to stop wheels
    P6OUT &= ~L_FORWARD; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_FORWARD; // Set Port pin Low [Wheel Off]
    P6OUT &= ~L_REVERSE; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_REVERSE; // Set Port pin Low [Wheel Off]
}

void Wheels_Forward(void){// code to move forwards
    P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
    P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]
}


void Wheels_Reverse(void){//code to move in reverse
    P6OUT |= L_REVERSE;
    P6OUT |= R_REVERSE;
}

void Wheels_clkw(void){
    P6OUT |= L_FORWARD;
    P6OUT |= R_REVERSE;
}

void Wheels_counterclkw(void){
    P6OUT |= R_FORWARD;
    P6OUT |= L_REVERSE;
}



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

    if((ADC_Left_Detect >= BLKSTARTL) && (ADC_Right_Detect >= BLKSTARTR)){
        Waitcount = 0;
        state = DETECTED;
    }


}

void detect_movement(void){
    FlagWait = TRUE;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    strcpy(display_line[0], "BLACK LINE ");
    display_changed = TRUE;
    if(Waitcount >= PAUSE){
        state = SPIN;
        FlagWait = FALSE;
        Waitcount=0;
    }
}

void spinning_movement(void){
    // now get it off course like a turn
    strcpy(display_line[0], "SPINNING ");
    display_changed = TRUE;
    if(ADC_Left_Detect >= BLACKLSP && ADC_Right_Detect >= BLACKRSP){
        state = TRACKWAIT;
    }
    else {
        RIGHT_REVERSE_SPEED = SPINSPEED;
        LEFT_FORWARD_SPEED = SPINSPEED;
    }

}
void track_wait(void){
    FlagWait = TRUE;
    strcpy(display_line[0], " TRACKWAIT");
    display_changed = TRUE;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;

    if(Waitcount >= PAUSE){
        state = TRACK;
        FlagWait = FALSE;
        Waitcount=0;
    }

}

void tracking_movement(void){
    FlagWait = TRUE;
    double CirclePause = 7.5; // Originally 7.5
    double CircleDuration = 10.0; // Modify PauseSeconds to change when Circle continues tracking
    if((Waitcount > CirclePause*20) && (Waitcount < CirclePause*20+20*CircleDuration)){
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        strcpy(display[0],"CircleStop");
        display_changed = TRUE;
    }else{
        strcpy(display_line[0], " TRACKING ");
        display_changed = TRUE;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;




        // IT IS ON THE LINE
        if(ADC_Left_Detect >= black_low && ADC_Right_Detect >= black_low){ // Replaced BLACKDETECTL & BLACKDETECTR
            lostflg = FALSE;
            strcpy(display_line[0], " STRAIGHT ");
            display_changed = TRUE;
            RIGHT_FORWARD_SPEED = STRAIGHTMOVE;
            LEFT_FORWARD_SPEED = STRAIGHTMOVE;
            // THE LEFT SENSOR IS MISSING, BUT RIGHT SENSOR IS ON THE LINE
        }else if(ADC_Left_Detect < black_low && ADC_Right_Detect >= black_low){ // Replaced BLACKDETECTL & BLACKDETECTR
            lostflg = FALSE;
            strcpy(display_line[0], "  RIGHT   ");
            display_changed = TRUE;
            LEFT_FORWARD_SPEED = TURNSPEED;
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            DirectionTurn = RIGHT;
            // THE RIGHT SENSOR IS MISSING I NEED TO TURN LEFT TOWARD THE LINE
        }else if(ADC_Right_Detect < black_low && ADC_Left_Detect >= black_low){ // Replaced BLACKDETECTR & BLACKDETECTL
            lostflg = FALSE;
            strcpy(display_line[0], "   LEFT   ");
            display_changed = TRUE;
            RIGHT_FORWARD_SPEED = TURNSPEED;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            DirectionTurn = LEFT;
            // IT IS WAYYY OFF AND CANNOT FIND THE LINE
        }else
            if(ADC_Left_Detect < black_high && ADC_Right_Detect < black_high){ // Replaced  BLACKL & BLACKR
                LEFT_FORWARD_SPEED = WHEEL_OFF;
                RIGHT_FORWARD_SPEED = WHEEL_OFF;
                LEFT_REVERSE_SPEED = WHEEL_OFF;
                RIGHT_REVERSE_SPEED = WHEEL_OFF;
                lostflg = TRUE;
                if(lostCounter < 100){// Approximately 5 seconds
                    strcpy(display_line[0], "   LOST   ");
                    display_changed = TRUE;
//                    RIGHT_REVERSE_SPEED = SPEED1;
//                    LEFT_REVERSE_SPEED = SPEED1;
                    new_backward();
                }else{
                    strcpy(display_line[0], " FWD LOST ");
                    display_changed = TRUE;
//                    RIGHT_FORWARD_SPEED = SPEED1;
//                    LEFT_FORWARD_SPEED = SPEED1;
                    new_forward();
                }
            }else{
                lostflg = FALSE;
            }
        FlagSpinR = TRUE; // IS THIS ONLY FOR DANGER CODE?
//      DANGER CODE: Makes Car stop following Black Line after 2 minutes
//        if(SpincountR >= TWOMIN){
//            state = END;
//            SpincountR = 0;
//            FlagSpinR = FALSE;
//        }
    }

}


void end_state(void){
    strcpy(display_line[0], " STOPPED ");
    update_display = TRUE;
    display_changed = 1;
    Off_Case();
    SpincountL = 0;
    SpincountR = 0;
    state = WAIT;

}


void Off_Case(void){
    //    Forward_Off();
    //    Reverse_Off();
    RIGHT_FORWARD_SPEED = WHEEL_OFF;   // P6.1 Right Forward PWM duty cycle

    LEFT_FORWARD_SPEED = WHEEL_OFF;    // P6.2 Right Forward PWM duty cycl

    RIGHT_REVERSE_SPEED = WHEEL_OFF;   // P6.3 Left Forward PWM duty cycle

    LEFT_REVERSE_SPEED = WHEEL_OFF;
}











void new_forward(void){
    reverse_off();
    forward_on();
}

void new_backward(void){
    forward_off();
    reverse_on();
}

void new_left(void){
    right_off();
    left_on();
}

void new_right(void){
    left_off();
    right_on();
}

void forward_on(void){
    RIGHT_FORWARD_SPEED = TRUE_FWD_R;   // P6.3 Left Forward PWM duty cycle
    LEFT_FORWARD_SPEED = TRUE_FWD_L;
}

void forward_off(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;   // P6.3 Left Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}

void reverse_on(void){
    RIGHT_REVERSE_SPEED = TRUE_REV_R;   // P6.3 Left Forward PWM duty cycle
    LEFT_REVERSE_SPEED = TRUE_REV_L;
}

void reverse_off(void){
    RIGHT_REVERSE_SPEED = WHEEL_OFF;   // P6.3 Left Forward PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void left_on(void){
    RIGHT_REVERSE_SPEED = TRUE_REV_R;   // P6.3 Left Forward PWM duty cycle
    LEFT_FORWARD_SPEED = TRUE_REV_L;
}

void left_off(void){
    RIGHT_REVERSE_SPEED = WHEEL_OFF;   // P6.3 Left Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}

void right_on(void){
    RIGHT_FORWARD_SPEED = TRUE_REV_R;   // P6.3 Left Forward PWM duty cycle
    LEFT_REVERSE_SPEED = TRUE_REV_L;
}

void right_off(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;   // P6.3 Left Forward PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

// Both
// There is NO Function to turn both Forward and Reverse ON at the same time because I don't want my car to blow up
void motorsOFF(void){
    forward_off();
    reverse_off();
}

// Magic Smoke Detector
void vrfyDirection(void){
    // if(((P6IN & L_FORWARD) && (P6IN & L_REVERSE)) || ((P6IN & R_FORWARD) && (P6IN & R_REVERSE))){
    if((RIGHT_FORWARD_SPEED != WHEEL_OFF &&  RIGHT_REVERSE_SPEED != WHEEL_OFF) || (LEFT_FORWARD_SPEED != WHEEL_OFF &&  LEFT_REVERSE_SPEED != WHEEL_OFF)){
        // ISSUE: Both Left Forward and Reverse are on
        P1OUT |= RED_LED;
        motorsOFF(); // Turns off both left and right motor for the forward and reverse direction
        strcpy(display_line[0], " Error!!! ");
        strcpy(display_line[1], "MagicSmoke");
        strcpy(display_line[2], "Move Fwd &");
        strcpy(display_line[3], "Move Rev! ");
        display_changed = TRUE;
    }
}

void move_test(int direction){
    switch(direction){
        case 0:
            // OFF
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            break;

        case 1:
            // RIGHT FWD
            RIGHT_FORWARD_SPEED = MAX_SPEED;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            break;

        case 2:
            // RIGHT REV
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = MAX_SPEED;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            break;

        case 3:
            // LEFT FWD
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = MAX_SPEED;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            break;
        case 4:
            // LEFT REV
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = MAX_SPEED;
            break;
        case 5:
            // BOTH FWD
            RIGHT_FORWARD_SPEED = MAX_SPEED;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = MAX_SPEED;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            break;
        case 6:
            // BOTH REV
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = 49999;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = MAX_SPEED;
            break;
        case 7:
            // SPIN1
            RIGHT_FORWARD_SPEED = MAX_SPEED;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = MAX_SPEED;
            break;
        case 8:
            // SPIN2
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = MAX_SPEED;
            LEFT_FORWARD_SPEED = MAX_SPEED;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            break;
        default: break;

    }





}
