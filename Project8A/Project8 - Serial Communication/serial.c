/* Serial Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : serial.c
  Description:  This file contains the code to control Serial COmmunications
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












//// #include as of 11-08-24
//// Header Files
//#include  "msp430.h"
//#include  "functions.h"
//#include  "LCD.h"
//#include  "ports.h"
//#include  "macros.h"
//#include  "motors.h"
//#include  "Display.h"
//#include  "timers.h"
//#include  "switches.h"
//#include  "ADC.h"
//#include  "IR.h"
//#include  "serial.h"
//#include  "DAC.h"
//// Libraries
//#include  <string.h>
//#include  <stdio.h>
//
//
//
//// Global Variables
//char process_buffer[25];                 // Size for appropriate Command Length
//char pb_index;                           // Index for process_buffer
//
//
//
//// Transmit Function for USCI_A0
//void USCI_A0_transmit(void){
//    // Contents must be in process_buffer
//    // End of Transmission is identified by NULL character in process_buffer
//    // process_buffer includes Carriage Return and Line Feed
//    pb_index = 0;                          // Set Array index to first location
//    UCA0IE |= UCTXIE;                      // Enable TX interrupt
//}
//
//
//// Serial Communications
//void Init_Serial_UCA0(char speed){
//    //-----------------------------------------------------------------------------
//    //                                               TX error (%) RX error (%)
//    //  BRCLK   Baudrate UCOS16  UCBRx  UCFx    UCSx    neg   pos  neg  pos
//    //  8000000    4800     1     104     2     0xD6   -0.08 0.04 -0.10 0.14
//    //  8000000    9600     1      52     1     0x49   -0.08 0.04 -0.10 0.14
//    //  8000000   19200     1      26     0     0xB6   -0.08 0.16 -0.28 0.20
//    //  8000000   57600     1       8    10     0xF7   -0.32 0.32 -1.00 0.36
//    //  8000000  115200     1       4     5     0x55   -0.80 0.64 -1.12 1.76
//    //  8000000  460800     0      17     0     0x4A   -2.72 2.56 -3.76 7.28
//    //-----------------------------------------------------------------------------
//    ///////////////////////////////////////////////////////////////////////////////
//    // Copied from other Init_Serial_UCA0 function
//    int i;
//    for(i=0; i<SMALL_RING_SIZE; i++){
//        USB_Char_Rx[i] = 0x00;                  // USB Rx Buffer
//    }
//    usb_rx_ring_wr = BEGINNING;
//    usb_rx_ring_rd = BEGINNING;
//    for(i=0; i<LARGE_RING_SIZE; i++){         // May not use this
//        USB_Char_Tx[i] = 0x00;                  // USB Tx Buffer
//    }
//    usb_tx_ring_wr = BEGINNING;
//    usb_tx_ring_rd = BEGINNING;
//    ///////////////////////////////////////////////////////////////////////////////
//
//    // Configure eUSCI_A0 for UART mode
//    UCA0CTLW0 = 0;
//    UCA0CTLW0 |=  UCSWRST ;              // Put eUSCI in reset
//    UCA0CTLW0 |=  UCSSEL__SMCLK;         // Set SMCLK as fBRCLK
//    UCA0CTLW0 &= ~UCMSB;                 // MSB, LSB select
//    UCA0CTLW0 &= ~UCSPB;                 // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
//    UCA0CTLW0 &= ~UCPEN;                 // No Parity
//    UCA0CTLW0 &= ~UCSYNC;
//    UCA0CTLW0 &= ~UC7BIT;
//    UCA0CTLW0 |=  UCMODE_0;
//    //    BRCLK   Baudrate UCOS16  UCBRx  UCFx    UCSx    neg   pos  neg  pos
//    //    8000000  115200     1       4     5     0x55   -0.80 0.64 -1.12 1.76
//    //    UCA?MCTLW = UCSx + UCFx + UCOS16
//    UCA0BRW = 4 ;                        // 115,200 baud
//    UCA0MCTLW = 0x5551 ;
//    UCA0CTLW0 &= ~UCSWRST ;              // release from reset
//    UCA0TXBUF = 0x00;                    // Prime the Pump
//    UCA0IE |= UCRXIE;                    // Enable RX interrupt
//    //-----------------------------------------------------------------------------
//}
//
//
//
//
//void IOT_Process(void){               // Process IOT messages
//    int i;
//    unsigned int iot_rx_wr_temp;
//    iot_rx_wr_temp = iot_rx_wr;
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
//}
//
//
//
//////---------------------------------------------------------------------------
////void Init_Serial_UCA0(void){
////    int i;
////    for(i=0; i<SMALL_RING_SIZE; i++){
////        USB_Char_Rx[i] = 0x00;                  // USB Rx Buffer
////    }
////    usb_rx_ring_wr = BEGINNING;
////    usb_rx_ring_rd = BEGINNING;
////    for(i=0; i<LARGE_RING_SIZE; i++){         // May not use this
////        USB_Char_Tx[i] = 0x00;                  // USB Tx Buffer
////    }
////    usb_tx_ring_wr = BEGINNING;
////    usb_tx_ring_rd = BEGINNING;
////    // Configure UART 0
////    UCA0CTLW0 = 0;                            // Use word register
////    UCA0CTLW0 |= UCSWRST;                     // Set Software reset enable
////    UCA0CTLW0 |= UCSSEL__SMCLK;               // Set SMCLK as fBRCLK
////    /* 9,600 Baud Rate
////     // 1. Calculate N = fBRCLK / Baudrate
////     //     N = SMCLK / 9,600 => 8,000,000 / 9,600 = 833.3333333
////     // if N > 16 continue with step 3, otherwise with step 2
////     // 2. OS16 = 0, UCBRx = INT(N)
////     // continue with step 4
////     // 3. OS16 = 1,
////     //  UCx = INT(N/16),
////             = INT(N/16) = 833.333/16 => 52
////     // UCFx = INT([(N/16) –INT(N/16)] × 16)
////             = ([833.333/16-INT(833.333/16)]*16)
////             = (52.08333333-52)*16
////             = 0.083*16 = 1
////     // 4. UCSx is found by looking up the fractional part of N (= N-INT(N)) in table Table 18-4
////     //     Decimal of SMCLK / 8,000,000 / 9,600 = 833.3333333 => 0.333 yields 0x49 [Table]
////     // 5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed
////     //                                                TX error (%) RX error (%)
////     //  BRCLK   Baudrate UCOS16  UCBRx UCFx UCSx neg pos  neg pos
////     //  8000000    9600     1     52      1     0x49   -0.08 0.04 -0.10 0.14
////     */
////    UCA0BRW = 52; // 9,600 Baud
////    //  UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
////    //  UCA0MCTLW = 0x49  concatenate 1      concatenate 1;
////    UCA0MCTLW = 0x4911 ;
////    UCA0CTLW0 &= ~ UCSWRST;             // Set Software reset enable
////    UCA0IE |= UCRXIE;                   // Enable RX interrupt
////}
