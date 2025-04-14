
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
unsigned int baud_toggle;
char transmit_state;



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






#pragma vector = PORT4_VECTOR
__interrupt void switchP1_interrupt(void) {
    // Switch 1
    if (P4IFG & SW1) {


        P4IE &= ~SW1;                   // Disable Port Interrupt
        P4IFG &= ~SW1;                  // IFG SW1 cleared

        switchpressed = SW1;

        // Disable switches for debounce
        disable_switches();


        // Re-enable switches after debounce
        debounce_statesw1 = 1;
        count_debounce_SW1 = 0;

        TB0CCTL1 &= ~CCIFG;             // Clear SW1 debounce interrupt flag
        TB0CCR1 = TB0CCR1_INTERVAL;     // CCR1 add offset
        TB0CCTL1 |= CCIE;               // CCR1 enable interrupt

        //SW1 FUNCTIONS:

        //        TB0CCTL0 &= ~CCIE; // Disables timersB0
        //        P6OUT &= ~LCD_BACKLITE;
        switchpressed = ON;
        state = WAIT;


    }
    //-----------------------------------------------------------------------------
}

#pragma vector = PORT2_VECTOR

__interrupt void switchP2_interrupt(void){
    // Switch 2
    if (P2IFG & SW2) {
        P2IFG &= ~SW2;          // IFG SW2 cleared
        pressed = ~pressed;
        if(pressed){
            P2OUT |= IR_LED;
            strcpy(display_line[0], " IR ON    ");
        }
        else{
            P2OUT &= ~IR_LED;
            strcpy(display_line[0], " IR OFF   ");
        }

        disable_switches();

        display_changed = TRUE;

    }
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

