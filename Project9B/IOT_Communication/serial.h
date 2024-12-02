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


// ----- ----- ----- PROJECT 9 CODE ----- ----- ----- //
////// Internally Defined
char transmit_state;
//unsigned int ssid_index;
//unsigned int ip_index1;
//unsigned int ip_index2;
unsigned int ssid_record_flag;
unsigned int ip_record_flag;
unsigned int period_record;
unsigned int group1_flag;
unsigned int group2_flag;
volatile unsigned int iot_receive;

////// Externally Defined



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
//void Init_Serial_UCA0(char speed);
//void Init_Serial_UCA1(char speed);
/// /// /// TEMP PROJECT 8 CODE /// /// ///

// ----- ----- ----- PROJECT 9 CODE ----- ----- ----- //
void Init_Serial_UCA0(void);
void Init_Serial_UCA1(void);
// ----- ----- ----- PROJECT 9 CODE ----- ----- ----- //



#endif /* SERIAL_H_ */
