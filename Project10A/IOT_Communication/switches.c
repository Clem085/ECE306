/* Switches Program Information Header
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : switches.c
  Description:  This file contains the code to control the 2 buttons (SW1 & SW2) on the MSP430 Board
  Programmer: Connor Savugot
//  Date Created: Sep 20, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */

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





//// Backlight toggle state
volatile unsigned char backlight_on = 1;
// SW1 debounce active flag
volatile char switch_debounce_active = 0; // Bitmask for debounce status









//------------------------------------------------------------------------------
//  Description: Disables siwtch interrupts
//------------------------------------------------------------------------------
void disable_switch_interrupts(void){
    P4IE &= ~SW1;
    P2IE &= ~SW2;
}

//------------------------------------------------------------------------------
//  Description: Enables siwtch interrupts
//------------------------------------------------------------------------------
void enable_switch_interrupts(void){
    P4IE |= SW1;
    P2IE |= SW2;
}

void Forward_On(void){ //Sets the two port 6 outputs to high for forward direction
    P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
    P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]
}

////------------------------------------------------------------------------------
////  Description: SW1 interrupt handler (Port 4)
////------------------------------------------------------------------------------
//#pragma vector = PORT4_VECTOR
//__interrupt void switchP4_interrupt(void) {
//    if ((P4IFG & SW1) && (switch_debounce_active == 0)) {  // Check if SW1 caused the interrupt
//        switch1_pressed = 1;
//        //localCounter = 0;
//        localCounter++;
//
//        debounce_count1 = 0;  // Reset SW1 debounce counter
//        switch_debounce_active = 1;  // Start SW1 debounce
//
//        TB0CCR1 = TB0R + increment;  // ~100ms debounce interval for SW1
//        TB0CCTL1 |= CCIE;  // Enable CCR1 interrupt for SW1 debounce
//
//        P4IFG &= ~SW1;  // Clear SW1 interrupt flag
//
//        disable_switch_interrupts();
//        //       P1OUT ^= RED_LED;
//    }
//}
//
////------------------------------------------------------------------------------
////  Description: SW2 interrupt handler (Port 2)
////------------------------------------------------------------------------------
//#pragma vector = PORT2_VECTOR
//__interrupt void switchP2_interrupt(void) {
//    if ((P2IFG & SW2) && (switch_debounce_active == 0)) {  // Check if SW2 caused the interrupt
//        switch2_pressed = 1;
//        localCounter++;
//
//        debounce_count2 = 0;  // Reset SW2 debounce counter
//        switch_debounce_active = 1;  // Start SW2 debounce
//
//        TB0CCR2 = TB0R + increment;  // ~100ms debounce interval for SW2
//        TB0CCTL2 |= CCIE;  // Enable CCR2 interrupt for SW2 debounce
//
//        P2IFG &= ~SW2;  // Clear SW2 interrupt flag
//
//        //disable_switch_interrupts();
//        //        P6OUT ^= GRN_LED;
//    }
//}

////------------------------------------------------------------------------------
////  Description: Timer B0 CCR0 ISR - Backlight Blinking
////------------------------------------------------------------------------------
//#pragma vector = TIMER0_B0_VECTOR
//__interrupt void Timer0_B0_ISR(void) {
//    if(debounce_count1 >= 10){
//        debounce_count1 = 0;
//    }
//    localCounter++;  // Increment for every interrupt (~8 ms or as configured)
//
//    // // // // // // // // // // Added due to Timer usage in interrupts.c // // // // // // // // // //
//    update_display = TRUE;
//    Time_Sequence++;
//    // // // // // // // // // //
//    TB0CCR0 += TB0CCR0_INTERVAL;
//}

