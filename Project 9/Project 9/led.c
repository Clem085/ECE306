/*
 * led.c
 *
 *  Created on: Jan 25, 2024
 *      Author: varun
 */

#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

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

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
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
