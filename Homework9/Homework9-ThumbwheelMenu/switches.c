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

// #include as of 11-10-24
//// Header Files
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "motors.h"
#include  "Display.h"
#include  "timers.h"
#include  "interrupts.h"
#include  "switches.h"
#include  "ADC.h"
#include  "IR.h"
#include  "serial.h"
#include  "DAC.h"
#include  "menu.h"
//// Libraries
#include  <string.h>
#include  <stdio.h>

// Global Variables declared and referenced in Header file


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
extern char backlight_status;
extern volatile unsigned int Time_Sequence;
extern int activateSM;


// Debounce Vars
//char debounce_Status_SW1 = OFF;
//char debounce_Status_SW2 = OFF;
unsigned int count_debounce_SW1;
unsigned int count_debounce_SW2;


//// ENABLE SWITCHES
//// ENABLE SW1
//void enable_switch_SW1(void){
//    P5OUT |=  SW1;
//    P5DIR &= ~SW1;
//}
//// ENABLE SW2
//void enable_switch_SW2(void){
//    P5OUT |=  SW2;
//    P5DIR &= ~SW2;
//}
//// ENABLE BOTH
//void enable_switches(void){
//    enable_switch_SW1();
//    enable_switch_SW2();
//}
//
//
//
//// DISABLE SWITCHES
//// DISABLE SW1
//void disable_switch_SW1(void){
//    P5OUT |=  SW1;
//    P5DIR &= ~SW1;
//}
//// DISABLE SW2
//void disable_switch_SW2(void){
//    P5OUT |=  SW2;
//    P5DIR &= ~SW2;
//}
//// DISABLE BOTH
//void disable_switches(void){
//    disable_switch_SW1();
//    disable_switch_SW2();
//}
//





//void debounce(void){
//    debounceSW1();
//    debounceSW2();
//}
//
//void debounceSW1(void){
//    if(debounce_Status_SW1 == ON){
//        if(count_debounce_SW1 > DEBOUNCE_TIME){
//            debounce_Status_SW1 = OFF;
//            enable_switch_SW1();
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "  Press a ");
//            strcpy(display_line[2], " Switch to");
//            strcpy(display_line[3], "   Begin  ");
//            display_changed = TRUE;
//        }else{
//            backlight_status = 1;
//            // Update Display
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "          ");
//            strcpy(display_line[2], "          ");
//            strcpy(display_line[3], "  Switch1 ");
//            display_changed = TRUE;
//            disable_switch_SW2();
//        }
//    }else{
//        enable_switch_SW1();
//    }
//}
//
//
//void debounceSW2(void){
//    if(debounce_Status_SW2 == ON){
//        if(count_debounce_SW2 > DEBOUNCE_TIME){
//            debounce_Status_SW2 = OFF;
//            enable_switch_SW2();
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "  Press a ");
//            strcpy(display_line[2], " Switch to");
//            strcpy(display_line[3], "   Begin  ");
//            display_changed = TRUE;
//        }else{
//            backlight_status = 0;
//            // Update Display
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "          ");
//            strcpy(display_line[2], "          ");
//            strcpy(display_line[3], "  Switch2 ");
//            display_changed = TRUE;
//            disable_switch_SW2();
//        }
//    }else{
//        enable_switch_SW2();
//    }
//}


























//// Physical Buttons
// void Switch1_Shape_Process(void){
//     //-----------------------------------------------------------------------------
//     // Switch 1 Configurations
//     //-----------------------------------------------------------------------------
//     if (okay_to_look_at_switch1 && sw1_position){
//         if (!(P4IN & SW1)){
//             sw1_position = PRESSED;
//             okay_to_look_at_switch1 = NOT_OKAY;
//             count_debounce_SW1 = DEBOUNCE_RESTART;
//             // Event Code
//             if(event == NONE){
//                 backlight_status = 1;
//                 switch(dispEvent){
//                 case STRAIGHT:
//                     dispEvent = CIRCLE;
//                     strcpy(display_line[0], "          ");
//                     strcpy(display_line[1], "- Select -");
//                     strcpy(display_line[2], "  Circle  ");
//                     strcpy(display_line[3], "          ");
//                     display_changed = TRUE;
//                     break;
//                 case CIRCLE:
//                     dispEvent = TRIANGLE;
//                     strcpy(display_line[0], "          ");
//                     strcpy(display_line[1], "- Select -");
//                     strcpy(display_line[2], " Triangle ");
//                     strcpy(display_line[3], "          ");
//                     display_changed = TRUE;
//                     break;
//                 case TRIANGLE:
//                     dispEvent = FIGURE8C1;
//                     strcpy(display_line[0], "          ");
//                     strcpy(display_line[1], "- Select -");
//                     strcpy(display_line[2], " Figure 8 ");
//                     strcpy(display_line[3], "          ");
//                     display_changed = TRUE;
//                     break;
//                 case FIGURE8C1:
//                     dispEvent = STRAIGHT;
//                     strcpy(display_line[0], "          ");
//                     strcpy(display_line[1], "- Select -");
//                     strcpy(display_line[2], " Straight ");
//                     strcpy(display_line[3], "          ");
//                     display_changed = TRUE;
//                     break;
//                 case NONE:
//                     dispEvent  = STRAIGHT;
//                     strcpy(display_line[0], "          ");
//                     strcpy(display_line[1], "- Select -");
//                     strcpy(display_line[2], " Straight ");
//                     strcpy(display_line[3], "          ");
//                     display_changed = TRUE;
//                 default:
//                     break;
//                 }

