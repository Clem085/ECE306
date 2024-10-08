
//===========================================================================
// File Name : Display.c
//
// Description: This file contains the Initialization for display aspects
//
// Author: Kayla Radu
// Date: Sept 12, 2024
// Compiler: Built with Code Composer Studio Version: 12.8.0.00012
//===========================================================================

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "ports.h"

//Variables
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

extern volatile unsigned char backlight;
extern volatile unsigned char backlight_changed;

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}

void Backlight_Process(void){
    if(backlight_changed){
        backlight_changed = 0;
        if(backlight){
            P6OUT |= LCD_BACKLITE;
        }
        else {
            P6OUT &= LCD_BACKLITE;
        }
    }
}

