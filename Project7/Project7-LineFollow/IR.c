

// #include as of 10-21-24
// Header Files
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "motors.h"
#include  "Display.h"
#include  "timers.h"
#include  "switches.h"
#include  "ADC.h"
#include  "IR.h"
// Libraries
#include  <string.h>
#include  <stdio.h>

// IR_status Variable
// Change this turn toggle the IR emitter LED on or off
char IR_status;
char IR_changed;

// Controls IR LED
void IR_control(void){
    /*  Parameter Values
    action
        '0': Backlight OFF
        '1': Backlight ON
     */
    if(IR_status == OFF){
        P2OUT  &= ~IR_LED;
    }
    else{// IR_status = ON
        P2OUT  |=  IR_LED;
    }
}

