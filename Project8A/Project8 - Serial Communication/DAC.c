/*
 * DAC.c
 *
 *  Created on: Nov 8, 2024
 *      Author: jammi
 */

// #include as of 11-08-24
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
#include  "serial.h"
#include  "DAC.h"
// Libraries
#include  <string.h>
#include  <stdio.h>

void Init_DAC(void){
    // Init DAC

    P1OUT  &= ~RED_LED; // Set Red LED OFF
}



