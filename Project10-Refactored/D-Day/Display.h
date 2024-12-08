//===========================================================================
//  File Name: Display.h
//  Description: Header file for Display.c - Contains #defines, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Oct 14, 2024
//===========================================================================

#ifndef DISPLAY_H_
#define DISPLAY_H_

// Global Variables
//// Internally Defined
char backlight_status;
char light_percent;
int middleChar(char * string);
//// Externally Defined
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern int Switch_Counter1;
extern char adc_char[10];
// NEW
extern char sheet;
extern unsigned int ir_setting;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
extern unsigned int black_low;
extern unsigned int black_high;
extern unsigned int ir_one_time;


// Macro Definitions
// PWM
#define LCD_BACKLITE_DIMING (TB3CCR1)
#define PERCENT_100 (50000)
#define PERCENT_80  (45000)



// Functions
void backlight_control(void);
void dispPrint(char *line, char lineToUpdate);
void adc_line(char line, char location);
void PWM_backlight(void);





#endif /* DISPLAY_H_ */