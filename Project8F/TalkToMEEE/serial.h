//===========================================================================
//  File Name: serial.h
//  Description: Header file for serial.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Nov 8, 2024
//===========================================================================
#ifndef SERIAL_H_
#define SERIAL_H_

// Global Variables
////// Internally Defined
//volatile unsigned int iot_rx_wr;
//volatile char iot_TX_buf[16];
//volatile unsigned int iot_tx;
//volatile unsigned int iot_receive;
//char pb_index;
//unsigned int transmit_done;
//char display_line[4][11];
//volatile unsigned char update_display;
//volatile unsigned char display_changed;
//char baud_toggle;
//volatile char USB_Ring_Rx[16];
//volatile char IOT_Ring_Rx[16];
//volatile char Rx_display[16];
//char baud_flag;
//
////// Externally Defined
//extern volatile unsigned char IOT_2_PC[16];
//extern volatile unsigned char PC_2_IOT[16];
//extern unsigned int direct_iot;
//extern volatile unsigned int usb_rx_wr;
//extern unsigned int clear_display;

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
#define SMALL_RING_SIZE (16)
#define LARGE_RING_SIZE (32)
#define CHARACTER       (0)

// Function Declarations
void Init_Serial_UCA0(char);
void baud_rate_disp(void);
void USCI_A0_transmit(void);





#endif /* SERIAL_H_ */
