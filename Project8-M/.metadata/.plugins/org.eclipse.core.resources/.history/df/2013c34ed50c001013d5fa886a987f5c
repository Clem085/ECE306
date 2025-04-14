// File Name : init.c
//
// Description: This file contains the Initialization for conditions
//
// Created on: Jan, 2025
// Author: Molana Torabi

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "ports.h"

// Global variables for managing the display and display modes
extern char display_line[4][11];  // 2D array to hold the content for each line of the LCD
extern char *display[4];          // Pointers to each line of the display for easier manipulation
unsigned char display_mode;       // Display mode variable for switching between different display states
extern volatile unsigned char display_changed;  // Flag indicating if the display content has changed
extern volatile unsigned char update_display;   // Flag for triggering an update to the display

// Function to initialize the conditions for display content and interrupts
void Init_Conditions(void){
    //------------------------------------------------------------------------------
    int i;

    // Initialize display_line array to reset all characters (clear display)
    for(i = 0; i < 11; i++){
        display_line[0][i] = RESET_STATE;  // Reset first line
        display_line[1][i] = RESET_STATE;
        display_line[2][i] = RESET_STATE;
        display_line[3][i] = RESET_STATE;
    }

    // Null-terminate the string at index 10 for each line (max length 10)
    display_line[0][10] = 0;
    display_line[1][10] = 0;
    display_line[2][10] = 0;
    display_line[3][10] = 0;

    // Assign pointers for easy access to the display lines
    display[0] = &display_line[0][0];
    display[1] = &display_line[1][0];
    display[2] = &display_line[2][0];
    display[3] = &display_line[3][0];

    // Initialize the flag to indicate no update is required initially
    update_display = 0;

    // Enable interrupts for the system
    enable_interrupts();
}
