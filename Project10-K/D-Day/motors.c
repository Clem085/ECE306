/* Motors Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : motors.c
  Description:  This file contains the code to control the motors using PWM
  Programmer: Connor Savugot
  Date Created: Sep 20, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */

// #include as of 11-27-24
//// Header Files
#include "ADC.h"
#include "DAC.h"
#include "Display.h"
#include "functions.h"
#include "interrupts.h"
#include "IR.h"
#include "IOT.h"
#include "LCD.h"
#include "macros.h"
#include "menu.h"
#include "motors.h"
#include "msp430.h"
#include "ports.h"
#include "serial.h"
#include "switches.h"
#include "timers.h"
//// Libraries
#include <stdio.h>
#include <string.h>

// Global Variables declared and referenced in Header file







//unsigned char event;
extern unsigned int event_Counter;
extern unsigned int Last_Time_Sequence;
extern unsigned int cycle_time;
extern unsigned int time_change;
unsigned int Time_Sequence;

unsigned int delay_start;
unsigned int left_motor_count;
unsigned int right_motor_count;
unsigned int segment_count;


unsigned int waitingtostart;
extern unsigned int rdirection = 0;
extern unsigned int ldirection = 0;
extern unsigned int instruction;
unsigned char origevent;
extern unsigned int elapsed_time = 0;
extern unsigned int desiredtime= 0;
extern unsigned int desired_cycles = 0;

extern char display_line[4][11];
extern char *display[4];
extern unsigned char display_changed;
extern unsigned char update_display;
extern unsigned int switchpressed;
extern unsigned int instruction;

extern unsigned int blackleft;
extern unsigned int blackright;
extern unsigned int blacklinefound;

extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;

unsigned int SpincountL = 0;
unsigned int SpincountR = 0;
unsigned int Waitcount = 0;
extern unsigned int FlagSpinL;
extern unsigned int FlagSpinR;
extern unsigned int FlagWait;
unsigned int DirectionTurn = 0;

extern unsigned int state;

unsigned int lostCounter = 0;
unsigned int lostflg = 0;

extern unsigned int black_low;
extern unsigned int black_high;









