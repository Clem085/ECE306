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

// Switches
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
//        P2OUT &= ~IR_LED; // Initial Value = Low / Off
//        P2DIR |= IR_LED; // Direction = input

//        transmit_state = TRANSMIT;
//        switchpressed = ON;
//                state = WAIT;


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







#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //------------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //----------------------------------------------------------------------------
    //...... Add What you need happen in the interrupt ......
    // LCD Backlight
    // Runs every 50 MS

    if(display_count++ == 4){
        display_count = 0;
        update_display = TRUE;

        CCR0_counter +=1;
        iot_on_time++;
//        if (transmit_state == TRANSMIT){
//            transmit_count++;
//        }

    }

    if(pingcount++ == 100){
//        P1OUT ^= RED_LED;
        ping = !ping;
        pingcount = 0;
    }

    if(lostflg){
        lostCounter++;
        P1OUT |= RED_LED;
    }else{
        lostCounter = 0;
    }

    // Time Sequence
//    one_time = 1; //CARLSON CODE
//    if(Time_Sequence++ > 250){
//        Time_Sequence = 0;
//    }



    if(DimFlag == TRUE){
        if (Blink_counter++ >= 4){
            Blink_counter = 0;
            LCD_BACKLITE_DIMING = PERCENT_80; //Flips on

        }
    }

    if (run_time_flag){
        P6OUT ^= GRN_LED;
        run_time++;
    }

//    if(FlagSpinL == TRUE){
//        SpincountL++;
//    }


    if(FlagSpinR == TRUE){
        SpincountR++;
    }

    if(FlagWait == TRUE){
        Waitcount++;
    }

//  Change to use this code later
//    if(archFlag){
//        arch_counter++;
//    }

    if(arch_counter++ > 65534){
        arch_counter = 0;
    }


    if(motorDrain++ > 65534){
        motorDrain = 0;
    }

    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0

    //----------------------------------------------------------------------------
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
//        P1OUT ^= RED_LED;
    //----------------------------------------------------------------------------
    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
    //----------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
    case 0: break; // No interrupt
    case 2:
//        TB0CCR1 = TB0R + TB0CCR1_INTERVAL; // WARNING HELP QUESTION why not TB0CCR1 += TB0CCR1_INTERVAL;
        TB0CCR1 += TB0CCR1_INTERVAL; // WARNING
//        TB0CCTL1 |= CCIE;//Enable TimerB0_1
//        P1OUT ^= RED_LED;
        if(DAC_data > 900){
            DAC_data -= 50;
            SAC3DAT = DAC_data;
        }
        break;
    case 4: // CCR2 used for ADC Sampling. 1 Sample every 20ms, 60ms for all 3 samples
//        P2IFG &= ~SW2;
//        P2IE |= SW2;
//        TB0CCTL2 &= ~CCIFG;
//        TB0CCTL2 &= ~CCIE;
//        TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR1
//        TB0CCTL0 |= CCIE;
        /*strcpy(display_line[0], "          ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;*/
        TB0CCR2 += TB0CCR2_INTERVAL;
        ADCCTL0 |= ADCSC; // Start next sample
        break;
    case 14: // overflow
        //...... Add What you need happen in the interrupt ......
        break;
    default: break;
    }
    //----------------------------------------------------------------------------
}

//#pragma vector=TIMER0_B1_VECTOR
//__interrupt void TIMER0_B1_ISR(void){
//    P1OUT ^= RED_LED;
//    //----------------------------------------------------------------------------
//    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//    //----------------------------------------------------------------------------
//    switch(__even_in_range(TB0IV,14)){
//    case  0: break;                    // No interrupt
//    case  2:                           // CCR1 Used for SW1 Debounce
//        P6OUT ^= GRN_LED;
//        count_debounce_SW1++;
//        if (count_debounce_SW1 >= DEBOUNCE_TIME){
//            count_debounce_SW1 = 0;
//
//            TB0CCTL1 &= ~CCIE;
//            P4IFG &= ~SW1;
//            P4IE  |= SW1;
//
//            TB0CCTL0 |= CCIE;
//        }
//
//        if(DAC_data > 900){
//            DAC_data -= 50;
//            SAC3DAT = DAC_data;
//        }
//        break;
//
//    case  4:                           // CCR2 Used for SW2 Debounce
//        count_debounce_SW2++;
//        if (count_debounce_SW2 >= DEBOUNCE_TIME){
//            count_debounce_SW2 = 0;
//
//            TB0CCTL2 &= ~CCIE;
//            P2IFG &= ~SW2;
//            P2IE  |=  SW2;
//
//            TB0CCTL0 |= CCIE;
//        }
//
//        break;
//
//    case 14:                           // overflow available for greater than 1 second timer
//        break;
//    default: break;
//    }
    //----------------------------------------------------------------------------


