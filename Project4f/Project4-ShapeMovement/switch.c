/* Program Information Header
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : switches.c
  Description:  This file contains the code to control the 3 switches on the MSP430 Board
      >>> As of now, Switch 1 (S1) is used to select a shape to draw. Switch 2 (S2) is used to confirm the Shape. Switch 1 (S1) is unused.
  Programmer: Connor Savugot
  Date: Sep 20, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */
// Includes
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

// Globals
extern unsigned char dispEvent;
extern volatile unsigned char display_changed;
extern unsigned char event;
extern char display_line[4][11];

extern unsigned int straight_step;
extern unsigned int circle_step;
extern unsigned int circle_step2;
extern unsigned int triangle_step;
extern unsigned int figure8_step;

extern unsigned char state;

// TEST
extern int Switch1_Pressed;
extern int Switch2_Pressed;
extern int Switch_Counter1;
extern int Switch_Counter2;
int okay_to_look_at_switch1=1;
int count_debounce_SW1;
int sw1_position=1;
int okay_to_look_at_switch2=1;
int count_debounce_SW2;
int sw2_position=1;
extern volatile unsigned int debounce_count1;
extern volatile unsigned int debounce_count2;
extern unsigned int backlight_status;



//// Physical Buttons
void Switch1_Process(void){
    //-----------------------------------------------------------------------------
    // Switch 1 Configurations
    //-----------------------------------------------------------------------------
    if (okay_to_look_at_switch1 && sw1_position){
        if (!(P4IN & SW1)){
            sw1_position = PRESSED;
            okay_to_look_at_switch1 = NOT_OKAY;
            count_debounce_SW1 = DEBOUNCE_RESTART;
            // Event Code
            backlight_status = 1;
            switch(dispEvent){
            case STRAIGHT:
                dispEvent = CIRCLE;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "- Select -");
                strcpy(display_line[2], "  Circle  ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                break;
            case CIRCLE:
                dispEvent = TRIANGLE;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "- Select -");
                strcpy(display_line[2], " Triangle ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                break;
            case TRIANGLE:
                dispEvent = FIGURE8;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "- Select -");
                strcpy(display_line[2], " Figure 8 ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                break;
            case FIGURE8:
                dispEvent = STRAIGHT;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "- Select -");
                strcpy(display_line[2], " Straight ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                break;
            case NONE:
                dispEvent  = STRAIGHT;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "- Select -");
                strcpy(display_line[2], " Straight ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
            default:
                break;
            }


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


void Switch2_Process(void){
    //-----------------------------------------------------------------------------
    // Switch 2 Configurations
    //-----------------------------------------------------------------------------
    if (okay_to_look_at_switch2 && sw2_position){
        if (!(P2IN & SW2)){
            sw2_position = PRESSED;
            okay_to_look_at_switch2 = NOT_OKAY;
            count_debounce_SW2 = DEBOUNCE_RESTART;
            // Event Code
            backlight_status = 0;
            state = WAIT;
            switch(dispEvent){
            case STRAIGHT:
                event = STRAIGHT;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "  Running ");
                strcpy(display_line[2], " Straight ");
                strcpy(display_line[3], "          ");
                straight_step = 1;
                break;
            case CIRCLE:
                event = CIRCLE;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "  Running ");
                strcpy(display_line[2], "  Circle  ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                dispEvent = NONE;
                circle_step = 1;
                circle_step2 = 1;
                break;
            case TRIANGLE:
                event = TRIANGLE;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "  Running ");
                strcpy(display_line[2], " Triangle ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                dispEvent = NONE;
                triangle_step = 1;
                break;
            case FIGURE8:
                event = FIGURE8;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "  Running ");
                strcpy(display_line[2], " Figure 8 ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                dispEvent = NONE;
                figure8_step = 1;
                break;
            case NONE:
                LRmotorStop();
                break;
            default:
                break;
            }

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


