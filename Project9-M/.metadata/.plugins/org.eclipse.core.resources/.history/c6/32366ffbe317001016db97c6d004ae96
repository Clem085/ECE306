//===========================================================================
//  File Name: serial.h
//
//  Description: Serial Communication Interface Header
//               - Defines global variables and constants for UART communication
//               - Declares functions for serial port initialization and control
//               - Supports baud rate configuration and data transmission
//
//  Programmer: Molana
//  Date Created: March, 2025
//===========================================================================

#ifndef SERIAL_H_
#define SERIAL_H_

/* ===========================================================================
 * Global Variables
 * ===========================================================================*/

char baud_toggle;  // Toggle flag for baud rate switching between:
                   // - 0: Default baud rate (115200)
                   // - 1: Alternate baud rate (460800)

/* ===========================================================================
 * Macro Definitions
 * ===========================================================================*/

#define BEGINNING      (0)    // Initial index value for ring buffers
#define SMALL_RING_SIZE (16)  // Size of small ring buffers (USB/IOT Char Rx/Tx)
#define LARGE_RING_SIZE (32)  // Size of large ring buffers (process buffers)
#define CHARACTER      (0)    // Base value for character processing

/* ===========================================================================
 * Function Declarations
 * ===========================================================================*/

/**
 * Initializes UART port UCA0 with specified baud rate
 * @param speed Baud rate selection (1=115200, 2=460800)
 * Configures:
 * - Clock source (SMCLK)
 * - Data format (8N1)
 * - Ring buffers
 * - Interrupts
 */
void Init_Serial_UCA0(char speed);

/**
 * Updates display with current baud rate information
 * Shows active baud rate on LCD when called
 */
void baud_rate_disp(void);

/**
 * Initiates transmission on UCA0
 * Requirements:
 * - Data must be in process_buffer
 * - Buffer must be NULL-terminated
 * Enables transmit interrupts to start sending
 */
void USCI_A0_transmit(void);

#endif /* SERIAL_H_ */
