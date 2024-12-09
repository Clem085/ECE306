/* Serial Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : serial.c
  Description:  This file contains the code to control Serial COmmunications
  Programmer: Connor Savugot
  Date Created: Nov 8, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
*/

// #include as of 11-27-24
//// Header Files
#include "ADC.h"
#include "DAC.h"
#include "Display.h"
#include "functions.h"
#include "interrupts.h"
#include "IR.h"
#include "IOT.h"
#include "LCD.h"
#include "macros.h"
#include "menu.h"
#include "motors.h"
#include "msp430.h"
#include "ports.h"
#include "serial.h"
#include "switches.h"
#include "timers.h"
//// Libraries
#include <stdio.h>
#include <string.h>

// Global Variables declared and referenced in Header file


unsigned int iot_rx_wr;
extern unsigned int tx_index;
extern unsigned char IOT_2_PC[16];
extern unsigned char PC_2_IOT[16];
extern char USB_Ring_Rx[16];
extern char IOT_Ring_Rx[32];
extern char Rx_display[16];
extern char ssid_string[10];
extern char ip_string1[10];
extern char ip_string2[10];
extern char iot_TX_buf[32];
extern unsigned int direct_iot;
unsigned int iot_tx;
extern unsigned int usb_rx_wr;
unsigned int iot_receive;
unsigned int temp;
unsigned int temp1;
char pb_index;
unsigned int transmit_done;
extern unsigned int clear_display;
char display_line[4][11];
unsigned char update_display;
unsigned char display_changed;
char transmit_state;
unsigned int ssid_index = 0;
unsigned int ip_index1 = 0;
unsigned int ip_index2 = 0;
unsigned int ssid_flag;
unsigned int ip_flag;
unsigned int period_record;
unsigned int group1_flag;
unsigned int group2_flag;
unsigned int ping = 0;
unsigned int prevping = 0;
extern char pinging1;
extern char pinging2;

int moore = 0;

void USCI_A0_transmit(void){
    iot_rx_wr = 0;        // Set Array index to first location
    UCA0IE |= UCTXIE;       // Enable TX interrupt
}

void USCI_A1_transmit(void){
    pb_index = 0;           // Set Array index to first location
    UCA1IE |= UCTXIE;       // Enable TX interrupt
}




void Init_Serial_UCA0(void){
    //------------------------------------------------------------------------------
    //                                                      TX error (%) RX error (%)
    // BRCLK    Baudrate    UCOS16  UCBRx   UCFx    UCSx    neg      pos     neg    pos
    // 8000000  4800         1      104     2       0xD6    -0.08   0.04    -0.10   0.14
    // 8000000  9600         1      52      1       0x49    -0.08   0.04    -0.10   0.14
    // 8000000  19200        1      26      0       0xB6    -0.08   0.16    -0.28   0.20
    // 8000000  57600        1      8       10      0xF7    -0.32   0.32    -1.00   0.36
    // 8000000  115200       1      4       5       0x55    -0.80   0.64    -1.12   1.76
    // 8000000  460800       0      17      0       0x4A    -2.72   2.56    -3.76   7.28
    //------------------------------------------------------------------------------
    // Configure eUSCI_A0 for UART mode
        UCA0CTLW0 = 0;                  //Put eUSCI in reset
        UCA0CTLW0 |= UCSWRST ;          // Set SMCLK as fBRCLK
        UCA0CTLW0 |= UCSSEL__SMCLK;     // MSB, LSB select
        UCA0CTLW0 &= ~UCMSB;            // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
        UCA0CTLW0 &= ~UCSPB;            // No Parity
        UCA0CTLW0 &= ~UCPEN;
        UCA0CTLW0 &= ~UCSYNC;
        UCA0CTLW0 &= ~UC7BIT;
        UCA0CTLW0 |= UCMODE_0;
    // BRCLK    Baudrate     UCOS16     UCBRx   UCFx    UCSx    neg     pos     neg     pos
    // 8000000  115200       1          4       5       0x55    -0.80   0.64    -1.12   1.76
    // UCA?MCTLW = UCSx + UCFx + UCOS16

        UCA0BRW = 4;                    // 115,200 baud
        UCA0MCTLW = 0x5551;

        UCA0CTLW0 &= ~UCSWRST;          // release from reset
        //UCA0TXBUF = 0x00;               // Prime the Pump
        UCA0IE |= UCRXIE;               // Enable RX interrupt
}

