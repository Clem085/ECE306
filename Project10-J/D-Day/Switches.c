/* Switches Program Information Header
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : switches.c
  Description:  This file contains the code to control the 2 buttons (SW1 & SW2) on the MSP430 Board
  Programmer: Connor Savugot
//  Date Created: Sep 20, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
*/

/////////////// DEPRECATED CODE /////////////// DEPRECATED CODE ///////////////
// This code was used BEFORE ISR's were implemented to better handle switches
/////////////// DEPRECATED CODE /////////////// DEPRECATED CODE ///////////////

// #include as of 11-27-24
//// Header Files
#include "ADC.h"
#include "DAC.h"
#include "Display.h"
#include "functions.h"
#include "interrupts.h"
#include "IR.h"
#include "IOT.h"
#include "LCD.h"
#include "macros.h"
#include "menu.h"
#include "motors.h"
#include "msp430.h"
#include "ports.h"
#include "serial.h"
#include "switches.h"
#include "timers.h"
//// Libraries
#include <stdio.h>
#include <string.h>

// Global Variables declared and referenced in Header file


//Variable declarations==========================================================
unsigned int okay_to_look_at_switch1;
unsigned int sw1_position;
unsigned int count_debounce_SW1;
unsigned int okay_to_look_at_switch2;
unsigned int sw2_position;
unsigned int count_debounce_SW2;
unsigned int shapes_count;
extern char display_line[4][11];
extern char *display[4];
unsigned int event;
unsigned char display_changed;

//===============================================================================

//Function declarations==========================================================

void Switches_Process(void);
void Switch1_Process(void);
void Switch2_Process(void);

//===============================================================================

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
    if(okay_to_look_at_switch1 && sw1_position){
        if(!(P4IN & SW1)){
            sw1_position = PRESSED;
            okay_to_look_at_switch1 = NOT_OKAY;
            count_debounce_SW1 = DEBOUNCE_RESTART;

            UCA0BRW = 4;                    // 115,200 baud
            UCA0MCTLW = 0x5551;

            strcpy(display_line[3], " 115200 b ");
            display_changed = TRUE;

        }
    }
    if(count_debounce_SW1 <= DEBOUNCE_TIME){
        count_debounce_SW1++;
    }else{
        okay_to_look_at_switch1 = OKAY;
        if(P4IN & SW1){
            sw1_position = RELEASED;
        }
    }
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void Switch2_Process(void){
//------------------------------------------------------------------------------
// Switch 2 Configurations
//------------------------------------------------------------------------------
    if(okay_to_look_at_switch2 && sw2_position){
        if(!(P2IN & SW2)){
            sw2_position = PRESSED;
            okay_to_look_at_switch2 = NOT_OKAY;
            count_debounce_SW2 = DEBOUNCE_RESTART;

        }
    }
    if(count_debounce_SW2 <= DEBOUNCE_TIME){
        count_debounce_SW2++;
    }else{
        okay_to_look_at_switch2 = OKAY;
        if(P2IN & SW2){
            sw2_position = RELEASED;
        }
    }
}
//------------------------------------------------------------------------------

