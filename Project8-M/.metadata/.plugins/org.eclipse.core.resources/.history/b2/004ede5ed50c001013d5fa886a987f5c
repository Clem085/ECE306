
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
#include "ADC.h"



//Variables
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

extern volatile unsigned int backlight;
extern volatile unsigned char backlight_changed;
extern char adc_char[10];
extern char display_line[4][11];

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
            P6OUT &= ~LCD_BACKLITE;
        }
    }
}
void LCD_Display(const char *line1, const char *line2, const char *line3, const char *line4) {
    // Assume display_line is used to represent each line of the LCD
    extern char display_line[4][11];  // Assuming the display lines each have 11 characters

    strncpy(display_line[0], line1, 10);
    display_line[0][10] = '\0';  // Null-terminate each line

    strncpy(display_line[1], line2, 10);
    display_line[1][10] = '\0';

    strncpy(display_line[2], line3, 10);
    display_line[2][10] = '\0';

    strncpy(display_line[3], line4, 10);
    display_line[3][10] = '\0';

    // Update the display flags
    update_display = 1;
    display_changed = 1;
}
//-------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 9
//
//-------------------------------------------------------------
void adc_line(int line, int location){
//-------------------------------------------------------------
int i;
unsigned int real_line;
real_line = line - 1;
for(i=0; i < 4; i++) {
display_line[real_line][i+location] = adc_char[i];
}
update_display = TRUE;
display_changed = TRUE;
}
//-------------------------------------------------------------


