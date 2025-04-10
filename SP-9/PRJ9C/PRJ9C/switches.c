/*
 * switches.c
 *
 *  Created on: Feb 11, 2025
 *      Author: price
 */

#include  "msp430.h"
#include  <string.h>
#include  "include\functions.h"
#include  "Include\LCD.h"
#include  "include\ports.h"
#include "macros.h"
#include  "include\shapes.h"

unsigned int sw1_position;
unsigned int okay_to_look_at_switch1;
unsigned int count_debounce_SW1;

unsigned int sw2_position = RELEASED;
unsigned int okay_to_look_at_switch2 = OKAY;
unsigned int count_debounce_SW2;
unsigned int shapes = 0;

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern unsigned int time_change;
extern char display_line[4][11];
extern char *display[4];

unsigned int start_pressed;
unsigned int debounce_count;

//void Switches_Process (void){
//    Switch1_Process();
//    Switch2_Process();
//}

//void Switch1_Process (void) {
//    //------------------------------------------------------------------------------
//    if (okay_to_look_at_switch1 && sw1_position){
//        if (!(P4IN & SW1)){
//            sw1_position = PRESSED;
//            okay_to_look_at_switch1 = NOT_OKAY;
//            count_debounce_SW1 = DEBOUNCE_RESTART;
//            shapes++;
//        }
//     }
//
//    if (count_debounce_SW1 <= DEBOUNCE_TIME){
//        count_debounce_SW1++;
//    }else{
//        okay_to_look_at_switch1 = OKAY;
//        if (P4IN & SW1){
//            sw1_position = RELEASED;
//         }
//     }
//
//}
//
//void Switch2_Process (void) {
//    if (okay_to_look_at_switch2 && sw2_position){
//        if (!(P2IN & SW2)){
//            sw2_position = PRESSED;
//            okay_to_look_at_switch2 = NOT_OKAY;
//            count_debounce_SW2 = DEBOUNCE_RESTART;
//
//   // do what you want with button press
//            start_pressed = 1;
//       }
//
//
//    }
//    if (count_debounce_SW2 <= DEBOUNCE_TIME){
//        count_debounce_SW2++;
//    }else{
//        okay_to_look_at_switch2 = OKAY;
//        if (P2IN & SW2){
//            sw2_position = RELEASED;
//        }
//     }
//}

