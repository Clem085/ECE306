/*
 * Interrupt_Ports.c
 *
 *  Created on: Nov 11, 2024
 *      Author: kaylaradu
 */




/*
 * Ports_Interrupts.c
 *
 *  Created on: Feb 22, 2024
 *      Author: varun
 */

//------------------------------------------------------------------------------

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "switches.h"
#include <stdio.h>
#include "timersB0.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//variable declarations
unsigned int display_count;
volatile unsigned char display_changed;
extern char display_line[4][11];
volatile unsigned char update_display;
volatile char one_time;
volatile unsigned int Time_Sequence;
unsigned int tmr_ms_count;
unsigned int blink_count;
unsigned int event;
unsigned int state;
unsigned int t=0;
extern unsigned int clear_display;
extern char NCSU_str[10] = "NCSU  #1";
char display_line[4][11];
unsigned int baud_toggle;
char transmit_state;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Port 4 interrupt for switch 1, it is disabled for the duration
// of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT4_VECTOR
__interrupt void switch1_interrupt(void) {
// Switch 1
    if (P4IFG & SW1) {
        P4IE &= ~SW1;//Disable Switch 1
        P4IFG &= ~SW1;//Clear Switch 1 Flag
        TB0CCTL1 &= ~CCIFG; //Clear TimerB0 Interrupt Flag for Capture Compare Register 1
        TB0CCR1 = TB0R + TB0CCR1_INTERVAL;//Add Interval to TB0R for TB0CCR1
        TB0CCTL1 |= CCIE;//Enable TimerB0_1
        //P6OUT &= ~LCD_BACKLITE;
        TB0CCTL0 &= ~CCIE;
        P2OUT &= ~IR_LED; // Initial Value = Low / Off
        P2DIR |= IR_LED; // Direction = input

        transmit_state = TRANSMIT;


        clear_display = 1;
         USCI_A0_transmit();
//        while(NCSU_str[t] != '\0'){
//            UCA0_transmit(NCSU_str[t]);
//            t++;
//        }
        //            UCA0_transmit(NCSU_str[t]);
//        t = 0;


    }
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
// Port 2 interrupt for switch 2, it is disabled for the duration
// of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT2_VECTOR
__interrupt void switch2_interrupt(void) {
// Switch 2
    if (P2IFG & SW2) {
        P2IE &= ~SW2;//Disable Switch 1
        P2IFG &= ~SW2;//Clear Switch 1 Flag
        TB0CCTL2 &= ~CCIFG; //Clear TimerB0 Interrupt Flag for Capture Compare Register 1
        TB0CCR2 = TB0R + TB0CCR2_INTERVAL;//Add Interval to TB0R for TB0CCR1
        TB0CCTL2 |= CCIE;//Enable TimerB0_1
        P6OUT &= ~LCD_BACKLITE;
        TB0CCTL0 &= ~CCIE;
        /*P2OUT |= IR_LED; // Initial Value = Low / Off
        P2DIR |= IR_LED; // Direction = input*/
        state = SEARCH;

        if (baud_toggle) {
            baud_toggle = 0;
            UCA0BRW = 17;                   //460,800 baud
            UCA0MCTLW = 0x4A00;
        }
        else {
            baud_toggle = 1;
            UCA0BRW = 4;                    // 115,200 baud
            UCA0MCTLW = 0x5551;
        }



        clear_display = 1;
//        USCI_A0_transmit();
//        USCI_A0_transmit();
//        while(NCSU_str[t] != '\0'){
//            UCA0_transmit(NCSU_str[t]);
//            t++;
//        }
//        //            UCA0_transmit(NCSU_str[t]);
//        t = 0;

        //display_changed = TRUE;
        //Disable Switch 2
        //Clear Switch 2 Flag
        //Clear TimerB0 Interrupt Flag for Capture Compare Register 2
        //Add Interval to TB0R for TB0CCR2
        //Enable TimerB0_2
    }
//------------------------------------------------------------------------------
}


