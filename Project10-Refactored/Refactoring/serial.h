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
unsigned int iot_rx_wr;
unsigned int iot_tx;
unsigned int iot_receive;
unsigned int serial_char;
unsigned int iot_char;
char serial_rx_wr;
unsigned int transmit_done;
unsigned char display_changed;
unsigned int ssid_record_flag;
unsigned int ip_record_flag;
unsigned int period_record;
unsigned int ip1_flag;
unsigned int ip2_flag;
unsigned int ssid_index;
unsigned int ip_index1;
unsigned int ip_index2;
unsigned int ping;
//// Externally Defined
extern unsigned int tx_index;
extern char IOT_Ring_Rx[32];
extern char ssid_display[10];
extern char ip_display1[10];
extern char ip_display2[10];
extern char iot_TX_buf[32];
extern unsigned int clear_display;


// Macro Definitions
#define BEGINNING            (0)
#define SMALL_RING_SIZE     (16)
#define LARGE_RING_SIZE     (32)


// Function Declarations
void Init_Serial_UCA0(void);
void Init_Serial_UCA1(void);
void ping_function(void);
void USCI_A0_transmit(void);
void USCI_A1_transmit(void);


#endif /* SERIAL_H_ */
