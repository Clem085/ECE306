//===========================================================================
//  File Name : Display.c
//  Description: This file contains code to control the Display
//      Note: This Code was originally stored in main.c. Moved to separate file as instructed in Project 2
//  Author: Jim Carlson
//  Date: Jan 2023
//  -- -- -- -- -- -- -- -- -- -- --
//  Revision Log
//  9/20/2024
//      Programmer: Connor Savugot
//      Revision: Added 1 new Functions
//          backlight(action);
//  9/24/2024
//      Programmer: Connor Savugot
//      Revision: Optimized backlight to be more memory efficient by using only global variables
//  -- -- -- -- -- -- -- -- -- -- --
//===========================================================================
// Includes
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

// Globals
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern unsigned int backlight_status;


void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}

// Controls Display Backlight
void backlight_control(void){
/*  Parameter Values
    action
        0: Backlight OFF
        1: Backlight ON
*/
    if(backlight_status == 0){
        P6OUT  &= ~LCD_BACKLITE;
        P6DIR  &= ~LCD_BACKLITE;
    }
    else{
        P6OUT  |=  LCD_BACKLITE;
        P6DIR  |=  LCD_BACKLITE;
    }
}
