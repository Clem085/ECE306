/*
 * serial.c
 *
 *  Created on: Nov 21, 2024
 *  Author: Kayla Radu
 *
 */

#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "strings.h"
#include "wheels.h"
#include "Timers.h"
//#include  "DAC.h"
#include "switches.h"


volatile unsigned int iot_rx_wr;
extern volatile unsigned int tx_index;
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
unsigned int ping = 0;
unsigned int prevping = 0;
extern volatile char pinging1;
extern volatile char pinging2;

int moore = 0;

void USCI_A0_transmit(void){

    iot_rx_wr = 0;        // Set Array index to first location
    UCA0IE |= UCTXIE;       // Enable TX interrupt
}
void USCI_A1_transmit(void){

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
    //UCA1TXBUF = 0x00;
    UCA1IE |= UCRXIE;
}

#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    //----------------------------------------------------------------------------
    // Echo back RXed character, confirm TX buffer is ready first
    switch(__even_in_range(UCA0IV,0x08)){
    case 0:  break;                        // Vector 0 - no interrupt
    case 2:{                                // Vector 2 - RXIFG
        temp = iot_rx_wr++;
        IOT_2_PC[temp] = UCA0RXBUF;           // Rx -> IOT_2_PC character array
        if (iot_rx_wr >= (sizeof(IOT_2_PC))){
            iot_rx_wr = BEGINNING;              // Circular buffer back to beginning
        }
        UCA1IE |= UCTXIE;                     // Enable Tx interrupt
    }break;
    case 4:{                                // Vector 4 - TXIFG
        UCA0TXBUF = PC_2_IOT[direct_iot++];
        if (direct_iot >= (sizeof(PC_2_IOT))){
            direct_iot = BEGINNING;
        }
        if (direct_iot == usb_rx_wr){
            UCA0IE &= ~UCTXIE; // Disable TX interrupt
        }
    }break;
    default: break;
    }
    //-----------------------------------------------------------------------------
}



#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    //-----------------------------------------------------------------------------
    // Echo back RXed character, confirm TX buffer is ready first
    switch(__even_in_range(UCA1IV,0x08)){
    case 0:  break;                           // Vector 0 - no interrupt
    case 2:{                                  // Vector 2 - RXIFG
        temp = usb_rx_wr++;
        PC_2_IOT[temp] = UCA1RXBUF;             // Rx -> PC_2_IOT character array
        if (usb_rx_wr >= (sizeof(PC_2_IOT))){
            usb_rx_wr = BEGINNING;                // Circular buffer back to beginning
        }
        UCA0IE |= UCTXIE;                       // Enable Tx interrupt
    }break;
    case 4:{                                  // Vector 4 - TXIFG
        UCA1TXBUF = IOT_2_PC[direct_usb++];
        if (direct_usb >= (sizeof(IOT_2_PC))){
            direct_usb = BEGINNING;               // Circular buffer back to beginning
        }
        if (direct_usb == iot_rx_wr){
            UCA1IE &= ~UCTXIE; // Disable TX interrupt
        }
    }break;
    default: break;
    }
    //-----------------------------------------------------------------------------
}



void pingpong(void){
    if(prevping != ping){
        //        P6OUT ^= GRN_LED;
        if(ping){
            strcpy(IOT_Ring_Rx, pinging1);
            UCA1IE |= UCTXIE;
            prevping = ping;
        }
        else{
            strcpy(IOT_Ring_Rx, pinging2);
            UCA1IE |= UCTXIE;
            prevping = ping;
        }
    }
}


