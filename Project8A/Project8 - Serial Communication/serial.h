//===========================================================================
//  File Name: serial.h
//  Description: Header file for serial.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Nov 8, 2024
//===========================================================================
#ifndef SERIAL_H_
#define SERIAL_H_

// Global Variables
//// Internally Defined
volatile unsigned int iot_rx_wr;
volatile char iot_TX_buf[16];
volatile unsigned int iot_tx;
volatile unsigned int iot_receive;
char pb_index;
unsigned int transmit_done;
char display_line[4][11];
volatile unsigned char update_display;
volatile unsigned char display_changed;
char baud_toggle;
volatile char USB_Ring_Rx[16];
volatile char IOT_Ring_Rx[16];
volatile char Rx_display[16];
char baud_flag;

//// Externally Defined
extern volatile unsigned char IOT_2_PC[16];
extern volatile unsigned char PC_2_IOT[16];
extern unsigned int direct_iot;
extern volatile unsigned int usb_rx_wr;
extern unsigned int clear_display;



// Macro Definitions
#define BEGINNING            (0)
//#define SMALL_RING_SIZE     (16) // Must be placed ABOVE USB_Char_Rx[], Just set to 16 manually each place its used



// Function Declarations
void Init_Serial_UCA0(void);
void baud_rate_disp(void);
void USCI_A0_transmit(void);





#endif /* SERIAL_H_ */
