/*
 * led.c
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


