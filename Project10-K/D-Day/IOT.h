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
unsigned int displayclr_flag;
unsigned int system_initialization;
unsigned int tx_index;
char IOT_Ring_Rx[32]; // LARGE_RING_SIZE
char iot_TX_buf[32]; // LARGE_RING_SIZE
char ssid_string[10];
char ip_string1[10];
char ip_string2[10];
int iot_parsing_counter;

// Experiemental
unsigned int response_parse;
unsigned int movement;
unsigned int setTime;
extern unsigned int Waitcount;
extern unsigned int FlagWait;

//// Externally Defined
extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int initialize_done;
extern unsigned int iot_on_time;
extern unsigned int state;

// Macro Definitions
#define IR_MAGIC_NUM (500)



// Function Declarations
void IR_control(void);
void iot_process(void);
void iot_on(void);
void iot_boot_sequence(void);
void iot_command_parse(void);

#endif /* IOT_H_ */
