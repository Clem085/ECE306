//===========================================================================
//  File Name: serial.h
//  Description: Header file for serial.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Nov 8, 2024
//===========================================================================
#ifndef SERIAL_H_
#define SERIAL_H_

// Must be placed ABOVE USB_Char_Rx[]
#define SMALL_RING_SIZE     (16)

// Global Variables
//// Internally Defined
 volatile unsigned int usb_rx_ring_wr;
 volatile char USB_Char_Rx[SMALL_RING_SIZE];
//// Externally Defined



// Macro Definitions
#define BEGINNING            (0)
//#define SMALL_RING_SIZE     (16) // Must be placed ABOVE USB_Char_Rx[]



// Function Declarations
void Init_Serial_UCA0(char speed);





#endif /* SERIAL_H_ */
