/* Interrupts Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : interrupts.c
  Description:  This file contains the code to control all ISR (Interrupt Service Routines)
  Programmer: Connor Savugot
  Date Created: Oct 21, 2024
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

// Global Variables declared and referenced in Header file



// Timers
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //    ADC_Update = TRUE;
    //-----------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //---------------------------------------------------------------------------
    update_display = TRUE;
    Time_Sequence++;
    //    State_Sequence = Time_Sequence;
    //    P6OUT ^= LCD_BACKLITE;
    TB0CCR0 += TB0CCR0_INTERVAL;
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
    //---------------------------------------------------------------------------
    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
    //---------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
    case  0: break;                    // No interrupt
    case  2:                           // CCR1 Used for SW1 Debounce
        count_debounce_SW1++;
        if (count_debounce_SW1 >= DEBOUNCE_TIME){
            count_debounce_SW1 = 0;

            TB0CCTL1 &= ~CCIE;
            P4IFG &= ~SW1;
            P4IE  |= SW1;

            //            TB0CCTL0 |= CCIE;
        }

        TB0CCR1 += TB0CCR1_INTERVAL;


        break;

    case  4:                           // CCR2 Used for SW2 Debounce
        count_debounce_SW2++;
        if (count_debounce_SW2 >= DEBOUNCE_TIME){
            count_debounce_SW2 = 0;

            TB0CCTL2 &= ~CCIE;
            P2IFG &= ~SW2;
            P2IE  |=  SW2;

            //            TB0CCTL0 |= CCIE;
        }


        TB0CCR2 += TB0CCR2_INTERVAL;

        break;

    case 14:                           // overflow available for greater than 1 second timer
        break;
    default: break;
    }
}











//#pragma vector = TIMER3_B0_VECTOR
//__interrupt void Timer3_B0_ISR(void){
//    //-----------------------------------------------------------------------------
//    // TimerB3 0 Interrupt handler
//    //---------------------------------------------------------------------------
////    update_display = TRUE;
//    //    State_Sequence = Time_Sequence;
//    //    P6OUT ^= LCD_BACKLITE;
//    TB3CCR0 += TB3CCR0_INTERVAL;
//}


// Switches
//-----------------------------------------------------------------------------
// Port 4 interrupt for switch 1, it is disabled for the duration
// of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT4_VECTOR
__interrupt void switch1_interrupt(void) {
    // Switch 1
    if (P4IFG & SW1) {

        P4IE &= ~SW1;
        P4IFG &= ~SW1;

        TB0CCTL1 &= ~CCIFG;             // Clear SW1 debounce interrupt flag
        TB0CCR1 = TB0CCR1_INTERVAL;     // CCR1 add offset
        TB0CCTL1 |= CCIE;               // CCR1 enable interrupt

        //SW1 FUNCTIONS:
        // Homework 9 Button Function
        display_changed = TRUE;
        if(menuType == OUTER_MENU){
            menuType = INNER_MENU;
            songInit = TRUE;
        }else{// menuType == IDLE
            menuType = OUTER_MENU;
        }
        //        P1OUT ^= RED_LED; // Toggle Red LED ON/OFF
        //        switch(menu){
        //            case IDLE:
        //                menu = RESISTOR;
        //                break;
        //            case RESISTOR:
        //                menu = SHAPES;
        //                break;
        //            case SHAPES:
        //                menu = SONG;
        //                break;
        //            case SONG:
        //                menu = RESISTOR;
        //                break;
        //            default:
        //                break;
        //            }
    }
    //-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
// Port 2 interrupt for switch 2, it is disabled for the duration
// of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT2_VECTOR
__interrupt void switch2_interrupt(void) {
    // Switch 2
    if (P2IFG & SW2) {

        P2IE &= ~SW2;
        P2IFG &= ~SW2;

        TB0CCTL2 &= ~CCIFG;             // Clear SW2 debounce interrupt flag
        TB0CCR2 = TB0CCR2_INTERVAL;     // CCR2 add offset
        TB0CCTL2 |= CCIE;               // CCR2 enable interrupt

        //SW2 FUNCTIONS:
        strcpy(display_line[0], "          ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;
        menuType = OUTER_MENU;




        //        // Implement Later
        //        ADC_Update ^= 1; // Toggles the State of ADC_Update, Makes ADC Values Appear/Disappear on LCD
        //        if(!ADC_Update){
        //            strcpy(display_line[0], "          ");
        //            strcpy(display_line[1], "          ");
        //            strcpy(display_line[2], "          ");
        //            strcpy(display_line[3], "          ");
        //            update_display =TRUE;
        //            backlight_status = OFF;
        //        }

        // Project 7
        //        if(IR_status == OFF){
        //            IR_changed = TRUE;
        //            strcpy(display_line[0], "  IR ON   ");
        //            strcpy(display_line[1], "          ");
        //            strcpy(display_line[2], "          ");
        //            strcpy(display_line[3], "          ");
        //            update_display = TRUE;
        //            IR_status = ON;
        //        }
        //        else{// IR_status = ON
        //            IR_changed = TRUE;
        //            strcpy(display_line[0], "  IR OFF  ");
        //            strcpy(display_line[1], "          ");
        //            strcpy(display_line[2], "          ");
        //            strcpy(display_line[3], "          ");
        //            update_display =TRUE;
        //            IR_status = OFF;
        //        }


    }
    //-----------------------------------------------------------------------------
}




//// DAC Interrupt
// The interrupt is not used
#pragma vector = SAC1_SAC3_VECTOR
__interrupt void SAC3_ISR(void){
    switch(__even_in_range(SAC0IV,SACIV_4)){
    case SACIV_0: break;
    case SACIV_2: break;
    case SACIV_4:
        //   DAC_data++;
        //   DAC_data &= 0xFFF;
        //   SAC3DAT = DAC_data;                 // DAC12 output positive ramp
        break;
    case 14:
        // Overflow Occurred
        if(DAC_overflow_counter++ >= 3){
            // The following line should be done in a timer overflow interrupt [after 2 or 3 overflows]
            P2OUT   |=  DAC_ENB;                  // Value = High [enabled]
            // Each time through the overflow time after enable, subtract 50 from DAC_data
            DAC_data = 4000;
            SAC3DAT  = DAC_data;                  // Stepping DAC Output
            // Somewhere around 1200 will be about 6v. You will need to measure it.
            DAC_overflow_counter = 0;
        }
    default: break;
    }
}




// ADC Interrupt
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    //    backlight_status = ON;
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
    case ADCIV_NONE:
        break;
    case ADCIV_ADCOVIFG:   // When a conversion result is written to the ADCMEM0
        // before its previous conversion result was read.
        break;
    case ADCIV_ADCTOVIFG:   // ADC conversion-time overflow
        break;
    case ADCIV_ADCHIIFG:    // Window comparator interrupt flags
        break;
    case ADCIV_ADCLOIFG:    // Window comparator interrupt flag
        break;
    case ADCIV_ADCINIFG:    // Window comparator interrupt flag
        break;
    case ADCIV_ADCIFG:      // ADCMEM0 memory register with the conversion result
        ADCCTL0 &= ~ADCENC;                          // Disable ENC bit.
        switch (ADC_Channel++){
        case 0x00:                                   // Channel A2 Interrupt
            ADCMCTL0 &= ~ADCINCH_2;                  // Disable Last channel A2
            ADCMCTL0 |=  ADCINCH_3;                  // Enable Next channel A3

            ADC_Left_Detect = ADCMEM0;               // Move result into Global Values
            ADC_Left_Detect = ADC_Left_Detect >> 2;  // Divide the result by 4
            if(ADC_Update && ADC_Display){
                HexToBCD(ADC_Left_Detect);
                dispPrint(adc_char,'2');
            }
            //            ADC_Update = TRUE;

            break;

        case 0x01:                                   // Channel A3 Interrupt
            ADCMCTL0 &= ~ADCINCH_3;                  // Disable Last channel A2
            ADCMCTL0 |=  ADCINCH_5;                  // Enable Next channel A1

            ADC_Right_Detect = ADCMEM0;              // Move result into Global Values
            ADC_Right_Detect = ADC_Right_Detect >> 2;// Divide the result by 4
            if(ADC_Update && ADC_Display){
                HexToBCD(ADC_Right_Detect);
                dispPrint(adc_char,'3');
            }
            //            ADC_Update = TRUE;

            break;

        case 0x02:                                   // Channel A1 Interrupt
            ADCMCTL0 &= ~ADCINCH_5;                  // Disable Last channel A?
            ADCMCTL0 |= ADCINCH_2;                   // Enable Next [First] channel 2

            ADC_Thumb = ADCMEM0;                     // Move result into Global Values
            ADC_Thumb = ADC_Thumb >> 2;              // Divide the result by 4
            if(ADC_Update && ADC_Display){
                HexToBCD(ADC_Thumb);
                dispPrint(adc_char,'4');
            }
            //            ADC_Update = TRUE;



            ADC_Temp = ADC_Thumb >> 5;
//            HexToBCD(ADC_Temp);
//            dispPrint(adc_char,'3');
//            HexToBCD(ADC_Prev);
//            dispPrint(adc_char,'4');
//            if(ADC_Temp > ADC_Prev){
//                ADC_Prev = ADC_Temp;
//                ADC_Changed = TRUE;
//            }

            if(ADC_Prev == 8192){
                ADC_Prev = ADC_Temp;
            }

            ADC_Channel = 0;
            break;

        default:
            break;
        }
        ADCCTL0 |= ADCENC;                          // Enable Conversions
        ADCCTL0 |= ADCSC;
        break;
        default:
            break;
    }
}


////-----------------------------------------------------------------------------
////  The eUSCI_Ahas only one interrupt vector that is shared for transmission and for reception.
//#pragma vector=EUSCI_A0_VECTOR
//__interrupt void eUSCI_A0_ISR(void){
//    unsigned int temp;
//    switch(__even_in_range(UCA0IV,0x08)){
//    case 0:                                     // Vector 0 -no interrupt
//        break;
//    case 2:                                     // Vector 2 -RXIFG
//        temp = usb_rx_ring_wr++;
//        IOT_2_PC[temp] = UCA0RXBUF; // Rx -> IOT_2_PC character array
//        //      USB_Char_Rx[temp] = UCA0RXBUF; // Replaced by Code above based on Slide 6 of Lecture 13 - eUSCI0 to eUSCI1 The Big Picture
//        if (usb_rx_ring_wr >= (sizeof(USB_Char_Rx))){
//            usb_rx_ring_wr= BEGINNING;             // Circular buffer back to beginning
//        }
//        UCA0TXBUF = IOT_2_PC[temp]; // Transmit out the same port
//        // ^^^ Added code based on Slide 6 of Lecture 13 - eUSCI0 to eUSCI1 The Big Picture
//        break;
//    case 4:                                     // Vector 4 –TXIFG
//        //////////////////////////////////////////////////////////////////////////////////////////////////////
//        //Added Based on Lecture 13 - eUSCI0 to eUSCI1 The Big Picture Slide 13
//        UCA0TXBUF = process_buffer[pb_index];  // Transmit Current Indexed value
//        process_buffer[pb_index++] = NULL;     // Null Location of Transmitted value
//        if(process_buffer[pb_index] == NULL){  // Is the next pb_index location NULL - End of Command
//            UCA0IE &= ~UCTXIE;                   // Disable TX interrupt
//        }
//        //////////////////////////////////////////////////////////////////////////////////////////////////////
//        break;
//    default: break;
//    }
//}
////----------------------------------------------------------------------------
