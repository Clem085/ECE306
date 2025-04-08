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


extern char adc_char[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
volatile char IOT_Ring_Rx[SMALL_RING_SIZE];
char display_line[4][11];

void transmit_receive(char line, char loc){
    int i;
    unsigned int kayon_line;
    kayon_line = line - 1;
    for(i=0 ; i<9 ; i++) {
        display_line[kayon_line][i+loc] = IOT_Ring_Rx[i];
    }
    display_changed = TRUE;

}


void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}


