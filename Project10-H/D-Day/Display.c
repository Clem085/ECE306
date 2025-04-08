/*
 * Display.c
 *
 *  Created on: Dec 2, 2024
 *      Author: kaylaradu
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
extern char *display[4];
extern char display_line[4][11];
extern char sheet;
extern unsigned int ir_setting;
extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int black_low;
extern unsigned int black_high;
extern unsigned int ir_one_time;

void Display_Process(void){
    if(update_display){
        update_display = 0;
        if(display_changed){
            display_changed = 0;
            Display_Update(0,0,0,0);
        }
    }
}

void Clr_Display(void){
    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;
}


void Display_complete(void){
    char temp[11]; // Temporary buffer to format the string
    // Format the string manually
    temp[0] = 'A';
    temp[1] = 'R';
    temp[2] = 'R';
    temp[3] = 'I';
    temp[4] = 'V';
    temp[5] = 'E';
    temp[6] = 'D';
    temp[7] = ' ';
    temp[8] = '0'; // Space after the number
    temp[9] = sheet; // Place the `sheet` character
    temp[10] = '\0'; // Null terminator

    // Copy the formatted string into the display line
    strcpy(display_line[0], temp);
    display_changed = TRUE;



}

void ir_conf_display(void){
    switch(ir_setting){
    case 0:
        // Reset Config to Default
        black_low = 500;
        black_high = 600;
        dispPrint("BL=0500", '1');
        dispPrint("BH=0600", '2');
        display_changed = TRUE;
        break;
    case 1:
        // Set Low Black Line
        black_low = (ADC_Left_Detect+ADC_Right_Detect)/2 - 50;
        black_high = (ADC_Left_Detect+ADC_Right_Detect)/2 + 50;

        HexToBCD(black_low);
        char temp[5]; // Temporary buffer to format the string
        // Format the string manually
        temp[0] = adc_char[0];
        temp[1] = adc_char[1];
        temp[2] = adc_char[2];
        temp[3] = adc_char[3];
        temp[4] = '\0';

        char tempBL[11] = "BL="; // Temporary buffer to format the string
        dispPrint(strcat(tempBL,temp), '1');

        HexToBCD(black_high);
        char tempBH[11] = "BH="; // Temporary buffer to format the string
        // Format the string manually
        temp[0] = adc_char[0];
        temp[1] = adc_char[1];
        temp[2] = adc_char[2];
        temp[3] = adc_char[3];
        temp[4] = '\0';
        dispPrint(strcat(tempBH,temp), '2');

        strcpy(display_line[3], "          ");
        display_changed = TRUE;

        break;
    default: break;
    }
}

//-----------------------------------------------------------------
//-------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 9
//
//-------------------------------------------------------------
void adc_line(char line, char location){
    //-------------------------------------------------------------
    int i;
    unsigned int real_line;
    real_line = line - 1;
    for(i=0; i < 4; i++) {
        display_line[real_line][i+location] = adc_char[i];
    }
}
//-------------------------------------------------------------

