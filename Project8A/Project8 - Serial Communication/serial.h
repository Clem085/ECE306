/*
 * serial.h
 *
 *  Created on: Nov 8, 2024
 *      Author: jammi
 */

#ifndef SERIAL_H_
#define SERIAL_H_

// #defines
#define BEGINNING            (0)
#define SMALL_RING_SIZE     (16)

 // global variables
 volatile unsigned int usb_rx_ring_wr;
 volatile char USB_Char_Rx[SMALL_RING_SIZE];


void Init_Serial_UCA0(char speed);



#endif /* SERIAL_H_ */