//             }



//         }
//     }
//     if (count_debounce_SW1 <= DEBOUNCE_TIME){
//         count_debounce_SW1++;
//     }else{
//         okay_to_look_at_switch1 = OKAY;
//         if (P4IN & SW1){
//             sw1_position = RELEASED;
//         }
//     }
// }


// void Switch2_Shape_Process(void){
//     //-----------------------------------------------------------------------------
//     // Switch 2 Configurations
//     //-----------------------------------------------------------------------------
//     if (okay_to_look_at_switch2 && sw2_position){
//         if (!(P2IN & SW2)){
//             sw2_position = PRESSED;
//             okay_to_look_at_switch2 = NOT_OKAY;
//             count_debounce_SW2 = DEBOUNCE_RESTART;
//             // Event Code
//             if(event == NONE){
//                 backlight_status = 0;
//                 state = WAIT;
//                 switch(dispEvent){
//                 case STRAIGHT:
//                     event = STRAIGHT;
//                     strcpy(display_line[0], "          ");
//                     strcpy(display_line[1], "  Running ");
//                     strcpy(display_line[2], " Straight ");
//                     strcpy(display_line[3], "          ");
//                     straight_step = 1;
//                     break;
//                 case CIRCLE:
//                     event = CIRCLE;
//                     strcpy(display_line[0], "          ");
//                     strcpy(display_line[1], "  Running ");
//                     strcpy(display_line[2], "  Circle  ");
//                     strcpy(display_line[3], "          ");
//                     display_changed = TRUE;
//                     dispEvent = NONE;
//                     circle_step = 1;
//                     circle_step2 = 1;
//                     break;
//                 case TRIANGLE:
//                     event = TRIANGLE;
//                     strcpy(display_line[0], "          ");
//                     strcpy(display_line[1], "  Running ");
//                     strcpy(display_line[2], " Triangle ");
//                     strcpy(display_line[3], "          ");
//                     display_changed = TRUE;
//                     dispEvent = NONE;
//                     triangle_step = 1;
//                     break;
//                 case FIGURE8C1:
//                     event = FIGURE8C1;
//                     strcpy(display_line[0], "          ");
//                     strcpy(display_line[1], "  Running ");
//                     strcpy(display_line[2], " Figure 8 ");
//                     strcpy(display_line[3], "          ");
//                     display_changed = TRUE;
//                     dispEvent = NONE;
//                     figure8_step = 1;
//                     break;
//                 case NONE:
//                     LRmotorStop();
//                     break;
//                 default:
//                     break;
//                 }
//             }
//         }
//     }
//     if (count_debounce_SW2 <= DEBOUNCE_TIME){
//         count_debounce_SW2++;
//     }else{
//         okay_to_look_at_switch2 = OKAY;
//         if (P2IN & SW2){
//             sw2_position = RELEASED;
//         }
//     }
// }

//void Switch1_Proj5_Process(void){
//    //-----------------------------------------------------------------------------
//    // Switch 1 Configurations
//    //-----------------------------------------------------------------------------
//    if (okay_to_look_at_switch1 && sw1_position){
//        if (!(P4IN & SW1)){
//            sw1_position = PRESSED;
//            okay_to_look_at_switch1 = NOT_OKAY;
//            count_debounce_SW1 = DEBOUNCE_RESTART;
//            // Event Code
//            backlight_status = 1;
//            state = WAIT;
//
//        }
//    }
//    if (count_debounce_SW1 <= DEBOUNCE_TIME){
//        count_debounce_SW1++;
//    }else{
//        okay_to_look_at_switch1 = OKAY;
//        if (P4IN & SW1){
//            sw1_position = RELEASED;
//        }
//    }
//}