//#pragma vector=TIMER0_B1_VECTOR
//__interrupt void TIMER0_B1_ISR(void){
//    //----------------------------------------------------------------------------
//    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//    //----------------------------------------------------------------------------
//
//    switch (__even_in_range(TB0IV, 14)) {
//    case 2:  // SW1 debounce completion
//        debounce_count1++;
//        if (debounce_count1 >= 10) {  // Adjust as needed
//            switch_debounce_active = 0;  // End debounce
//            //enable_switch_interrupts();
//            TB0CCTL1 &= ~CCIE;  // Disable debounce interrupt
//        }
//        break;
//
//    case 4:  // SW2 debounce completion
//        debounce_count2++;
//        if (debounce_count2 >= 10) {
//            switch_debounce_active = 0;
//            enable_switch_interrupts();
//            TB0CCTL2 &= ~CCIE;
//        }
//        break;
//
//    default: break;
//    }
//    switch(__even_in_range(TB0IV,14)){
//    case 0: break; // No interrupt
//    case 2: // CCR1 not used
//        //...... Add What you need happen in the interrupt ......
//        TB0CCR1 += TB0CCR1; // Add Offset to TBCCR1
//        break;
//    case 4: // CCR2 not used
//        //...... Add What you need happen in the interrupt ......
//        TB0CCR2 += TB0CCR2; // Add Offset to TBCCR2
//        break;
//    case 14: // overflow
//        //...... Add What you need happen in the interrupt ......
//        break;
//    default: break;
//    }
//}
//---------------------------------------------------------------------------

////------------------------------------------------------------------------------
////  Description: Timer B0 CCR1 and CCR2 ISR - Debounce Handling
////------------------------------------------------------------------------------
//#pragma vector = TIMER0_B1_VECTOR
//__interrupt void Timer0_B1_ISR(void) {
//    switch (TB0IV) {
//    case 0x00: // No pending interrupt
//        break;
//    case TB0IV_TBCCR1:  // 0x02 // SW1 debounce completion
//        debounce_count1++;
//        if (debounce_count1 >= 12) {  // ~1 second debounce time // used to be 12
//            switch1_pressed = 0;
//            switch_debounce_active = 0;  // End SW1 debounce
//            //enable_switch_interrupts();
//            TB0CCTL1 &= ~CCIE;  // Disable CCR1 interrupt
//
//            // Re-enable backlight blinking timer
//            TB0CCTL0 |= CCIE;
//
//
//        }
//
//        // DAC
//        if(DAC_data > 900){
//            DAC_data -= 50;
//            SAC3DAT = DAC_data;
//        }
//        break;
//
//    case TB0IV_TBCCR2:  // 0x04 // SW2 debounce completion
//        debounce_count2++;
//        if (debounce_count2 >= 12) {  // ~1 second debounce time // Used to be 12
//            switch2_pressed = 0;
//            switch_debounce_active = 0;  // End SW2 debounce
//            //enable_switch_interrupts();
//            TB0CCTL2 &= ~CCIE;  // Disable CCR2 interrupt
//
//            // Re-enable backlight blinking timer
//            TB0CCTL0 |= CCIE;
//        }
//        break;
//
//    default:
//        break;
//    }
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

/// /// /// TEMP PROJECT 8 CODE /// /// ///



void SW1_Project9(void){
    //    state = MOVE;
    //    switch_debounce_active = 1;
    //    switch1_pressed = 1;
    PWM1_BOTH_FWD();
}

void SW2_Project9(void){
    //    state = STOP;
    //    switch_debounce_active = 1;
    //    switch2_pressed = 1;
    PWM1_BOTH_OFF();
}

//void SW1_Project8(void){
//    // Transmit Mode
//    state = TRANSMIT;
//
//    clear_display = 1;
//    USCI_A0_transmit();
//}
//
//void SW2_Project8(){    // Toggle Baudrate
////    state = SEARCH;
//    baud_flag = 1;
//
//    if (baud_toggle) {
//        baud_toggle = 0;
//        UCA0BRW = 17;                   //460,800 baud
//        UCA0MCTLW = 0x4A00;
//    }
//    else {
//        baud_toggle = 1;
//        UCA0BRW = 4;                    // 115,200 baud
//        UCA0MCTLW = 0x5551;
//    }
//    clear_display = 1;
//}
//
//void SW1_Homework9(void){
//    display_changed = TRUE;
//    if(menuType == OUTER_MENU){
//        menuType = INNER_MENU;
//        songInit = TRUE;
//    }else{// menuType == IDLE
//        menuType = OUTER_MENU;
//    }
//}
//
//void SW2_Homework9(void){
//    strcpy(display_line[0], "          ");
//    strcpy(display_line[1], "          ");
//    strcpy(display_line[2], "          ");
//    strcpy(display_line[3], "          ");
//    display_changed = TRUE;
//    menuType = OUTER_MENU;
//}



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
