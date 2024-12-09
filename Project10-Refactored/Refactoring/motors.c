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
// Initialized with a value
unsigned int pause_count = 0;
unsigned int white_counter = 0;
unsigned int white_flag = 0;

// Motor  // Black Line Following // START //
//-------------------------------------------------------------------------
// This sets the car to start moving toward the line unitl it sees it
void line_search(void){
    fwd_slow();
    strcpy(display_line[0], "Find  Line");
    display_changed = TRUE;
    if((ADC_Left_Detect >= LOW_BLACK_LEFT) && (ADC_Right_Detect >= LOW_BLACK_RIGHT)){
        pause_count = 0;
        state = BLK_DETECTED;
    }
}

void line_found(void){
    pause_flag = TRUE;
    motors_off();
    strcpy(display_line[0], "Black  Line");
    display_changed = TRUE;
    if(pause_count >= PAUSE){
        state = PARALLEL;
        pause_flag = FALSE;
        pause_count=0;
    }
}

void parallel_park(void){
    // now get it off course like a turn
    strcpy(display_line[0], "  BL Turn ");
    display_changed = TRUE;
    if(ADC_Left_Detect >= HIGH_BLACK_LEFT && ADC_Right_Detect >= HIGH_BLACK_RIGHT){
        state = PAUSE_FOLLOW;
    }
    else {
        spin_left();
    }

}
void follow_pause(void){
    pause_flag = TRUE;
    strcpy(display_line[0], "  BL Turn ");
    display_changed = TRUE;
    motors_off();

    if(pause_count >= PAUSE){
        state = FOLLOW;
        pause_flag = FALSE;
        pause_count=0;
    }
}

void follow_line(void){
    pause_flag = TRUE;
    double CirclePause = 10.0; // 7.5
    double CircleDuration = 10.0; // Modify PauseSeconds to change when Circle continues FOLLOWing
    if((pause_count > CirclePause*20) && (pause_count < CirclePause*20+20*CircleDuration)){
        motors_off();
        strcpy(display[0],"CircleStop");
        display_changed = TRUE;
    }else{
        strcpy(display_line[0], "BL  Follow");
        display_changed = TRUE;
        motors_off();
        if(ADC_Left_Detect >= black_low && ADC_Right_Detect >= black_low){ // Replaced BLACKDETECTL & BLACKDETECTR
            white_flag = FALSE;
            strcpy(display_line[4], " Forward  ");
            display_changed = TRUE;
            fwd_slow();
        }else if(ADC_Right_Detect < black_low && ADC_Left_Detect >= black_low){ // Replaced BLACKDETECTR & BLACKDETECTL
            white_flag = FALSE;
            strcpy(display_line[4], "   Left   ");
            display_changed = TRUE;
            right_slow();
        }else if(ADC_Left_Detect < black_low && ADC_Right_Detect >= black_low){ // Replaced BLACKDETECTL & BLACKDETECTR
            white_flag = FALSE;
            strcpy(display_line[4], "  Right   ");
            display_changed = TRUE;
            left_slow();
        }else
            if(ADC_Left_Detect < black_high && ADC_Right_Detect < black_high){ // Replaced  BLACKL & BLACKR
                motors_off();
                white_flag = TRUE;
                if(white_counter < 100){// Approximately 5 seconds
                    strcpy(display_line[4], "White  Rev");
                    display_changed = TRUE;
                    rev_fast();
                }else{
                    strcpy(display_line[4], "White  Fwd");
                    display_changed = TRUE;
                    fwd_fast();
                }
            }else{
                white_flag = FALSE;
            }
    }

}
// Motor  // Black Line Following // END //








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
void left_slow(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = SLOW_FWD_RIGHT;
}
void right_slow(void){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = SLOW_FWD_RIGHT;
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
    double sec_spin = 0.29; // Seconds For PARALLEL // was 0.30
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
    case 2: // PARALLEL 90 CLOCKWISE
        strcpy(display_line[3],"90 PARALLEL #1");
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
    case 4: // PARALLEL 90 CLOCKWISE AGAIN
        strcpy(display_line[3],"90 PARALLEL #2");
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
            moving_flag = 0;
            motors_off();
            moving = 0;
            command = WAIT;
            movement_flag = 0;
        }
        break;
    default: break;
    } // END of ARCH switch(archState)
}
// Arch // Movement // END //







