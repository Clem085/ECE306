//===========================================================================
//  File Name: interrupts.h
//  Description: Header file for interrupts.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Oct 21, 2024
//===========================================================================
#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

// Global Variables
//// Internally Defined
volatile unsigned int Time_Sequence;
char DAC_overflow_counter;
unsigned int run_time;
unsigned int run_time_flag;

//// Externally Defined
extern volatile unsigned char update_display;
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern unsigned int count_debounce_SW1;
extern unsigned int count_debounce_SW2;
extern volatile unsigned int ADC_Channel;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
extern volatile unsigned int ADC_Thumb;
extern char state;
extern char adc_char[10];
extern char IR_status;
extern char IR_changed;
extern char ADC_Update;
extern char ADC_Display;
extern unsigned int DAC_data;
extern char menu;
extern char menuType;
extern char songInit;
extern unsigned int clear_display;
extern volatile char Rx_display[16];
extern char baud_flag;
// Debounce
extern volatile unsigned int debounce_count1;
extern volatile unsigned int debounce_count2;
extern volatile char switch_debounce_active;
// Serial
extern volatile char IOT_Ring_Rx[32];
extern volatile unsigned int iot_rx_wr;
extern volatile char iot_TX_buf[32];
extern volatile char USB_Ring_Rx[32];
extern volatile char USB_Char_Tx[32];
extern volatile unsigned int usb_rx_ring_wr;
extern volatile unsigned int usb_tx;
extern volatile unsigned int iot_tx;


// Macro Definitions
#define OVERFLOW (14)


// Function Declarations



#endif /* INTERRUPTS_H_ */
