
//===========================================================================
// File Name : switches.c
//
// Description: This file contains the code for when a switch is pressed and
//
// Author: Kayla Radu
// Date: Sept 20, 2024
// Compiler: Built with Code Composer Studio Version: 12.8.0.00012


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "switches.h"
#include <stdio.h>

// Global Variables
extern volatile unsigned char event;  // (e.g., STRAIGHT, CIRCLE)
extern volatile unsigned int event_Counter;
extern volatile unsigned int Time_Sequence; // Already existing
extern volatile unsigned int state;
unsigned int okay_to_look_at_switch1;
unsigned int sw1_position;
unsigned int count_debounce_SW1;
unsigned int okay_to_look_at_switch2;
unsigned int sw2_position = 0;
unsigned int count_debounce_SW2;

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
unsigned char eventlcd;
extern volatile unsigned int triangleset;
extern volatile unsigned int figureeightset;
extern volatile unsigned int rdirection;
extern volatile unsigned int ldirection;
extern volatile unsigned int instruction;
volatile unsigned char origevent;


//------------------------------------------------------------------------------
void Switches_Process(void){
//------------------------------------------------------------------------------
// This function calls the individual Switch Functions
//------------------------------------------------------------------------------
Switch1_Process();
Switch2_Process();
}
//------------------------------------------------------------------------------





//------------------------------------------------------------------------------
void Switch1_Process(void){
//------------------------------------------------------------------------------
// Switch 1 Configurations
//------------------------------------------------------------------------------
    if (okay_to_look_at_switch1 && (sw1_position == RELEASED)) {  // Check if we are allowed to process the switch and if it was previously released
        if (!(P4IN & SW1)) {  // Check if Switch 1 is pressed (active low)
            sw1_position = PRESSED;
            okay_to_look_at_switch1 = NOT_OKAY;  // Prevent further processing until debounce
            count_debounce_SW1 = DEBOUNCE_RESTART;  // Start debouncing
            // This is going to change what action you are performing by incrementing the value



            // This is going to change the LCD Display
            switch (event_Counter) {
            case 0:
                eventlcd = STRAIGHT;
                event_Counter = 1;
                break;
            case 1:
                eventlcd = CIRCLE;
                event_Counter = 2;
                break;
            case 2:
                eventlcd = TRIANGLE;
                event_Counter = 3;
                break;
            case 3:
                eventlcd = FIGUREEIGHT;
                event_Counter = 4;
                break;
            case 4:
                eventlcd = PROJECTFIVE;
                event_Counter = 0;
                break;
            default:break;
            }


            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "          ");
            strcpy(display_line[3], "          ");

            char newline2[11]; // 10 characters
            sprintf(newline2, "     %c     ", eventlcd); // Fill with 5 spaces, the event character, and 5 more spaces
            strncpy(display_line[2], newline2, 10); // Copy exactly 10 characters into display_line[2]

            display_changed = TRUE;
        }
    }

    if (count_debounce_SW1 <= DEBOUNCE_TIME) {
        count_debounce_SW1++;  // Increment debounce counter
    } else {
        okay_to_look_at_switch1 = OKAY;  // Allow further checks on the switch
        if (P4IN & SW1) {  // Check if Switch 1 is released (active high)
            sw1_position = RELEASED;  // Set switch to RELEASED
        }
    }
}


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Switch2_Process(void){
//------------------------------------------------------------------------------
// Switch 2 Configurations
//------------------------------------------------------------------------------
    if (okay_to_look_at_switch2 && (sw2_position == RELEASED)){
        if (!(P2IN & SW2)){
            sw2_position = PRESSED;
            okay_to_look_at_switch2 = NOT_OKAY;
            count_debounce_SW2 = DEBOUNCE_RESTART;


            switch (event_Counter-1) {
            case 0:
                event = STRAIGHT;
                break;
            case 1:
                event = CIRCLE;
                break;
            case 2:
                event = TRIANGLE;
                triangleset = 1;
                break;
            case 3:
                event = FIGUREEIGHT;
                figureeightset = 1;
                break;
            case -1:
                rdirection = FORWARD;
                ldirection = FORWARD;
                event = PROJECTFIVE;
                origevent = PROJECTFIVE;
            default:break;
            }




    // do what you want with button press
        }
    }
    if (count_debounce_SW2 <= DEBOUNCE_TIME){
        count_debounce_SW2++;
    }
    else{
        okay_to_look_at_switch2 = OKAY;
        if (P2IN & SW2){
            sw2_position = RELEASED;
        }
    }
    }
//------------------------------------------------------------------------------
