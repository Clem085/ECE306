//------------------------------------------------------------------------------
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "StateMachine.h"
#include  "ADC.h"
#include  "DAC.h"

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Init_DAC(void);
void Init_ADC(void);
void Display_Process(void);
void Init_LEDs(void);
void Init_Clocks(void);
void Init_LCD(void);

void Display_DAC(void);
void DAC_Procress(void);
void Display_ADC(void);

void Init_Timer_B0(void);
void Init_Timer_B3(void);

void Init_Serial_UCA0(char speed);
void Init_Serial_UCA1(char speed);

void USCI_A0_transmit(void);

void IOT_Process(void);

extern volatile unsigned int Time_Sequence = 0;

volatile int switch1_pressed = 0;

volatile int switch2_pressed = 0;

// Global Variables
char process_buffer[25]; // Size for appropriate Command Length
char pb_index;  // Index for process_buffer

extern unsigned int iot_rx_wr;

extern unsigned int iot_rx_rd;

extern unsigned int iot_index;

extern char IOT_Data[11][11];

extern char IOT_Ring_Rx[11];

extern char ip_mac[11];

extern volatile unsigned int iot_tx;

extern volatile unsigned int boot_state;

extern volatile unsigned int IOT_parse;

extern volatile char switch_debounce_active;

extern int ip_address[11];

volatile char AT[11];

extern volatile unsigned int usb_rx_ring_wr;
extern volatile char USB_Char_Rx[SMALL_RING_SIZE];

extern volatile unsigned int pause_counter;
extern volatile unsigned int pause_counter1;

volatile unsigned int test_Value;

volatile unsigned int ip_address_found;

volatile unsigned int localCounter;


volatile unsigned int ncsu_index = 0;

char iot_TX_buf[11];

volatile char USB_Ring_Rx[11];

char process_buf[25];

int baudLow = 0;

unsigned int nextline;
int unsigned line = 0;
int character = 0;

extern volatile char NCSU[] = "NCSU  #1  ";
volatile extern char Baud_Rate1[] = "115,200";
volatile extern char Baud_Rate2[] = "460,800";

volatile unsigned int requested_move;

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
    Init_Serial_UCA1(1);
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

    Display_Process();

//    IOT_Process();

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
    if(IOT_Ring_Rx[0] != NULL && switch2_pressed == 0 && switch1_pressed == 0){
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
}

void USCI_A0_transmit(void){// Transmit Function for USCI_A0
     // Contents must be in process_buffer
     // End of Transmission is identified by NULL character in process_buffer
     // process_buffer includes Carriage Return and Line Feed
     pb_index = 0;                          // Set Array index to first location
     UCA0IE |= UCTXIE;                      // Enable TX interrupt
 }

// Process IOT messages
void IOT_Process(void){
    unsigned int iot_rx_wr_temp;

    //iot_rx_wr_temp = current index for IOT_Data
    iot_rx_wr_temp = iot_rx_wr;
    if(iot_rx_wr_temp != iot_rx_rd){    // Determine if IOT is available
       //IOT Data is your processor buffer
       IOT_Data[line][character] = IOT_Ring_Rx[iot_rx_rd++];
       if(iot_rx_rd >= sizeof(IOT_Ring_Rx)){
         iot_rx_rd = BEGINNING;
       }

       if(IOT_Data[line][character] == 0x0A){
         character = 0;
         line++;
         if(line >= 4){
           line = 0;
         }
         int nextline = line + 1;
         if (nextline >= 4){
           nextline = 0;
         }
       }
       else {
           switch(character){
                case  0:
                  if(IOT_Data[line][character] == '+'){   // Got "+"
                    test_Value++;
                    if(test_Value){
                      P1SEL0 |= RED_LED;
                    }
                    IOT_parse = 1;
                  }
                  break;
                case  1:
                  // GRN_LED_ON;
                  break;
                case  4:
                  if(IOT_Data[line][character] == 'y'){   // Got read"y"
                      int i =0;
                    for(i=0; i < sizeof(NCSU); i++){
                      iot_TX_buf[i] = NCSU[i];
                    }
                    UCA0IE |= UCTXIE;
                    boot_state = 1;
                    P6SEL0 |= GRN_LED;
                  }
                  break;
                case  5:
                        if(IOT_Data[line][character] == 'G'){      // Got IP
                            int i;
                          for(i=0; i<sizeof(ip_mac); i++){
                            iot_TX_buf[i] = ip_mac[i];
                          }
                          iot_tx = 0;
                          UCA0IE |= UCTXIE;
                        }
                        break;
                      case  6:
                        break;
                      case  10:
                        if(IOT_Data[line][character] == 'I'){
                          ip_address_found = 1;
                          strcpy(display_line[0], "IP Address");
                          int i =0;
                          for(i=0; i<sizeof(ip_address); i++){
                            ip_address[i] = 0;
                          }
                          display_changed = 1;
                          iot_index = 0;
                       }
                        break;
                      default: break;
              }
         }
     }
}

/// So when you take in the interrupt service routine, you put characters into the ring buffer.
/// And in main you're going to take them out of the ring buffer. So just like.


//------------------------------------------------------------------------------
