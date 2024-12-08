//===========================================================================
//  File Name: Display.h
//  Description: Header file for Display.c - Contains #defines, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Oct 14, 2024
//===========================================================================

#ifndef DISPLAY_H_
#define DISPLAY_H_

//-----//-----// ORIGINAL //-----//-----//
//// Global Variables
////// Internally Defined
//char backlight_status;
//char light_percent;
//int middleChar(char * string);
////// Externally Defined
//extern char display_line[4][11];
//extern volatile unsigned char display_changed;
//extern volatile unsigned char update_display;
//extern int Switch_Counter1;
//extern char adc_char[10];
//
//
//
// Macro Definitions
// PWM
#define LCD_BACKLITE_DIMING (TB3CCR1)
#define PERCENT_100 (50000)
#define PERCENT_80  (45000)
//
//
//
//// Functions
//void backlight_control(void);
//void dispPrint(char *line, char lineToUpdate);
//void adc_line(char line, char location);
//void PWM_backlight(void);
//-----//-----// ORIGINAL //-----//-----//




#endif /* DISPLAY_H_ */
