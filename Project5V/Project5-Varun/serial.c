/*
 * serial.c
 *
 *  Created on: Mar 27, 2024
 *      Author: varun
 */

#include    "macros.h"
#include    "ports.h"
#include    <msp430.h>
#include  "functions.h"

volatile unsigned int iot_rx_wr;
extern volatile unsigned int legacy;
extern volatile unsigned char IOT_2_PC[SMALL_RING_SIZE];
extern volatile unsigned char PC_2_IOT[SMALL_RING_SIZE];
extern volatile char USB_Ring_Rx[SMALL_RING_SIZE];
extern volatile char IOT_Ring_Rx[LARGE_RING_SIZE];
extern volatile char Rx_display[SMALL_RING_SIZE];
extern volatile char ssid_display[SSID_SIZE];
extern volatile char ip_display1[SSID_SIZE];
extern volatile char ip_display2[SSID_SIZE];
extern volatile char iot_TX_buf[LARGE_RING_SIZE];
extern unsigned int direct_iot;
volatile unsigned int iot_tx;
extern volatile unsigned int usb_rx_wr;
volatile unsigned int iot_receive;
volatile unsigned int temp;
volatile unsigned int temp1;
char pb_index;
unsigned int transmit_done;
extern unsigned int clear_display;
char display_line[4][11];
volatile unsigned char update_display;
volatile unsigned char display_changed;
char transmit_state;
unsigned int ssid_index = 0;
unsigned int ip_index1 = 0;
unsigned int ip_index2 = 0;
unsigned int ssid_record_flag;
unsigned int ip_record_flag;
unsigned int period_record;
unsigned int group1_flag;
unsigned int group2_flag;

int moore = 0;

void USCI_A0_transmit(void){
  // Contents must be in process_buffer
  // End of Transmission is identified by NULL character in process_buffer
  // process_buffer includes Carriage Return and Line Feed
    iot_rx_wr = 0;        // Set Array index to first location
    UCA0IE |= UCTXIE;       // Enable TX interrupt
}
void USCI_A1_transmit(void){
  // Contents must be in process_buffer
  // End of Transmission is identified by NULL character in process_buffer
  // process_buffer includes Carriage Return and Line Feed
    pb_index = 0;           // Set Array index to first location
    UCA1IE |= UCTXIE;       // Enable TX interrupt
}


void IOT_Process(void){
    int i;
    unsigned int iot_rx_wr_temp;
    iot_rx_wr_temp = iot_rx_wr;
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
    UCA1IE |= UCRXIE;
}

#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    //Echo back RXed character, confirm TX buffer is ready first
    switch(__even_in_range(UCA0IV, 0x08)){
    case 0:                                         // Vector 0 - no interruot
        break;

    case 2:                                        // Vector 2 - RXIFG
        iot_receive = UCA0RXBUF;
        temp1 = iot_receive;
        if (temp1 != 0x00){
            UCA1TXBUF = temp1;
            iot_TX_buf[iot_rx_wr] = temp1;
            if (ssid_record_flag){
                ssid_display[ssid_index++] = temp1;
                if (temp1 == '"'){
                    ssid_record_flag = FALSE;
                }
                else if (ssid_index == 10){
                    ssid_record_flag = FALSE;
                }
            }
            if (ssid_index < 10){
                if (iot_TX_buf[iot_rx_wr - 1] == ':' && iot_TX_buf[iot_rx_wr] == '"'){
                    ssid_record_flag = TRUE;
                }
            }

            if (ip_record_flag){
                if (period_record < 3) {
                    if (temp1 == '.'){
                        period_record++;
                    }
                }
                if (group1_flag){
                    ip_display1[ip_index1++] = temp1;
                }
                if (group2_flag){
                    ip_display2[ip_index2++] = temp1;
                }

                if (period_record == 2) {
                    group1_flag = FALSE;
                    group2_flag = TRUE;
                }

                if (temp1 == '"'){
                    ip_record_flag = FALSE;
                }
            }

            if (ip_index1 == 0){
                if (iot_TX_buf[iot_rx_wr - 2] == 'P' && iot_TX_buf[iot_rx_wr - 1] == ',' && iot_TX_buf[iot_rx_wr] == '"'){
                    group1_flag = TRUE;
                    ip_record_flag = TRUE;
                }
            }
            iot_rx_wr++;
        }

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
__interrupt void eUSCI_A1_ISR(void){                // This interrupt is the interrupt relating to serial communication port UCA1
    //----------------------------------------------
    // interrupt transmits and receives through UCA1

    char usb_value;
    switch(__even_in_range(UCA1IV, 0x08)){
    case 0: break;                  //vector 0 - not interrupt

    case 2:{                                    //Vector 2 - RX1IFG
        temp = UCA1RXBUF;
        UCA0TXBUF = temp;

    }break;

    case 4:{                                    // Vector 4 - TX1IFG
        UCA0TXBUF = IOT_Ring_Rx[legacy];
        IOT_Ring_Rx[legacy++] = 0;
        if(IOT_Ring_Rx[legacy] == 0x00){
                UCA1IE &= ~UCTXIE;
                iot_tx = 0;
                transmit_done = TRUE;
                clear_display = 0;
        }

    }break;

    default: break;
    }
}

