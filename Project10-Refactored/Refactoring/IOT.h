//===========================================================================
//  File Name: IOT.h
//  Description: Header file for IOT.c - Contains Globals, Macros, & function declarations
//  Author: Connor Savugot
//  Date: Nov 8, 2024
//===========================================================================
#ifndef IOT_H_
#define IOT_H_


// Global Variables
//// Variables Initialized with a Value are in IOT.c
//// Internally Defined
unsigned int command_execution_flag;
unsigned int display_clear_flag;
unsigned int system_initialization;
unsigned int tx_index;
char IOT_Ring_Rx[32]; // LARGE_RING_SIZE
char iot_TX_buf[32]; // LARGE_RING_SIZE
char ssid_display[10];
char ip_display1[10];
char ip_display2[10];
int iot_parsing_counter;
unsigned int response_index;
unsigned int movement_flag;
unsigned int stopMoving;

//// Externally Defined
extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int system_initialization;
extern unsigned int iot_boot_time;
extern unsigned int state;

// Macro Definitions
#define IR_MAGIC_NUM (500)



// Function Declarations
void IR_control(void);
void iot_process(void);
void iot_on(void);
void iot_boot_sequence(void);
void iot_command_parse(void);
void clear_display_buffer(void);

#endif /* IOT_H_ */
