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
#include <switches.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "motors.h"
#include  "Display.h"
#include  "timersB0.h"
#include  "ThumbWheel.h"
#include <string.h>


// Globals
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern unsigned int backlight_status;
extern int Switch_Counter1;

char tempLine0[11];
char tempLine1[11];
char tempLine2[11];
char tempLine3[11];

extern char adc_char[10];

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
    }
    else{
        P6OUT  |=  LCD_BACKLITE;
    }
}




// Dummy display_line array to hold display values
//char display_line[4][10 + 1]; // +1 for null terminator

// Function Definition
//void dispPrint(char *line0, char *line1, char *line2, char *line3) {
//    char tempLine[11]; // Temporary line buffer
//    if (line0 != NULL && line1 != NULL && line2 != NULL && line3 != NULL) {
//        // Process each line
//        char *lines[] = { line0, line1, line2, line3 };
//        int lineIndex;
//        for (lineIndex = 0; lineIndex < 4; lineIndex++) {
//            int spaces = (10 - strlen(lines[lineIndex])) >> 1;// Calculate spaces needed for centering the string
//
//            int i;// Initialize tempLine with spaces using a loop
//            for (i = 0; i < 10; i++) {
//                tempLine[i] = ' '; // Fill tempLine with spaces
//            }
//            tempLine[10] = '\0'; // Null-terminate
//
//            // Copy the line string into the center of tempLine
//            strncpy(tempLine + spaces, lines[lineIndex], strlen(lines[lineIndex]));
//
//            // Ensure the string is null-terminated
//            tempLine[10] = '\0'; // Explicitly null-terminate again after copying
//
//            // Copy the temporary line to display_line
//            strcpy(display_line[lineIndex], tempLine);
//        }
//        display_changed = TRUE;
//    } else {
//        // Handle null parameters
//        strcpy(display_line[0], " Error!!! ");
//        strcpy(display_line[1], "dispPrint ");
//        strcpy(display_line[2], "InvldParam");
//        strcpy(display_line[3], "line=NULL ");
//        display_changed = TRUE;
//    }
//}


void dispPrint(char *line, char lineToUpdate) {
    char tempLine[11]; // Temporary line buffer
    int lineIndex = -1; // Index of the line to be updated

    // Determine which line to update based on the input character (1-4)
    switch (lineToUpdate) {
        case 1:
            lineIndex = 0;
            break;
        case 2:
            lineIndex = 1;
            break;
        case 3:
            lineIndex = 2;
            break;
        case 4:
            lineIndex = 3;
            break;
        default:
            // Handle invalid input
            strcpy(display_line[0], " Invalid  ");
            display_changed = TRUE;
            return;
    }

    // If the provided line is not NULL, update the corresponding display line
    if (line != NULL) {
        int spaces = (10 - strlen(line)) >> 1; // Calculate spaces for centering
        int i;
        // Initialize tempLine with spaces
        for (i = 0; i < 10; i++) {
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













//
//int middleChar(char * string){
//    int spaces = (10 - strlen(string)) >> 1;// Calculate spaces needed for centering the string
//    return spaces;
//}

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
    for(i=0; i < 10; i++) {
        if(i >= location && i <= location+strlen(adc_char)){
            display_line[real_line][i+location] = adc_char[i];
        }else{
            display_line[real_line][i+location] = ' ';
        }
    }
}
//-------------------------------------------------------------