// Black Line // Initial Movement // START //
void initialMovementBL(void){
    switch(stopMoving){
    case '0': {
        // NONE
        if(init_cmd_state==0){
            init_cmd_state++;
            Clear_Display();
            motors_off();
            state = WAIT;
        }else{
            stopMoving = 'I';
        }
        break;
    }
    case '1': {
        // FWD1
        fwd_fast();
        //        slow_forward();
        dispPrint("FWD1: 5", '1');
        display_changed = TRUE;
        if(moving > FWD1_DURATION){ // Change Hard Coded Value Based on Testing
            // Ensure Black Line still runs fine after this
            moving_flag = 0;
            motors_off();
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                Clear_Display();
                motors_off();
                state = WAIT;
            }else{
                stopMoving = 'I';
                moving = 0;
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
        if(moving > FWD2_DURATION){ // Change Hard Coded Value Based on Testing
            moving_flag = 0;
            movement_flag = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                Clear_Display();
                motors_off();
                state = WAIT;
            }else{
                stopMoving = 'I';
                moving = 0;
            }
        }
        break;
    }
    case '3': {
        // BACK1
        rev_fast();
        dispPrint("BACK1: 5", '1');
        display_changed = TRUE;
        if(moving > BACK1_DURATION){ // Change Hard Coded Value Based on Testing
            moving_flag = 0;
            //            motors_off();
            //            moving = 0;
            //            command = WAIT;
            movement_flag = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                Clear_Display();
                motors_off();
                state = WAIT;
            }else{
                stopMoving = 'I';
                moving = 0;
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
        if(moving > BACK2_DURATION){ // Change Hard Coded Value Based on Testing
            moving_flag = 0;
            motors_off();
            //            moving = 0;
            //            command = WAIT;
            movement_flag = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                Clear_Display();
                motors_off();
                state = WAIT;
            }else{
                stopMoving = 'I';
                moving = 0;
            }
        }
        break;
    }
    case '5':{
        // 180
        spin_left();
        dispPrint("180", '1');
        display_changed = TRUE;
        if(moving > S180_DURATION){ // Change Hard Coded Value Based on Testing
            moving_flag = 0;
            motors_off();
            movement_flag = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                Clear_Display();
                motors_off();
                state = WAIT;
            }else{
                stopMoving = 'I';
                moving = 0;
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
            if(moving > LEFT1_DURATION_LEFT){ // Change Hard Coded Value Based on Testing
                motors_off();
                movement_flag = 0;
                init_cmd_state++;
                moving = 0;
                //                moving_flag = 0;
                //                command = WAIT;
            }
            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("LEFT1 FWD", '1');
            display_changed = TRUE;
            if(moving > LEFT1_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                moving_flag = 0;
                motors_off();
                //                moving = 0;
                //                command = WAIT;
                movement_flag = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    Clear_Display();
                    motors_off();
                    state = WAIT;
                }else{
                    stopMoving = 'I';
                    moving = 0;
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
            if(moving > LEFT2_DURATION_LEFT){ // Change Hard Coded Value Based on Testing
                motors_off();
                movement_flag = 0;
                init_cmd_state++;
                moving = 0;
                //                moving_flag = 0;
                //                command = WAIT;
            }

            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("LEFT2 FWD", '1');
            display_changed = TRUE;
            if(moving > LEFT2_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                moving_flag = 0;
                motors_off();
                //                moving = 0;
                //                    command = WAIT;
                movement_flag = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    Clear_Display();
                    motors_off();
                    state = WAIT;
                }else{
                    stopMoving = 'I';
                    moving = 0;
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
            if(moving > RIGHT1_DURATION_RIGHT){ // Change Hard Coded Value Based on Testing
                motors_off();
                movement_flag = 0;
                init_cmd_state++;
                moving = 0;
                //                moving_flag = 0;
                //                command = WAIT;
            }

            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("RIGHT1 FWD", '1');
            display_changed = TRUE;
            if(moving > RIGHT1_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                moving_flag = 0;
                motors_off();
                //                moving = 0;
                //                        command = WAIT;
                movement_flag = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    Clear_Display();
                    motors_off();
                    state = WAIT;
                }else{
                    stopMoving = 'I';
                    moving = 0;
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
            if(moving > RIGHT2_DURATION_RIGHT){ // Change Hard Coded Value Based on Testing
                motors_off();
                movement_flag = 0;
                init_cmd_state++;
                moving = 0;
                //                moving_flag = 0;
                //                command = WAIT;
            }
            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("RIGHT2 FWD", '1');
            display_changed = TRUE;
            if(moving > RIGHT2_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                moving_flag = 0;
                motors_off();
                //                moving = 0;
                //                            command = WAIT;
                movement_flag = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    Clear_Display();
                    motors_off();
                    state = WAIT;
                }else{
                    stopMoving = 'I';
                    moving = 0;
                }
            }
            break;
        }
        default: break;
        }
        break;
    }

    case 'I': {
        black_line();
        break;
    }

    default: break;
    }
}
// Black Line // Initial Movement // END //


// Black Line // Line Following // START //
void black_line(void){
    following = 1;
    switch(state){
    case WAIT:
        motors_off();
        strcpy(display_line[0], "   Start  ");
        display_changed = TRUE;
        pause_flag = TRUE;
        if(pause_count >= PAUSE){
            pause_flag = FALSE;
            pause_count=0;
            state = START; // ARCH
        }
        //            archState = 1;
        //            arch_counter = 0;
        break;
    case START:
        line_search();
        break;
    case BLK_DETECTED:
        line_found();
        break;
    case PARALLEL:
        parallel_park();
        break;
    case PAUSE_FOLLOW:
        follow_pause();
        break;
    case FOLLOW:
        follow_line();
        break;
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
