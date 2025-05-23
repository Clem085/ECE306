/* Interrupts Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : interrupts.c
  Description:  This file contains the code to control all ISR (Interrupt Service Routines)
  Programmer: Connor Savugot
  Date Created: Oct 21, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */

// #include as of 11-27-24
//// Header Files
#include  "ADC.h"
#include  "DAC.h"
#include  "Display.h"
#include  "functions.h"
#include  "interrupts.h"
#include  "IR.h"
#include  "IOT.h"
#include  "LCD.h"
#include  "macros.h"
#include  "menu.h"
#include  "motors.h"
#include  "msp430.h"
#include  "ports.h"
#include  "serial.h"
#include  "switches.h"
#include  "timers.h"
//// Libraries
#include  <stdio.h>
#include  <string.h>

// Global Variables declared and referenced in Header file


/// /// ADC Interrupts /// /// ADC Interrupts /// /// ADC Interrupts /// ///
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
    case ADCIV_NONE:
        break;
    case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
        // before its previous conversion result was read.
        break;
    case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
        break;
    case ADCIV_ADCHIIFG: // Window comparator interrupt flags
        break;
    case ADCIV_ADCLOIFG: // Window comparator interrupt flag
        break;
    case ADCIV_ADCINIFG: // Window comparator interrupt flag
        break;
    case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result



        ADCCTL0 &= ~ADCENC; // Disable ENC bit.
        switch (ADC_Channel++){
                case 0:                                   // Channel A2 Interrupt
                    ADCMCTL0 &= ~ADCINCH_2;                  // Disable Last channel A2
                    ADCMCTL0 |=  ADCINCH_3;                  // Enable Next channel A3

                    ADC_Left_Detect = ADCMEM0;               // Move result into Global Values
                    ADC_Left_Detect = ADC_Left_Detect >> 2;  // Divide the result by 4
        //            HexToBCD(ADC_Left_Detect);
        //            adc_line(2,2);


                    break;

                case 1:                                   // Channel A3 Interrupt
                    ADCMCTL0 &= ~ADCINCH_3;                  // Disable Last channel A2
                    ADCMCTL0 |=  ADCINCH_5;                  // Enable Next channel A1

                    ADC_Right_Detect = ADCMEM0;              // Move result into Global Values
                    ADC_Right_Detect = ADC_Right_Detect >> 2;// Divide the result by 4
        //            HexToBCD(ADC_Right_Detect);
        //            adc_line(3,3);

                    break;

                case 2:                                   // Channel A1 Interrupt
                    ADCMCTL0 &= ~ADCINCH_5;                  // Disable Last channel A?
                    ADCMCTL0 |= ADCINCH_2;                   // Enable Next [First] channel 2

                    ADC_thumb = ADCMEM0;                     // Move result into Global Values
                    ADC_thumb = ADC_thumb >> 2;              // Divide the result by 4
        //            HexToBCD(ADC_Thumb);
        //            adc_line(4,4);


                    ADC_Channel = 0;
                    break;

                default:
                    break;
                }
        ADCCTL0 |= ADCENC; // Enable Conversions
        break;
        default:    break;
    }

}
/// /// ADC Interrupts /// /// ADC Interrupts /// /// ADC Interrupts /// ///







/// /// Timer Interrupts /// /// Timer Interrupts /// /// Timer Interrupts /// ///
//------------------------------------------------------------------------------
// Globals Initialized here with Values
unsigned int DimFlag = TRUE;
unsigned int FlagSpinL = FALSE;
unsigned int FlagSpinR = FALSE;
unsigned int FlagWait = FALSE;
unsigned int pingcount = 0;

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

    if(DimFlag == TRUE){
        if(Blink_counter++ >= 4){
            Blink_counter = 0;
            LCD_BACKLITE_BRIGHTNESS = PERCENT_80; //Flips on

        }
    }

    if(run_time_flag){
        P6OUT ^= GRN_LED;
        run_time++;
    }


    if(FlagSpinR == TRUE){
        SpincountR++;
    }

    if(FlagWait == TRUE){
        Waitcount++;
    }


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
        if(DAC_data > 900){
            DAC_data -= 50;
            SAC3DAT = DAC_data;
        }
        break;
    case 4: // CCR2 used for ADC Sampling. 1 Sample every 20ms, 60ms for all 3 samples
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
//----------------------------------------------------------------------------
/// /// Timer Interrupts /// /// Timer Interrupts /// /// Timer Interrupts /// ///









/// /// Switch Interrupts /// /// Switch Interrupts /// /// Switch Interrupts /// ///
//------------------------------------------------------------------------------
// Port 4 interrupt for switch 1, it is disabled for the duration
// of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT4_VECTOR
__interrupt void switch1_interrupt(void){
// Switch 1
    if(P4IFG & SW1){
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
__interrupt void switch2_interrupt(void){
// Switch 2
    if(P2IFG & SW2){
        P2IE &= ~SW2;//Disable Switch 1
        P2IFG &= ~SW2;//Clear Switch 1 Flag
        TB0CCTL2 &= ~CCIFG; //Clear TimerB0 Interrupt Flag for Capture Compare Register 1
        TB0CCR2 = TB0R + TB0CCR2_INTERVAL;//Add Interval to TB0R for TB0CCR1
        TB0CCTL2 |= CCIE;//Enable TimerB0_1
        P6OUT &= ~LCD_BACKLITE;
        TB0CCTL0 &= ~CCIE;
        /*P2OUT |= IR_LED; // Initial Value = Low / Off
        P2DIR |= IR_LED; // Direction = input*/
        state = FIND;

        if(baud_toggle){
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
/// /// Switch Interrupts /// /// Switch Interrupts /// /// Switch Interrupts /// ///
