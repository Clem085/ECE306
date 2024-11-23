
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "strings.h"
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
unsigned int CCR0_counter;
unsigned int transmit_count;
unsigned int iot_on_time;
unsigned int start_iot_time;
char transmit_state;
unsigned int initialize_done;
unsigned int run_time;
unsigned int run_time_flag;
//------------------------------------------------------------------------------

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //------------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //----------------------------------------------------------------------------
    //...... Add What you need happen in the interrupt ......
    // LCD Backlight

    if (display_count++ == 20){
            display_count = 0;
            update_display = TRUE;
            tmr_ms_count++;
            CCR0_counter +=1;
            iot_on_time++;
            if (transmit_state == TRANSMIT){
                transmit_count++;
            }
            if (run_time_flag){
                run_time++;
            }
        }

    if(blink_count++ > MS_COUNT){
        blink_count = 0; // Reset for next count
        P6OUT ^= LCD_BACKLITE; // Flip State of LCD_BACKLITE
    }
    ADCCTL0 |= ADCSC; // Start next sample
    // Time Sequence
    one_time = 1;
    if(Time_Sequence++ > 250){
        Time_Sequence = 0;
    }

    start_iot_time++;

//    if(iot_on_time++ > 100){
//        iot_on_time = 0;
//    }

    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0

    //----------------------------------------------------------------------------
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
    case 0: break; // No interrupt
    case 2: // CCR1 not used
        P4IFG &= ~SW1;
        P4IE |= SW1;
        TB0CCTL1 &= ~CCIFG;
        TB0CCTL1 &= ~CCIE;
        TB0CCR1 += TB0CCR1_INTERVAL; // Add Offset to TBCCR1
        TB0CCTL0 |= CCIE;
        /*strcpy(display_line[0], "          ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;*/
        break;
    case 4: // CCR2 not used
        P2IFG &= ~SW2;
        P2IE |= SW2;
        TB0CCTL2 &= ~CCIFG;
        TB0CCTL2 &= ~CCIE;
        TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR1
        TB0CCTL0 |= CCIE;
        /*strcpy(display_line[0], "          ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;*/
        break;
    case 14: // overflow
        //...... Add What you need happen in the interrupt ......
        break;
    default: break;
}
//----------------------------------------------------------------------------
}
