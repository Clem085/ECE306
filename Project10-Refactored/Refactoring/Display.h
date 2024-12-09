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
//// Externally Defined
extern char adc_char[4];
extern unsigned char display_changed;
extern unsigned char update_display;
extern char *display[4];
extern char display_line[4][11];
extern char sheet;
extern unsigned int ir_setting;
extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int black_low;
extern unsigned int black_high;
extern unsigned int ir_one_time;


// Macro Definitions
//// PWM
#define PERCENT_100 (50000)
#define PERCENT_80  (45000)
#define PERCENT_0   (00000)

// Functions
void Display_Process(void);
void backlight_control(void);
void dispPrint(char *line, char lineToUpdate);
void adc_line(char line, char location);
void PWM_backlight(void);
void Clear_Display(void);
void Display_complete(void);
void ir_conf_display(void);




#endif /* DISPLAY_H_ */