void Init_Serial_UCA1(void){
    // Configure eUSCI_A0
    UCA1CTLW0 = 0;
    UCA1CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1CTLW0 &= ~UCMSB;            // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA1CTLW0 &= ~UCSPB;            // No Parity
    UCA1CTLW0 &= ~UCPEN;
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |= UCMODE_0;

    UCA1BRW = 4;                   //460,800 baud
    UCA1MCTLW = 0x5551;

    UCA1CTLW0 &= ~UCSWRST;
    //UCA1TXBUF = 0x00;
    UCA1IE |= UCRXIE;
}

#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    //Echo back RXed character, confirm TX buffer is ready first
    switch(__even_in_range(UCA0IV, 0x08)){
    case 0:                                         // Vector 0 - no interruot
        break;
    case 2:                                        // Vector 2 - RXIFG
//        transmit_state = RECEIVE;
        iot_receive = UCA0RXBUF;
        temp1 = iot_receive;
//        Rx_display[iot_rx_wr] = iot_receive;
        if(temp1 != 0x00){
            UCA1TXBUF = temp1;
            iot_TX_buf[iot_rx_wr] = temp1;
            if(ssid_flag){

                if(temp1 != '"'){
                    ssid_string[ssid_index++] = temp1;
                }

                if(temp1 == '"'){
                    ssid_flag = 0;
                }
                else if(ssid_index == 10){
                    ssid_flag = 0;
                }
            }
            if(ssid_index < 10){
                if(iot_TX_buf[iot_rx_wr - 1] == ':' && iot_TX_buf[iot_rx_wr] == '"'){
                    ssid_flag = 1;
                }
            }
            if(ip_flag){
                if(period_record < 3){
                    if(temp1 == '.'){
                        period_record++;
                    }
                }
                if(group1_flag){
                    ip_string1[ip_index1++] = temp1;
                }
                if(group2_flag){
                    if(temp1 != '"'){
                        ip_string2[ip_index2++] = temp1;
                    }
                }

                if(period_record == 2){
                    group1_flag = 0;
                    group2_flag = 1;
                }

                if(temp1 == '"'){
                    ip_flag = 0;
                }
            }

            if(ip_index1 == 0){
                if(iot_TX_buf[iot_rx_wr - 2] == 'P' && iot_TX_buf[iot_rx_wr - 1] == ',' && iot_TX_buf[iot_rx_wr] == '"'){
                    group1_flag = 1;
                    ip_flag = 1;
                }
            }
            iot_rx_wr++;
        }



//        IOT_Ring_Rx[iot_rx_wr++] = iot_receive;     // Add to Ring Buffer
        if(iot_rx_wr >= sizeof(iot_TX_buf)){
            iot_rx_wr = BEGINNING;
        }
        break;

    case 4:{                                         // Vector 4 - TXIFG

    } break;

    default: break;

    }
}


#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    // This interrupt is the interrupt relating to serial communication port UCA1
    //----------------------------------------------
    // interrupt transmits and receives through UCA1
    switch(__even_in_range(UCA1IV, 0x08)){
    case 0: break;                  //vector 0 - not interrupt

    case 2:{                                    //Vector 2 - RX1IFG
        temp = UCA1RXBUF;
        UCA0TXBUF = temp;
    }break;

    case 4:{                                    // Vector 4 - TX1IFG
        UCA0TXBUF = IOT_Ring_Rx[tx_index];
        IOT_Ring_Rx[tx_index++] = 0;
        if(IOT_Ring_Rx[tx_index] == 0x00){
                UCA1IE &= ~UCTXIE;
                iot_tx = 0;
                transmit_done = 1;
                clear_display = 0;
        }
    }break;
    default: break;
    }
}


void ping_function(void){
    if(prevping != ping){
        //P6OUT ^= GRN_LED;
        tx_index = 0;
        if(ping){
            strcpy(IOT_Ring_Rx, (char *)pinging1); // Casted to constant (removes Volatile) in resolve warnings.
            UCA1IE |= UCTXIE;
            prevping = ping;
        }
        else{
            strcpy(IOT_Ring_Rx, (char *)pinging1); // Casted to constant (removes Volatile) in resolve warnings.
            UCA1IE |= UCTXIE;
            prevping = ping;
        }
    }
}


