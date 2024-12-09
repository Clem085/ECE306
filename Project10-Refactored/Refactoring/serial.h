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

//// Externally Defined



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