// Motor  // Black Line Following // START //
//-------------------------------------------------------------------------
// This sets the car to start moving toward the line unitl it sees it
void start_movement(void){
    motors_off();
    LEFT_FORWARD_SPEED = 12000;
    RIGHT_FORWARD_SPEED = 12000;

    strcpy(display_line[0], "INTERCEPT ");
    display_changed = TRUE;

    if((ADC_Left_Detect >= LOW_BLACK_LEFT) && (ADC_Right_Detect >= LOW_BLACK_RIGHT)){
        Waitcount = 0;
        state = BLK_DETECTED;
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
    if(ADC_Left_Detect >= HIGH_BLACK_LEFT && ADC_Right_Detect >= HIGH_BLACK_RIGHT){
        state = PAUSE_TRACK;
    }
    else {
        RIGHT_REVERSE_SPEED = SPINSPEED;
        LEFT_FORWARD_SPEED = SPINSPEED;
    }

}
void track_wait(void){
    FlagWait = TRUE;
    strcpy(display_line[0], "  BL Turn ");
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
    double CirclePause = 10.0; // 7.5
    double CircleDuration = 10.0; // Modify PauseSeconds to change when Circle continues tracking
    if((Waitcount > CirclePause*20) && (Waitcount < CirclePause*20+20*CircleDuration)){
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        strcpy(display[0],"CircleStop");
        display_changed = TRUE;
    }else{
        strcpy(display_line[0], "BL Travel ");
        display_changed = TRUE;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;




        // IT IS ON THE LINE
        if(ADC_Left_Detect >= black_low && ADC_Right_Detect >= black_low){ // Replaced BLACKDETECTL & BLACKDETECTR
            lostflg = FALSE;
            strcpy(display_line[4], " STRAIGHT ");
            display_changed = TRUE;
            RIGHT_FORWARD_SPEED = STRAIGHTMOVE;
            LEFT_FORWARD_SPEED = STRAIGHTMOVE;
            // THE LEFT SENSOR IS MISSING, BUT RIGHT SENSOR IS ON THE LINE
        }else if(ADC_Left_Detect < black_low && ADC_Right_Detect >= black_low){ // Replaced BLACKDETECTL & BLACKDETECTR
            lostflg = FALSE;
            strcpy(display_line[4], "  RIGHT   ");
            display_changed = TRUE;
            LEFT_FORWARD_SPEED = TURNSPEED;
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            DirectionTurn = RIGHT;
            // THE RIGHT SENSOR IS MISSING I NEED TO TURN LEFT TOWARD THE LINE
        }else if(ADC_Right_Detect < black_low && ADC_Left_Detect >= black_low){ // Replaced BLACKDETECTR & BLACKDETECTL
            lostflg = FALSE;
            strcpy(display_line[4], "   LEFT   ");
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
                    strcpy(display_line[4], "   LOST   ");
                    display_changed = TRUE;
                    //                    RIGHT_REVERSE_SPEED = SPEED1;
                    //                    LEFT_REVERSE_SPEED = SPEED1;
                    rev_fast();
                }else{
                    strcpy(display_line[4], " FWD LOST ");
                    display_changed = TRUE;
                    //                    RIGHT_FORWARD_SPEED = SPEED1;
                    //                    LEFT_FORWARD_SPEED = SPEED1;
                    fwd_fast();
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
// Motor  // Black Line Following // END //

void end_state(void){
    strcpy(display_line[0], " STOPPED ");
    update_display = TRUE;
    display_changed = 1;
    motors_off();
    SpincountL = 0;
    SpincountR = 0;
    state = WAIT;

}








// Motor // PWM // START //
// --------------------------------------------------------------
//// PWM Off
void motors_off(void){
    fwd_off();
    rev_off();
}
void fwd_off(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}
void rev_off(void){
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}
void spin_left_off(void){
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}
void spin_right_off(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}
//// Slow PWM Values
void fwd_slow(void){
    rev_off();
    RIGHT_FORWARD_SPEED = SLOW_FWD_RIGHT;
    LEFT_FORWARD_SPEED = SLOW_FWD_LEFT;
}
void rev_slow(void){
    fwd_off();
    RIGHT_REVERSE_SPEED = SLOW_FWD_RIGHT;
    LEFT_REVERSE_SPEED = SLOW_FWD_LEFT;
}
//// Fast PWM Values
void fwd_fast(void){
    rev_off();
    RIGHT_FORWARD_SPEED = FAST_FWD_RIGHT;
    LEFT_FORWARD_SPEED = FAST_FWD_LEFT;
}
void rev_fast(void){
    fwd_off();
    RIGHT_REVERSE_SPEED = FAST_FWD_RIGHT;
    LEFT_REVERSE_SPEED = FAST_FWD_LEFT;
}
void left_fast(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = FAST_FWD_RIGHT;
}
void right_fast(void){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = FAST_FWD_RIGHT;
}
//// Spin Speeds On
void spin_left(void){
    spin_right_off();
    RIGHT_REVERSE_SPEED = SLOW_SPIN;
    LEFT_FORWARD_SPEED = SLOW_SPIN;
}
void spin_right(void){
    spin_left_off();
    RIGHT_FORWARD_SPEED = SLOW_SPIN;
    LEFT_REVERSE_SPEED = SLOW_SPIN;
}
//--------------------------------------------------------------
// Motor // PWM // END //







// Arch // Movement // START //
void arch_movement(void){
    double sec_fwd1 = 2.4; // Seconds For FWD
    double sec_fwd2 = 2.75; // Seconds For FWD2 // 2.50 WAS SHORT
    double sec_fwd3 = 1.25; // Seconds For FWD3 // was 1.0
    double sec_spin = 0.29; // Seconds For SPIN // was 0.30
    double sec_wait = 1.00; // Seconds for Wait

    switch(archState){
    case 0: // WAIT CASE
        strcpy(display_line[3],"   WAIT   ");
        motors_off();
        if(motorDrain > 20*sec_wait){
            arch_counter = 0;
            motorDrain = 0;
            archState = nextState;
        }
        break;
    case 1: // FORWARD
        strcpy(display_line[3],"FORWARD #1");
        display_changed = TRUE;
        LEFT_FORWARD_SPEED = LSLOWCIRCLE; //LEFTARCHFWD; //
        RIGHT_FORWARD_SPEED = RSLOWCIRCLE; // RIGHTARCHFWD;
        if(arch_counter > 20*sec_fwd1){
            motors_off();
            arch_counter = 0;
            motorDrain = 0;
            archState = 0;
            nextState = 2;
        }
        break;
    case 2: // SPIN 90 CLOCKWISE
        strcpy(display_line[3],"90 SPIN #1");
        display_changed = TRUE;
        LEFT_REVERSE_SPEED = LEFTARCHFWD;
        RIGHT_FORWARD_SPEED = RIGHTARCHFWD;
        if(arch_counter > 20*sec_spin){
            motors_off();
            archState = 0; // Enter Wait Case
            arch_counter = 0;
            motorDrain = 0;
            nextState = 3;
        }
        break;
    case 3: // FORWARD AGAIN
        strcpy(display_line[3],"FORWARD #2");
        display_changed = TRUE;
        LEFT_FORWARD_SPEED = LSLOWCIRCLE; //LEFTARCHFWD;
        RIGHT_FORWARD_SPEED = RSLOWCIRCLE; //RIGHTARCHFWD;
        if(arch_counter > 20*sec_fwd2){
            motors_off();
            archState = 0; // Enter Wait Case
            arch_counter = 0;
            motorDrain = 0;
            nextState = 4;
        }
        break;
    case 4: // SPIN 90 CLOCKWISE AGAIN
        strcpy(display_line[3],"90 SPIN #2");
        display_changed = TRUE;
        LEFT_REVERSE_SPEED = LEFTARCHFWD;
        RIGHT_FORWARD_SPEED = RIGHTARCHFWD;
        if(arch_counter > 20*sec_spin){
            motors_off();
            archState = 0; // Enter Wait Case
            arch_counter = 0;
            motorDrain = 0;
            nextState = 5;
        }
        break;
    case 5: // FORWARD FINAL
        strcpy(display_line[3],"FORWARD #3");
        display_changed = TRUE;
        LEFT_FORWARD_SPEED = LSLOWCIRCLE; //LEFTARCHFWD;
        RIGHT_FORWARD_SPEED = RSLOWCIRCLE; //RIGHTARCHFWD;
        if(arch_counter > 20*sec_fwd3){
            motors_off();
            arch_counter = 0;
            archState = 0;
            state = START;
            dispPrint(" ",'4');
            display_changed = TRUE;
            run_time_flag = 0;
            motors_off();
            run_time = 0;
            commanding_send = WAIT;
            movement = 0;
        }
        break;
    default: break;
    } // END of ARCH switch(archState)
}
// Arch // Movement // END //







// Black Line // Initial Movement // START //
void initialMovementBL(void){
    switch(setTime){
    case '0': {
        // NONE
        if(init_cmd_state==0){
            init_cmd_state++;
            Clr_Display();
            motors_off();
            state = WAIT;
        }else{
            setTime = 'I';
        }
        break;
    }
    case '1': {
        // FWD1
        fwd_fast();
        //        slow_forward();
        dispPrint("FWD1: 5", '1');
        display_changed = TRUE;
        if(run_time > FWD1_DURATION){ // Change Hard Coded Value Based on Testing
            // Ensure Black Line still runs fine after this
            run_time_flag = 0;
            motors_off();
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                Clr_Display();
                motors_off();
                state = WAIT;
            }else{
                setTime = 'I';
                run_time = 0;
            }
        }
        break;
    }
    case '2': {
        // FWD2
        fwd_fast();
        //        slow_forward();
        dispPrint("FWD2: 9", '1');
        display_changed = TRUE;
        if(run_time > FWD2_DURATION){ // Change Hard Coded Value Based on Testing
            run_time_flag = 0;
            //            motors_off();

            //            commanding_send = INTERCEPT;
            movement = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                Clr_Display();
                motors_off();
                state = WAIT;
            }else{
                setTime = 'I';
                run_time = 0;
            }
        }
        break;
    }
    case '3': {
        // BACK1
        rev_fast();
        dispPrint("BACK1: 5", '1');
        display_changed = TRUE;
        if(run_time > BACK1_DURATION){ // Change Hard Coded Value Based on Testing
            run_time_flag = 0;
            //            motors_off();
            //            run_time = 0;
            //            commanding_send = WAIT;
            movement = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                Clr_Display();
                motors_off();
                state = WAIT;
            }else{
                setTime = 'I';
                run_time = 0;
            }
        }
        break;
    }
    case '4':{
        // BACK2
        //        slow_backward();
        rev_fast();
        dispPrint("BACK2: 9", '1');
        display_changed = TRUE;
        if(run_time > BACK2_DURATION){ // Change Hard Coded Value Based on Testing
            run_time_flag = 0;
            motors_off();
            //            run_time = 0;
            //            commanding_send = WAIT;
            movement = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                Clr_Display();
                motors_off();
                state = WAIT;
            }else{
                setTime = 'I';
                run_time = 0;
            }
        }
        break;
    }
    case '5':{
        // 180
        spin_left();
        dispPrint("180", '1');
        display_changed = TRUE;
        if(run_time > S180_DURATION){ // Change Hard Coded Value Based on Testing
            run_time_flag = 0;
            motors_off();
            //            run_time = 0;
            //            commanding_send = WAIT;
            movement = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                Clr_Display();
                motors_off();
                state = WAIT;
            }else{
                setTime = 'I';
                run_time = 0;
            }
        }
        break;
    }
    case '6': {
        // LEFT1
        switch(init_cmd_state){
        case 0: {
            spin_left();
            dispPrint("LEFT1 LEFT", '1');
            display_changed = TRUE;
            if(run_time > LEFT1_DURATION_LEFT){ // Change Hard Coded Value Based on Testing
                motors_off();
                movement = 0;
                init_cmd_state++;
                run_time = 0;
                //                run_time_flag = 0;
                //                commanding_send = WAIT;
            }
            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("LEFT1 FWD", '1');
            display_changed = TRUE;
            if(run_time > LEFT1_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                run_time_flag = 0;
                motors_off();
                //                run_time = 0;
                //                commanding_send = WAIT;
                movement = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    Clr_Display();
                    motors_off();
                    state = WAIT;
                }else{
                    setTime = 'I';
                    run_time = 0;
                }
            }
            break;
        }
        default: break;
        }
        break;
    }

    case '7': {
        // LEFT2
        switch(init_cmd_state){
        case 0: {
            spin_left();
            dispPrint("LEFT2 LEFT", '1');
            display_changed = TRUE;
            if(run_time > LEFT2_DURATION_LEFT){ // Change Hard Coded Value Based on Testing
                motors_off();
                movement = 0;
                init_cmd_state++;
                run_time = 0;
                //                run_time_flag = 0;
                //                commanding_send = WAIT;
            }

            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("LEFT2 FWD", '1');
            display_changed = TRUE;
            if(run_time > LEFT2_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                run_time_flag = 0;
                motors_off();
                //                run_time = 0;
                //                    commanding_send = WAIT;
                movement = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    Clr_Display();
                    motors_off();
                    state = WAIT;
                }else{
                    setTime = 'I';
                    run_time = 0;
                }
            }
            break;
        }
        default: break;
        }
        break;
    }


    case '8': {
        // RIGHT1
        switch(init_cmd_state){
        case 0: {
            spin_right();
            dispPrint("RIGHT1 RIGHT", '1');
            display_changed = TRUE;
            if(run_time > RIGHT1_DURATION_RIGHT){ // Change Hard Coded Value Based on Testing
                motors_off();
                movement = 0;
                init_cmd_state++;
                run_time = 0;
                //                run_time_flag = 0;
                //                commanding_send = WAIT;
            }

            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("RIGHT1 FWD", '1');
            display_changed = TRUE;
            if(run_time > RIGHT1_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                run_time_flag = 0;
                motors_off();
                //                run_time = 0;
                //                        commanding_send = WAIT;
                movement = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    Clr_Display();
                    motors_off();
                    state = WAIT;
                }else{
                    setTime = 'I';
                    run_time = 0;
                }
            }
            break;
        }
        default: break;
        }
        break;
    }
    case '9': {
        // RIGHT2
        switch(init_cmd_state){
        case 0: {
            spin_right();
            dispPrint("RIGHT2 RIGHT", '1');
            display_changed = TRUE;
            if(run_time > RIGHT2_DURATION_RIGHT){ // Change Hard Coded Value Based on Testing
                motors_off();
                movement = 0;
                init_cmd_state++;
                run_time = 0;
                //                run_time_flag = 0;
                //                commanding_send = WAIT;
            }
            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("RIGHT2 FWD", '1');
            display_changed = TRUE;
            if(run_time > RIGHT2_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                run_time_flag = 0;
                motors_off();
                //                run_time = 0;
                //                            commanding_send = WAIT;
                movement = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    Clr_Display();
                    motors_off();
                    state = WAIT;
                }else{
                    setTime = 'I';
                    run_time = 0;
                }
            }
            break;
        }
        default: break;
        }
        break;
    }

    case 'I': {
        blacklinemachine();
        break;
    }

    default: break;
    }
}
// Black Line // Initial Movement // END //


