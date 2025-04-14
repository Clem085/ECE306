
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
#include "timersB0.h"

// Global Variables
extern volatile unsigned int Time_Sequence; // Already existing
extern volatile unsigned int switchpressed = OFF;
extern volatile unsigned int count_debounce_SW1 = OFF;
extern volatile unsigned int count_debounce_SW2 = OFF;
extern volatile unsigned int debounce_statesw1;
extern volatile unsigned int debounce_statesw2;
extern volatile unsigned char display_changed;
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char state;
volatile unsigned int pressed = 1;

unsigned int okay_to_look_at_switch1;
unsigned int sw1_position;
unsigned int okay_to_look_at_switch2;
unsigned int sw2_position;



// ENABLE SWITCHES

// ENABLE SW1
void enable_switch_SW1(void){
    P5OUT |=  SW1;
    P5DIR &= ~SW1;
}
// ENABLE SW2
void enable_switch_SW2(void){
    P5OUT |=  SW2;
    P5DIR &= ~SW2;
}
// ENABLE BOTH
void enable_switches(void){
    enable_switch_SW1();
    enable_switch_SW2();
}



// DISABLE SWITCHES

// DISABLE SW1
void disable_switch_SW1(void){
    P5OUT |=  SW1;
    P5DIR &= ~SW1;
}
// DISABLE SW2
void disable_switch_SW2(void){
    P5OUT |=  SW2;
    P5DIR &= ~SW2;
}
// DISABLE BOTH
void disable_switches(void){
    disable_switch_SW1();
    disable_switch_SW2();
}



void Debounce_State(void){
    if(debounce_statesw1){
        if(count_debounce_SW1 >= DEBOUNCE_TIME){
            debounce_statesw1 = OFF;
            enable_switches();
        }
    }
    if(debounce_statesw2){
        if(count_debounce_SW2 >= DEBOUNCE_TIME){
            debounce_statesw2 = OFF;
            enable_switches();
        }
    }
}



void Switches_Process(void){
//------------------------------------------------------------------------------
// This function calls the individual Switch Functions
//------------------------------------------------------------------------------
Switch1_Process();
Switch2_Process();
}

//------------------------------------------------------------------------------
void Switch1_Process(void){
//------------------------------------------------------------------------------
// Switch 1 Configurations
//------------------------------------------------------------------------------
    if (okay_to_look_at_switch1 && sw1_position){
        if (!(P4IN & SW1)){
            sw1_position = PRESSED;
            okay_to_look_at_switch1 = NOT_OKAY;
            count_debounce_SW1 = DEBOUNCE_RESTART;

            UCA0BRW = 4;                    // 115,200 baud
            UCA0MCTLW = 0x5551;

            strcpy(display_line[3], " 115200 b ");
            display_changed = TRUE;

//            receive_line(2, 1);
//            display_changed = TRUE;*/
//
//            if (shapes_count == 3){
//                shapes_count = 0;
//            }
//
//            switch(shapes_count){
//                case 0:
//                    event = CIRCLE;
//                    strcpy(display_line[0], "           ");
//                    strcpy(display_line[1], "   CIRCLE  ");
//                    strcpy(display_line[2], "           ");
//                    strcpy(display_line[3], "           ");
//                    break;
//                case 1:
//                    event = FIGURE;
//                    strcpy(display_line[0], "           ");
//                    strcpy(display_line[1], "  FIGURE8  ");
//                    strcpy(display_line[2], "           ");
//                    strcpy(display_line[3], "           ");
//                    break;
//                case 2:
//                    event = TRIANGLE;
//                    strcpy(display_line[0], "           ");
//                    strcpy(display_line[1], "  TRIANGLE ");
//                    strcpy(display_line[2], "           ");
//                    strcpy(display_line[3], "           ");
//                    break;
//                default:
//                    break;
//            }
//            shapes_count++;
        }
    }
    if (count_debounce_SW1 <= DEBOUNCE_TIME){
        count_debounce_SW1++;
    }else{
        okay_to_look_at_switch1 = OKAY;
        if (P4IN & SW1){
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
    if (okay_to_look_at_switch2 && sw2_position){
        if (!(P2IN & SW2)){
            sw2_position = PRESSED;
            okay_to_look_at_switch2 = NOT_OKAY;
            count_debounce_SW2 = DEBOUNCE_RESTART;

            /*UCA0BRW = 17;                   //460,800 baud
            UCA0MCTLW = 0x4A00;

            strcpy(display_line[3], " 460800 b ");
            display_changed = TRUE;

            receive_line(2, 1);
            display_changed = TRUE;*/

        }
    }
    if (count_debounce_SW2 <= DEBOUNCE_TIME){
        count_debounce_SW2++;
    }else{
        okay_to_look_at_switch2 = OKAY;
        if (P2IN & SW2){
            sw2_position = RELEASED;
        }
    }
}
//------------------------------------------------------------------------------


