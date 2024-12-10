//===========================================================================
//  File Name : led.c
//  Description: This file contains code to control the red and green LEDs
//      Note: This Code was originally stored in main.c. Moved to separate file as instructed in Project 2
//  Author: Jim Carlson
//  Date: Jan 2023
//===========================================================================

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


void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}


