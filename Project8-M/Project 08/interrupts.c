/* Interrupts Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : interrupts.c
  Description:  Contains all Interrupt Service Routines (ISRs) for:
                - Timer interrupts (system timing)
                - ADC conversions (sensor reading)
                - Serial communication (UART0 and UART1)
  Programmer: Molana
  Date Created: March, 2025
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */

//// Header Files
#include  "msp430.h"          // MSP430 MCU headers
#include  "functions.h"       // Common utility functions
#include  "LCD.h"            // LCD display control
#include  "ports.h"          // Port and pin definitions
#include  "macros.h"         // System macros and constants
#include  "motors.h"         // Motor control functions
#include  "Display.h"        // Display handling
#include  "timers.h"         // Timer configurations
#include  "interrupts.h"     // Interrupt handling
#include  "switches.h"       // Switch debouncing
#include  "ADC.h"           // Analog-to-Digital Converter
#include  "IR.h"            // Infrared sensor interface
#include  "serial.h"        // Serial communication
#include  "menu.h"          // Menu system interface
//// Libraries
#include  <string.h>        // String manipulation
#include  <stdio.h>         // Standard I/O functions

// External Variable Declarations
extern volatile char USB_Ring_Rx[11];   // USB receive buffer (UART1)
extern char iot_TX_buf[11];            // IoT transmit buffer (UART0)
extern unsigned int iot_rx_wr;         // IoT receive buffer write index
extern char IOT_Ring_Rx[11];           // IoT receive ring buffer (UART0)
extern volatile unsigned int iot_tx;    // IoT transmit buffer index
extern volatile unsigned int usb_rx_ring_wr; // USB receive buffer write index
extern volatile unsigned int ncsu_index; // Serial communication buffer index
extern volatile unsigned int localCounter; // System tick counter
extern volatile unsigned int debounce_count1; // Switch 1 debounce counter
extern volatile unsigned int debounce_count2; // Switch 2 debounce counter

/**
 * Timer B0 CCR0 Interrupt Handler
 * - Maintains system timing
 * - Updates display refresh flag
 * - Handles basic debounce counting
 * Runs at regular intervals defined by TB0CCR0_INTERVAL
 */
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    // Reset debounce counter if it reaches threshold
    if(debounce_count1 >= 10){
        debounce_count1 = 0;
    }

    localCounter++;        // Increment system tick counter
    update_display = TRUE; // Set flag to refresh display
    Time_Sequence++;       // Increment system time counter

    // Reset timer interval
    TB0CCR0 += TB0CCR0_INTERVAL;
}

/**
 * ADC Conversion Complete Interrupt Handler
 * - Manages multi-channel ADC sampling sequence (A2, A3, A5)
 * - Processes and stores converted values
 * - Updates display when enabled
 */
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
        case ADCIV_NONE: break;       // No interrupt pending
        case ADCIV_ADCOVIFG: break;   // ADC overflow
        case ADCIV_ADCTOVIFG: break;  // ADC timeout
        case ADCIV_ADCIFG:           // ADC conversion complete
            ADCCTL0 &= ~ADCENC;      // Disable conversions during processing

            switch (ADC_Channel++){
                case 0x00: // Channel A2 (Left IR Detector)
                    ADCMCTL0 &= ~ADCINCH_2;  // Disable current channel
                    ADCMCTL0 |=  ADCINCH_3;  // Enable next channel (A3)

                    // Store and scale 12-bit result to 10-bit
                    ADC_Left_Detect = ADCMEM0 >> 2;

                    // Update display if enabled
                    if(ADC_Update && ADC_Display){
                        HexToBCD(ADC_Left_Detect);
                        dispPrint(adc_char,'2'); // Display on line 2
                    }
                    break;

                case 0x01: // Channel A3 (Right IR Detector)
                    ADCMCTL0 &= ~ADCINCH_3;  // Disable current channel
                    ADCMCTL0 |=  ADCINCH_5;  // Enable next channel (A5)

                    ADC_Right_Detect = ADCMEM0 >> 2;
                    if(ADC_Update && ADC_Display){
                        HexToBCD(ADC_Right_Detect);
                        dispPrint(adc_char,'3'); // Display on line 3
                    }
                    break;

                case 0x02: // Channel A5 (Thumbwheel)
                    ADCMCTL0 &= ~ADCINCH_5;  // Disable current channel
                    ADCMCTL0 |= ADCINCH_2;   // Restart sequence at channel A2

                    ADC_Thumb = ADCMEM0 >> 2;
                    if(ADC_Update && ADC_Display){
                        HexToBCD(ADC_Thumb);
                        dispPrint(adc_char,'4'); // Display on line 4
                    }

                    // Additional thumbwheel processing
                    ADC_Temp = ADC_Thumb >> 5; // Further scaling
                    if(ADC_Prev == 8192){ // Initial value check
                        ADC_Prev = ADC_Temp;
                    }
                    ADC_Channel = 0; // Reset channel sequence
                    break;

                default: break;
            }

            // Re-enable ADC for next conversion
            ADCCTL0 |= ADCENC;
            ADCCTL0 |= ADCSC;
            break;

        default: break;
    }
}

