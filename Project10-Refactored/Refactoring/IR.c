/* IR Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : IR.c
  Description:  This file contains the code to control the IR LED (Infrared Emitter LED)
  Programmer: Connor Savugot
  Date Created: Oct 19, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
*/

// #include as of 11-27-24
//// Header Files
#include  "ADC.h"
#include  "DAC.h"
#include  "Display.h"
#include  "functions.h"
#include  "interrupts.h"
#include  "IR.h"
#include  "IOT.h"
#include  "LCD.h"
#include  "macros.h"
#include  "menu.h"
#include  "motors.h"
#include  "msp430.h"
#include  "ports.h"
#include  "serial.h"
#include  "switches.h"
#include  "timers.h"
//// Libraries
#include  <stdio.h>
#include  <string.h>

// Global Variables declared and referenced in Header file



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