// Black Line // Line Following // START //
void blacklinemachine(void){
    following = 1;
    switch(state){
    case WAIT:
        motors_off();
        strcpy(display_line[0], "   Start  ");
        display_changed = TRUE;
        FlagWait = TRUE;
        if(Waitcount >= PAUSE){
            FlagWait = FALSE;
            Waitcount=0;
            state = START; // ARCH
        }
        //            archState = 1;
        //            arch_counter = 0;
        break;
    case START:
        start_movement();
        break;
    case BLK_DETECTED:
        detect_movement();
        break;
    case SPIN:
        spinning_movement();
        break;
    case PAUSE_TRACK:
        track_wait();
        break;
    case TRACK:
        tracking_movement();
        break;
        //    case END:
        //        end_state();
        //
        //        run_time_flag = 0;
        //        run_time = 0;
        //        commanding_send = WAIT;
        //        movement = 0;
        //
        //        break;
    default: break;
    }
}
// Black Line // Line Following // END //



// Magic Smoke Detector
void vrfyDirection(void){
    if(((P6IN & L_FORWARD) && (P6IN & L_REVERSE)) || ((P6IN & R_FORWARD) && (P6IN & R_REVERSE))){
        // ISSUE: Both Left Forward and Reverse are on
        P1OUT |= RED_LED;
        motors_off(); // Turns off both left and right motor for the forward and reverse direction
        strcpy(display_line[0], " Error!!! ");
        strcpy(display_line[1], "MagicSmoke");
        strcpy(display_line[2], "Move Fwd &");
        strcpy(display_line[3], "Move Rev! ");
        display_changed = TRUE;

    }
}