/**
 * UART0 (EUSCI_A0) Interrupt Handler
 * - Handles both transmission and reception for primary serial port
 * - Manages IOT_Ring_Rx receive buffer
 * - Forwards received data to UART1
 */
#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    char iot_receive;
    switch(__even_in_range(UCA0IV,0x08)){
        case 0: break; // No interrupt

        case 2: // Receive interrupt (Rx0IFG)
            iot_receive = UCA0RXBUF; // Read received byte

            // Handle null terminator specially
            if (iot_receive == 0x00){
                IOT_Ring_Rx[iot_rx_wr++] = iot_receive;
                iot_rx_wr = BEGINNING; // Reset buffer index
            }
            else{
                // Store in ring buffer
                IOT_Ring_Rx[iot_rx_wr++] = iot_receive;
                // Wrap around if buffer full
                if(iot_rx_wr >= sizeof(IOT_Ring_Rx)){
                    iot_rx_wr = BEGINNING;
                }
            }

            // Echo to UART1
            UCA1TXBUF = iot_receive;
            break;

        case 4: // Transmit interrupt (Tx0IFG)
            // Send next byte from transmit buffer
            UCA0TXBUF = iot_TX_buf[iot_tx];
            iot_TX_buf[iot_tx++] = 0; // Clear sent byte

            // Disable transmit interrupt if buffer empty
            if(iot_TX_buf[iot_tx] == 0x00){
                UCA0IE &= ~UCTXIE;
                iot_tx = 0; // Reset transmit index
            }
            break;

        default: break;
    }
}

/**
 * UART1 (EUSCI_A1) Interrupt Handler
 * - Handles USB serial communication
 * - Manages USB_Ring_Rx receive buffer
 * - Transmits data from iot_TX_buf
 */
#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    char usb_value;
    switch(__even_in_range(UCA1IV,0x08)){
        case 0: break; // No interrupt

        case 2: // Receive interrupt (Rx1IFG)
            usb_value = UCA1RXBUF; // Read received byte
            USB_Ring_Rx[usb_rx_ring_wr++] = usb_value; // Store in buffer

            // Handle newline as end of message
            if (usb_value == 0x0A){
                USB_Ring_Rx[usb_rx_ring_wr] = 0x00; // Null terminate
                usb_rx_ring_wr = BEGINNING; // Reset buffer index
                UCA0IE &= ~UCRXIE; // Disable UART0 receive
            }

            // Wrap around if buffer full
            if(usb_rx_ring_wr >= sizeof(USB_Ring_Rx)){
                usb_rx_ring_wr = BEGINNING;
            }
            break;

        case 4: // Transmit interrupt (Tx1IFG)
            // Send next byte if available
            if (iot_TX_buf[ncsu_index] != '\0'){
                UCA1TXBUF = iot_TX_buf[ncsu_index++];

                // Wrap around if buffer end reached
                if(ncsu_index >= sizeof(iot_TX_buf)){
                    ncsu_index = BEGINNING;
                }

                // Stop transmitting if caught up with receive index
                if(iot_rx_wr == ncsu_index){
                    UCA1IE &= ~UCTXIE;
                    ncsu_index = 0;
                }
            }
            else {
                // Buffer empty - disable transmit interrupt
                UCA1IE &= ~UCTXIE;
                ncsu_index = 0;
            }
            break;

        default: break;
    }
}
