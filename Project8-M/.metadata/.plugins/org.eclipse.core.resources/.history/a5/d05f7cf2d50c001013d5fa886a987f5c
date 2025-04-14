/*
 * serial.c
 *
 *  Created on: Nov 11, 2024
 *      Author: kaylaradu
 */

#include "LCD.h"
#include "functions.h"
#include "macros.h"
#include "wheels.h"
#include "msp430.h"
#include "ports.h"
#include "switches.h"
#include <string.h>
#include "timersB0.h"
#include <stdio.h>

// VARIABLES
volatile unsigned int iot_rx_wr;
extern volatile unsigned char IOT_2_PC[SMALL_RING_SIZE];
extern volatile unsigned char PC_2_IOT[SMALL_RING_SIZE];
extern volatile char USB_Ring_Rx[SMALL_RING_SIZE];
extern volatile char IOT_Ring_Rx[SMALL_RING_SIZE];
extern volatile char Rx_display[SMALL_RING_SIZE];
volatile char iot_TX_buf[SMALL_RING_SIZE];
extern unsigned int direct_iot;
volatile unsigned int iot_tx;
extern volatile unsigned int usb_rx_wr;
volatile unsigned int iot_receive;
char pb_index;
unsigned int transmit_done;
extern unsigned int clear_display;
char display_line[4][11];
volatile unsigned char update_display;
volatile unsigned char display_changed;
char transmit_state;

void USCI_A0_transmit(void){
  // Contents must be in process_buffer
  // End of Transmission is identified by NULL character in process_buffer
  // process_buffer includes Carriage Return and Line Feed
//    strcpy(iot_TX_buf, "NCSU  #1");
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

//void UCA0_transmit(char Char){
//    while(UCA0STATW & UCBUSY);
//    UCA0TXBUF = Char;
//}
//void UCA1_transmit(char Char){
//    UCA1TXBUF = Char;
//}

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

    UCA1BRW = 17;                   //460,800 baud
    UCA1MCTLW = 0x4A00;

    UCA1CTLW0 &= ~UCSWRST;
    //UCA1TXBUF = 0x00;
    UCA1IE |= UCRXIE;
}

#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    //Echo back RXed character, confirm TX buffer is ready first
    unsigned int temp;
    switch(__even_in_range(UCA0IV, 0x08)){
    case 0:                                         // Vector 0 - no interruot
        break;

    case 2:                                         // Vector 2 - RXIFG
        transmit_state = RECEIVE;
        iot_receive = UCA0RXBUF;
        Rx_display[iot_rx_wr] = iot_receive;
        iot_TX_buf[iot_rx_wr] = iot_receive;
        IOT_Ring_Rx[iot_rx_wr++] = iot_receive;     // Add to Ring Buffe
      
        if(iot_rx_wr >= sizeof(IOT_Ring_Rx)){

            iot_rx_wr = BEGINNING;
        }
//        UCA1IE |= UCTXIE;         // Enable Tx interrupt
//        UCA0TXBUF = iot_receive;
        break;

    case 4:{                                         // Vector 4 - TXIFG
        UCA0TXBUF = iot_TX_buf[iot_tx];
        iot_TX_buf[iot_tx++] = 0x00;
        if(iot_TX_buf[iot_tx] == 0x00){
        UCA0IE &= ~UCTXIE;
        iot_tx = 0;
        transmit_done = 1;
        clear_display = 0;
        }
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
        iot_receive = UCA1RXBUF;
        IOT_Ring_Rx[iot_rx_wr++] = iot_receive; // Add to Ring Buffer
        if(iot_rx_wr >= sizeof(IOT_Ring_Rx)){
        iot_rx_wr = BEGINNING;
        }

//        UCA1IE |= UCTXIE;
//        UCA1TXBUF = iot_receive;
    }break;

    case 4:{                                    // Vector 4 - TX1IFG
        UCA1TXBUF = iot_TX_buf[iot_tx];
        iot_TX_buf[iot_tx++] = 0;
        if(iot_TX_buf[iot_tx] == 0x00){
        UCA1IE &= ~UCTXIE;
        iot_tx = 0;
        transmit_done = 1;
        clear_display = 0;
        }
    }break;

    default: break;
    }
}


