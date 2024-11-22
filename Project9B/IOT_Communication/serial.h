//===========================================================================
//  File Name: serial.h
//  Description: Header file for serial.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Nov 8, 2024
//===========================================================================
#ifndef SERIAL_H_
#define SERIAL_H_

// Globals Only work in serial.c because magic???







////// PROJECT 8 TEMP
//// MINE
char baud_toggle;

////// Internally Defined
//volatile unsigned int boot_state;
//volatile unsigned int IOT_parse;
//
//volatile unsigned int iot_rx_wr;
//volatile unsigned int iot_rx_rd;
//volatile unsigned int iot_tx;
//volatile char USB_Char_Rx[11];
//volatile char USB_Char_Tx[11];
//volatile char IOT_Char_Rx[11];
//volatile char IOT_Char_Tx[11];
//char IOT_Data[11][11];
//char IOT_Ring_Rx[11];
//volatile unsigned int iot_index;
//volatile int ip_address[11];
//volatile int ip_address_found;
////unsigned int ip_mac = 0x00;
//volatile unsigned int test_Value;
////int character = 0;
//volatile char iot_TX_buf[11]; // removed volatile
//volatile unsigned int iot_tx;
//volatile unsigned int usb_rx_ring_rd;
//volatile unsigned int usb_tx_ring_wr;
//volatile unsigned int usb_tx_ring_rd;
//char ip_mac[11];
////char ncsu_str[10]; Special Case, must be managed elsewhere
//
////// Externally Defined
//extern volatile char USB_Ring_Rx[11];
////extern volatile char process_buf[25];
//extern int unsigned line;


// Macro Definitions
#define BEGINNING            (0)
//#define SMALL_RING_SIZE     (16) // Must be placed ABOVE USB_Char_Rx[], Just set to 16 manually each place its used
//#define LARGE_RING_SIZE (32)
#define CHARACTER       (0)

// Function Declarations
//void Init_Serial_UCA0(char);
//void baud_rate_disp(void);
//void USCI_A0_transmit(void);
/// /// /// TEMP PROJECT 8 CODE /// /// ///
void Init_Serial_UCA0(char speed);
void Init_Serial_UCA1(char speed);
/// /// /// TEMP PROJECT 8 CODE /// /// ///





#endif /* SERIAL_H_ */