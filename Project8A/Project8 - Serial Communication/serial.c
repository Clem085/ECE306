/* Serial Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : serial.c
  Description:  This file contains the code to control Serial Communications
  Programmer: Connor Savugot
  Date Created: Nov 8, 2024
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
    //  Project 9 ???
    //    if(iot_rx_wr_temp != iot_rx_rd){    // Determine if IOT is available
    //        IOT_Data[line][character] = IOT_Ring_Rx[iot_rx_rd++];
    //        if(iot_rx_rd >= sizeof(IOT_Ring_Rx)){
    //            iot_rx_rd = BEGINNING;
    //        }
    //        if(IOT_Data[line][character] == 0x0A){
    //            character = 0;
    //            line++;
    //            if(line >= 4){
    //                line = 0;
    //            }
    //            nextline = line + 1;
    //            if(nextline >= 4){
    //                nextline = 0;
    //            }
    //        }else{
    //            switch(character){
    //            case  0:
    //                if(IOT_Data[line][character] == '+'){   // Got "+"
    //                    test_Value++;
    //                    if(test_Value){
    //                        RED_LED_ON;
    //                    }
    //                    IOT_parse = 1;
    //                }
    //                break;
    //            case  1:
    //                // GRN_LED_ON;
    //                break;
    //            case  4:
    //                if(IOT_Data[line][character] == 'y'){   // Got read"y"
    //                    for(i=0; i<sizeof(AT); i++){
    //                        iot_TX_buf[i] = AT[i];
    //                    }
    //                    UCA0IE |= UCTXIE;
    //                    boot_state = 1;
    //                    // RED_LED_ON;
    //                    GRN_LED_OFF;
    //                }
    //                break;
    //
    //            case  5:
    //                if(IOT_Data[line][character] == 'G'){      // Got IP
    //                    for(i=0; i<sizeof(ip_mac); i++){
    //                        iot_TX_buf[i] = ip_mac[i];
    //                    }
    //                    iot_tx = 0;
    //                    UCA0IE |= UCTXIE;
    //                }
    //                break;
    //            case  6:
    //                break;
    //            case  10:
    //                if(IOT_Data[line][character] == 'I'){
    //                    ip_address_found = 1;
    //                    strcpy(display_line[0], "IP Address");
    //                    for(i=0; i<sizeof(ip_address); i++){
    //                        ip_address[i] = 0;
    //                    }
    //                    display_changed = 1;
    //                    iot_index = 0;
    //                }
    //                break;
    //            default: break;
    //            }
    //        }
    //    }

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
    UCA0CTLW0 |= UCSWRST;            // Put eUSCI in reset to configure
    UCA0CTLW0 |= UCSSEL__SMCLK;      // Set SMCLK as fBRCLK
    UCA0CTLW0 &= ~UCMSB;             // LSB first
    UCA0CTLW0 &= ~UCSPB;             // 1 stop bit
    UCA0CTLW0 &= ~UCPEN;             // No Parity
    UCA0CTLW0 &= ~UCSYNC;            // Asynchronous mode
    UCA0CTLW0 &= ~UC7BIT;            // 8-bit data
    UCA0CTLW0 |= UCMODE_0;           // UART mode

    if (baud_toggle) {               // 460800 baud
        UCA0BRW = 17;
        UCA0MCTLW = 0x4A00;
    } else {                         // 115200 baud
        UCA0BRW = 4;
        UCA0MCTLW = 0x5551;
    }

    UCA0CTLW0 &= ~UCSWRST;           // Release from reset
    UCA0IE |= UCRXIE;                // Enable RX interrupt

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
