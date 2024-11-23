

#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "strings.h"

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
volatile unsigned char display_changed;

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

