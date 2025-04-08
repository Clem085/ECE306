/* Main Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : main.c
  Description:  This file contains the Main Routine - "While" Operating System
  Programmer: Connor Savugot
  Date Created: Oct 14, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */

// #include as of 11-10-24
//// Header Files
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "motors.h"
#include  "Display.h"
#include  "timers.h"
#include  "interrupts.h"
#include  "switches.h"
#include  "ADC.h"
#include  "IR.h"
#include  "serial.h"
#include  "DAC.h"
#include  "menu.h"
//// Libraries
#include  <string.h>
#include  <stdio.h>

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void IOT_Process(void);
void Init_LEDs(void);
void Project8_StateMachine(void);
void Project9_StateMachine(void);
void drunkDriver_StateMachine(void);


// Global Variables
//// Internal Globals
char process_buffer[25]; // Size for appropriate Command Length
volatile char AT[11];
volatile unsigned int ip_address_found;
volatile unsigned int localCounter;


char process_buf[25];
int baudLow = 0;
volatile extern char Baud_Rate1[] = "115,200";
volatile extern char Baud_Rate2[] = "460,800";
volatile unsigned int requested_move;

//// External Globals

extern unsigned int iot_index;
extern char IOT_Data[11][11];
extern volatile unsigned int boot_state;
extern volatile unsigned int IOT_parse;
extern volatile char switch_debounce_active;
extern int ip_address[11];
extern volatile char USB_Char_Rx[16]; // SMALL_RING_SIZE = 16
extern volatile unsigned int pause_counter;
extern volatile unsigned int pause_counter1;
extern char pb_index;  // Index for process_buffer
extern char IOT_Ring_Rx[11];

extern char iot_TX_buf[11];
extern volatile int switch1_pressed;  // Declare the variable as external
extern volatile int switch2_pressed;  // Declare the variable as external
extern volatile unsigned int iot_rx_wr;

void main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    Time_Sequence = 0;

    PM5CTL0 &= ~LOCKLPM5;
    Init_Ports();
    Init_Clocks();
    Init_Conditions();
    Init_LCD();
    Init_Timer_B0();
    Init_Timer_B3();
    Init_DAC();
    //Init_ADC();

    //Char speed value subject to change
    Init_Serial_UCA0(1);

    baudLow = 0;
    UCA0BRW = 4;// 115,200 Baud
    UCA0MCTLW = 0x5551;

    strcpy(display_line[0], "  Waiting ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "BR:115,200");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;

    int i;
    for (i = 0; i < sizeof(IOT_Ring_Rx); i++){
        IOT_Ring_Rx[i] = '\0';
    }

    while(ALWAYS) {
        display_changed = TRUE;
        Display_Process();

        IOT_Process();

        //For Project 6 / 7 that stay inside the while loop
        // and inside an if statement for if switch 1 was pressed:
        //requested_move = STILL;
        //StateMachine();
        //LEFT_FORWARD_SPEED = SLOW; // Set Port pin High [Wheel On]
        //RIGHT_FORWARD_SPEED = SLOW; // Set Port pin High [Wheel On]
        //P6SEL0 |= L_FORWARD;
        //P6SEL0 |= R_FORWARD;
        //Display_ADC();
        //pause_counter = 0;
        //pause_counter1 = 0;
        //DAC_Procress();
        //Display_ADC();

        //    If the Ring buffer isn't null / empty
        //    place characters inside process buffer
        // And display the message on the bottom line
        Project8_StateMachine();
    }
}


void Project8_StateMachine(void){
    if(IOT_Ring_Rx[0] != '\0' && switch2_pressed == 0 && switch1_pressed == 0){
        strcpy(process_buf, IOT_Ring_Rx);
        strcpy(display_line[0], " Received");

        if (baudLow == 0){
            strcpy(display_line[2], "BR:115,200");
        }
        else {
            strcpy(display_line[2], "BR:460,800");
        }
        strcpy(display_line[3], process_buf);

        unsigned int i;
        for (i = 0; i < sizeof(IOT_Ring_Rx); i++){
            IOT_Ring_Rx[i] = '\0';
            //process_buf[i] = '\0';
        }
        iot_rx_wr = BEGINNING;
    }

    if (switch1_pressed == 1 && switch2_pressed == 0){
        //Change display to enter current baud rate
        strcpy(display_line[0], " Transmit ");

        strcpy(iot_TX_buf, process_buf);

        // Clear second line in case another message is there
        //strcpy(display_line[1], "          ");

        // Clear the third line
        strcpy(display_line[3], "          ");

        UCA0IE |= UCTXIE;

        // Move message to second line
        int i;
        for (i = 0; i < strlen(process_buf); i++){
            display_line[1][i] = process_buf[i];
        }
        for (i = strlen(process_buf); i < sizeof(display_line[1]); i++){
            display_line[1][i] = ' ';
        }

        display_changed = TRUE;

        pb_index = 0;


        for (i = 0; i < sizeof(IOT_Ring_Rx); i++){
            IOT_Ring_Rx[i] = 0x00;
            process_buf[i] = 0x00;
        }
        //iot_rx_wr = BEGINNING;

        switch1_pressed = 0;
        switch_debounce_active = 0;

        // enable interrupts again
        P4IE |= SW1;
        P2IE |= SW2;
    }

    if (switch2_pressed == 1 && switch1_pressed == 0){
        // If baud rate is currently 460800
        if (baudLow == 1){
            // Allows case 2 to occur when SW2 is pressed again
            baudLow = 0;
            strcpy(display_line[2], "BR:115,200");
            UCA0BRW = 4;// 115,200 Baud
            UCA0MCTLW = 0x5551;
        }
        // Otherwise the baud rate is currently 115200
        else{
            // Allows case 1 to occur when SW2 is pressed again
            baudLow = 1;
            strcpy(display_line[2], "BR:460,800");
            UCA0BRW = 17;// 460,800 Baud
            UCA0MCTLW = 0x4A00;
        }
        display_changed = TRUE;

        switch2_pressed = 0;
        switch_debounce_active = 0;

        // enable interrupts again
        P4IE |= SW1;
        P2IE |= SW2;
    }
}


/// So when you take in the interrupt service routine, you put characters into the ring buffer.
/// And in main you're going to take them out of the ring buffer. So just like.


//------------------------------------------------------------------------------
