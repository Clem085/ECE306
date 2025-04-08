/*
 * switches.c
 *
 *  Created on: Sep 29, 2024
 *      Author: corin
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

volatile int okay_to_look_at_switch1 = OKAY;
volatile int okay_to_look_at_switch2 = OKAY;
volatile int sw1_position = RELEASED;
volatile int sw2_position = RELEASED;
volatile int sw1_counter = 0;
volatile int sw2_counter = 0;
volatile int count_debounce_SW1 = DEBOUNCE_RESTART;
volatile int count_debounce_SW2 = DEBOUNCE_RESTART;

const unsigned int DEBOUNCE_TIME = 100;


//------------------------------------------------------------------------------

// runs switch1 and switch2 processes
void Switch_Process(void) {
    Switch1_Process();
    Switch2_Process();
}

// Switch 1 Configurations. Controls whether or not the switch can be pressed
void Switch1_Process(void){
    if (okay_to_look_at_switch1) {
        if (!(P4IN & SW1)) {
            sw1_position = PRESSED;
        }
    }
}


            // okay_to_look_at_switch1 = NOT_OKAY;
            // count_debounce_SW1 = DEBOUNCE_RESTART;
            // do what you want with button press
    // } if (count_debounce_SW1 <= DEBOUNCE_TIME) {
        // count_debounce_SW1++;
    // } else {
        // okay_to_look_at_switch1 = OKAY;
        // if (P4IN & SW1) {
            // sw1_position = RELEASED;
        // }
   // }

// Switch 2 Configurations
void Switch2_Process(void){
    if (okay_to_look_at_switch2) {
        if (!(P2IN & SW2)) {
            sw2_position = PRESSED;
            okay_to_look_at_switch2 = NOT_OKAY;
            count_debounce_SW2 = DEBOUNCE_RESTART;
            sw2_counter += 1;
            if (sw2_counter == 4) {
                sw2_counter = 1;
            }
            Display_Change(sw2_counter);

        }
    }

    // Debouncing logic
    if(count_debounce_SW2 <= DEBOUNCE_TIME){                        // Increment the debounce timer to create a delay for the switch being pressed
       count_debounce_SW2++;
    } else {
        okay_to_look_at_switch2 = OKAY;
       if (P2IN & SW2) {
            sw2_position = RELEASED;
        }
    }
}



