//===========================================================================
//  File Name : led.c
//  Description: This file contains code to control the red and green LEDs
//      Note: This Code was originally stored in main.c. Moved to separate file as instructed in Project 2
//  Author: Jim Carlson
//  Date: Jan 2023
//===========================================================================

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

void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  P1OUT |=  RED_LED;
  P6OUT |=  GRN_LED;
//------------------------------------------------------------------------------
}
