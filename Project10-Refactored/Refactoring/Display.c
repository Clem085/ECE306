/* Display Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : Display.c
  Description:  This file contains the code to control the LCD Display
  Programmer: Connor Savugot
  Date Created: Sep 20, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
*/

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

// Global Variables declared and referenced in Header file



// PWM Backlight Control
void PWM_backlight(void){
    double backlight_value;
    backlight_value = (light_percent / 100.00)*PERCENT_100;
    LCD_BACKLITE_BRIGHTNESS = backlight_value;
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

void Clear_Display(void){
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
    char temp[5] = {0}; // Ensure proper initialization
    char tempBL[11] = {0}; // Ensure proper initialization
    char tempBH[11] = {0}; // Ensure proper initialization

    switch (ir_setting){
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
        black_low = (ADC_Left_Detect + ADC_Right_Detect) / 2 - 50;
        black_high = (ADC_Left_Detect + ADC_Right_Detect) / 2 + 50;

        // Format and display black_low
        HexToBCD(black_low);
        temp[0] = adc_char[0];
        temp[1] = adc_char[1];
        temp[2] = adc_char[2];
        temp[3] = adc_char[3];
        temp[4] = '\0';

        strcpy(tempBL, "BL="); // Reset before concatenation
        strcat(tempBL, temp); // Concatenate formatted string
        dispPrint(tempBL, '1');

        // Format and display black_high
        HexToBCD(black_high);
        temp[0] = adc_char[0];
        temp[1] = adc_char[1];
        temp[2] = adc_char[2];
        temp[3] = adc_char[3];
        temp[4] = '\0';

        strcpy(tempBH, "BH="); // Reset before concatenation
        strcat(tempBH, temp); // Concatenate formatted string
        dispPrint(tempBH, '2');

        strcpy(display_line[3], "          ");
        display_changed = TRUE;
        break;
    default:
        break;
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
    for(i=0; i < 4; i++){
        display_line[real_line][i+location] = adc_char[i];
    }
}
//-------------------------------------------------------------





// Aligns text to Middle of Display
void dispPrint(char *line, char lineToUpdate){
    char tempLine[11]; // Temporary line buffer
    int lineIndex = -1; // Index of the line to be updated

    // Determine which line to update based on the input character (1-4)
    switch (lineToUpdate){
    case '1':
        lineIndex = 0;
        break;
    case '2':
        lineIndex = 1;
        break;
    case '3':
        lineIndex = 2;
        break;
    case '4':
        lineIndex = 3;
        break;
    default:
        // Handle invalid input
        strcpy(display_line[0], "-Invalid--");
        display_changed = TRUE;
        return;
    }

    // If the provided line is not NULL, update the corresponding display line
    if(line != NULL){
        int spaces = (10 - strlen(line)) >> 1; // Calculate spaces for centering
        int i;
        // Initialize tempLine with spaces
        for (i = 0; i < 10; i++){
            tempLine[i] = ' ';
        }
        tempLine[10] = '\0'; // Null-terminate

        // Copy the line string into the center of tempLine
        strncpy(tempLine + spaces, line, strlen(line));

        // Ensure the string is null-terminated
        tempLine[10] = '\0'; // Null-terminate explicitly

        // Copy the temporary line to the corresponding display line
        strcpy(display_line[lineIndex], tempLine);

        // Indicate that the display has changed
        display_changed = TRUE;
    } else {
        // Handle null parameters by displaying an error message on the selected line
        strcpy(display_line[lineIndex], " NULL Line ");
        display_changed = TRUE;
    }
}
