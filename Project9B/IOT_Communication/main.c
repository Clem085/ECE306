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
unsigned int test_value;
char chosen_direction;
char change;
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;
unsigned char dispEvent;
unsigned char event;
unsigned int travel_distance;
unsigned int right_count_time;
unsigned int left_count_time;
unsigned int wheel_count_time;
unsigned int time_change;
unsigned int delay_start;
unsigned int cycle_time;
unsigned int right_motor_count;
unsigned int left_motor_count;
unsigned int segment_count;
unsigned int straight_step;
unsigned int circle_step;
unsigned int circle_step2;
unsigned int triangle_step;
unsigned int figure8_step;
int activateSM;
unsigned int transmit_count;
char state;
unsigned int clear_display;
volatile int switch1_pressed = 0;
volatile int switch2_pressed = 0;
volatile char AT[11];
volatile unsigned int test_Value;
volatile unsigned int ip_address_found;
volatile unsigned int localCounter;
volatile unsigned int ncsu_index = 0;
volatile char USB_Ring_Rx[32];
char process_buf[25];
int baudLow = 0;
unsigned int nextline;
int unsigned line = 0;
int character = 0;
volatile extern char Baud_Rate1[] = "115,200";
volatile extern char Baud_Rate2[] = "460,800";
volatile unsigned int requested_move;
char process_buffer[25]; // Size for appropriate Command Length
char pb_index;  // Index for process_buffer
unsigned int pro_inc; // Increment for process buffer
// ----- ----- ----- PROJECT 9 CODE ----- ----- ----- //
volatile char stop_save_sett[] = "AT+SYSSTORE=0\r\n";
volatile char update_conn_sett[] = "AT+CIPMUX=1\r\n";
volatile char conf_server[] = "AT+CIPSERVER=1,8080\r\n";
volatile char SSID_access[] = "AT+CWJAP?\r\n";
volatile char IP_access[] = "AT+CIFSR\r\n";
volatile char check_okay[] = "AT\r\n";
volatile char IOT_Ring_Rx[32];


volatile char iot_TX_buf[32];
volatile char ssid_display[10];
volatile unsigned int legacy;
unsigned int command4_flag;
volatile char ip_display1[10];
volatile char ip_display2[10];
extern unsigned int initialize_done;
extern unsigned int run_time;
volatile char command_op;
extern unsigned int run_time_flag;
unsigned int response_parse;
int vv;
unsigned int distance;
unsigned int running;





//// External Globals
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern char backlight_status;
extern short int p3_4_type;
extern volatile unsigned int ADC_Channel;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
extern volatile unsigned int ADC_Thumb;
extern char IR_status;
extern char IR_changed;
extern char ADC_Update;
extern char ADC_Display;
extern char light_percent;
extern char menu;
extern char baud_toggle;
extern volatile unsigned int iot_rx_wr;
extern unsigned int iot_rx_rd;
extern unsigned int iot_index;
extern char IOT_Data[11][11];
extern volatile char IOT_Ring_Rx[32];
extern char ip_mac[11];
extern volatile unsigned int iot_tx;
extern volatile unsigned int boot_state;
extern volatile unsigned int IOT_parse;
extern volatile char switch_debounce_active;
extern int ip_address[11];
extern volatile unsigned int usb_rx_ring_wr;
extern volatile char USB_Char_Rx[SMALL_RING_SIZE];
extern volatile unsigned int pause_counter;
extern volatile unsigned int pause_counter1;
extern volatile char NCSU[] = "NCSU  #1  ";
extern char transmit_state;
extern unsigned int iot_on_time;

extern unsigned int ssid_index;
extern unsigned int ip_index1;
extern unsigned int ip_index2;



void main(void){
    //    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //------------------------------------------------------------------------------
    // Main Program
    // This is the main routine for the program. Execution of code starts here.
    // The operating system is Back Ground Fore Ground.
    //------------------------------------------------------------------------------
    PM5CTL0 &= ~LOCKLPM5;
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    p3_4_type = USE_GPIO;
    Init_Ports();                        // Initialize Ports
    Init_Clocks();                       // Initialize Clock System
    Init_Conditions();            // Initialize Variables and Initial Conditions
    Init_Timers();                       // Initialize Timers
    Init_LCD();                          // Initialize LCD
    Init_ADC();                          // Initialize ADC
    Init_DAC();                          // Initialize DAC

    /// /// /// TEMP PROJECT 8 CODE /// /// ///
//    Init_Serial_UCA0(1);
//    Init_Serial_UCA1(1);
    /// /// /// TEMP PROJECT 8 CODE /// /// ///

    // ----- ----- ----- PROJECT 9 CODE ----- ----- ----- //
    Init_Serial_UCA0();
    Init_Serial_UCA1();
    // ----- ----- ----- PROJECT 9 CODE ----- ----- ----- //

    //------------------------------------------------------------------------------
    // Beginning of the "While" Operating System
    //------------------------------------------------------------------------------
    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;

    ADC_Update = TRUE;
    ADC_Display = FALSE;
    IR_status = ON;
    light_percent = 80;
    menuType = IDLE;
    baud_toggle = 0;
//    state = STOP;

    /// /// /// TEMP PROJECT 8 CODE /// /// ///
//    baudLow = 0;
//    UCA0BRW = 4;// 115,200 Baud
//    UCA0MCTLW = 0x5551;
//
//    strcpy(display_line[0], "  Waiting ");
//    strcpy(display_line[1], "          ");
//    strcpy(display_line[2], "BR:115,200");
//    strcpy(display_line[3], "          ");
//    display_changed = TRUE;
//
//    int i;
//    for (i = 0; i < sizeof(IOT_Ring_Rx); i++){
//        IOT_Ring_Rx[i] = '\0';
//    }
//
//    pro_inc = 0;
    /// /// /// TEMP PROJECT 8 CODE /// /// ///

    // ----- ----- ----- PROJECT 9 CODE ----- ----- ----- //
    transmit_state = WAIT;
    iot_on_time = 0;
    unsigned int initial_process = 0;
    initialize_done = 0;
    // ----- ----- ----- PROJECT 9 CODE ----- ----- ----- //

    while (ALWAYS){                      // Can the Operating system run
        //        //        P3OUT ^= TEST_PROBE; // Toggle State of TEST_PROBE // Used to determine how fast main while loop is running // Toggles every time main while loop completes one full execution cycle
        //        // Updates
        //        Display_Process();
        //        IOT_Process(); // Must Run BEFORE State Machine for Serial Communications
        //                       // Actually maybe not, but try this first
        //
        //        // StateMachines
        //        Project9_StateMachine(); // Project 9 State Machine
        //        //        menuScroll();   // Homework 9 State Machine
        //
        //
        //        // Controls
        //        IR_control();
        //        PWM_backlight();
        //        vrfyDirection(); // Protects against Magic Smoke

        /// /// /// TEMP PROJECT 8 CODE /// /// ///
        display_changed = TRUE;
        Display_Process();


        // ----- ----- ----- PROJECT 9 CODE ----- ----- ----- //
        switch (initialize_done){
            case 0:
                if (iot_on_time < 15){
                    if (iot_on_time <= 10){
                        strcpy(display_line[0], "   IOT    ");
                        strcpy(display_line[1], "   OFF    ");
                        strcpy(display_line[2], "          ");
                        strcpy(display_line[3], "          ");
                        display_changed = TRUE;
                    }
                    else{
                        P3OUT |= IOT_EN;
                        strcpy(display_line[0], "   IOT    ");
                        strcpy(display_line[1], "   ON     ");
                        strcpy(display_line[2], "          ");
                        strcpy(display_line[3], "          ");
                        display_changed = TRUE;
                        initial_process = 1;
                    }
                }

                else if (iot_on_time < 100){
                    vv = 1;
                }


                else if (iot_on_time < 150){
                    switch (initial_process){
                    case 1:
                        vv = 0;
                        strcpy(display_line[3], "    1     ");
                        display_changed = TRUE;
                        if (iot_TX_buf[response_parse++] == '\r'){
        //                        response_parse--;
                            if (iot_TX_buf[response_parse-2] == 'P'){
                                strcpy(IOT_Ring_Rx, stop_save_sett);
                                UCA1IE |= UCTXIE;
                                initial_process = 2;
                                response_parse = 0;
            //                    legacy = 0;
                                while (IOT_Ring_Rx[vv] == 0x00){
                                    IOT_Ring_Rx[vv++] = 0;
                                }
                            }
                        }
                        break;
                    case 2:
                        vv= 0;
                        strcpy(display_line[3], "    2     ");
                        display_changed = TRUE;
                        if (iot_TX_buf[response_parse++] == '\r'){
                            if (iot_TX_buf[response_parse-2] == 'K'){
                                legacy = 0;
                                strcpy(IOT_Ring_Rx, update_conn_sett);
                                UCA1IE |= UCTXIE;
                                initial_process = 3;
                                response_parse = 0;
                            }
                        }
                        if (response_parse > 31){
                            response_parse = 0;
                        }
                        if (legacy > 31){
                            legacy = 0;
                        }
                        break;
                    case 3:
                        vv= 0;
                        strcpy(display_line[3], "    3     ");
                        display_changed = TRUE;
                        if (iot_TX_buf[response_parse++] == '\r'){
                            vv = 0;
                            if (iot_TX_buf[response_parse-10] == 'X'){
                                legacy = 0;
                                strcpy(IOT_Ring_Rx, conf_server);
                                UCA1IE |= UCTXIE;
                                initial_process = 4;
                                response_parse = 0;
                            }
                        }
                        if (response_parse > 31){
                            response_parse = 0;
                        }
                        if (legacy > 31){
                            legacy = 0;
                        }
                        break;
                    case 4:
                        vv= 0;
                        strcpy(display_line[3], "    4     ");
                        display_changed = TRUE;
                        if (iot_TX_buf[response_parse++] == '\r'){
                            vv = 0;
            //                if (response_parse-9 < 0){
                                if (iot_TX_buf[response_parse + 23] == '8'){
                                    legacy = 0;
                                    strcpy(IOT_Ring_Rx, SSID_access);
                                    UCA1IE |= UCTXIE;
                                    initial_process = 5;
                                    response_parse = 0;
            //                        command4_flag = 1;
                                }
            //                }
            //                else{
            //                    if (iot_TX_buf[response_parse - 9] == '8'){
            //                        legacy = 0;
            //                        strcpy(IOT_Ring_Rx, SSID_access);
            //                        UCA1IE |= UCTXIE;
            //                        initial_process = 5;
            //                        response_parse = 0;
            //                    }
            //                }
                        }
                        if (response_parse > 31){
                            response_parse = 0;
                        }
                        if (legacy > 31){
                            legacy = 0;
                        }
                        break;
                    case 5:
                        // printing the ssid
                        strcpy(display_line[3], "    5     ");
                        display_changed = TRUE;
                        strcpy(display_line[1], "          ");
                        strcpy(display_line[0], "          ");
                        strcpy(display_line[2], "          ");
                        strcpy(display_line[3], "          ");
                        display_changed = TRUE;
                        initial_process = 6;
                        break;
                    case 6:
                        vv= 0;
                        strcpy(display_line[3], "    6     ");
                        display_changed = TRUE;
            //            if (command4_flag){
                            if (iot_TX_buf[response_parse++] == '\r'){
                                vv = 0;
                                if (iot_TX_buf[response_parse-9] == ','){
                                    legacy = 0;
                                    strcpy(IOT_Ring_Rx, IP_access);
                                    UCA1IE |= UCTXIE;
                                    initial_process = 7;
                                    response_parse = 0;
                                    command4_flag = 0;
                                    strcpy(display_line[1], " SSID CAP ");
                                    strcpy(display_line[0], "          ");
                                                    display_changed = TRUE;
        //                            strcpy(display_line[0], ssid_display);
                                    strcpy(display_line[0], "ncsu      ");
                                    strcpy(display_line[2], "          ");
                                    strcpy(display_line[3], "          ");
                                    display_changed = TRUE;
                                }
                            }
            //            }
                        if (response_parse > 31){
                            response_parse = 0;
                        }
                        if (legacy > 31){
                            legacy = 0;
                        }
                        break;
                    case 7:
                        vv= 0;
                        strcpy(display_line[3], "    7     ");
                        display_changed = TRUE;
            //            if (command4_flag){
                        if (iot_TX_buf[response_parse++] == '\r'){
                            vv = 0;
                            if (iot_TX_buf[response_parse-8] == '"'){
                                legacy = 0;
                                strcpy(IOT_Ring_Rx, check_okay);
                                UCA1IE |= UCTXIE;
                                initial_process = 8;
                                response_parse = 0;
            //                    command4_flag = 0;
                                strcpy(display_line[1], " SSID CAP ");
                                strcpy(display_line[0], "          ");
                                                display_changed = TRUE;
        //                        strcpy(display_line[0], "ncsu      ");
        //                        strcpy(display_line[0], ssid_display);
        //                        strcpy(display_line[2], ip_display1);
        //                        strcpy(display_line[3], ip_display2);
                                display_changed = TRUE;
                            }
                        }
            //            }
                        if (response_parse > 31){
                            response_parse = 0;
                        }
                        if (legacy > 31){
                            legacy = 0;
                        }
                        break;
                    case 8:
                        strcpy(display_line[3], "    8     ");
                        display_changed = TRUE;
                        initialize_done = 1;
                        strcpy(display_line[4], "  READY!  ");
                        display_changed = TRUE;
                        break;

                    }
                }
                break;
            case 1:
                vv = 0;
                if (iot_TX_buf[response_parse++] == '^' && running == FALSE){
        //            response_parse++;
                    if(iot_TX_buf[response_parse] == '0' && iot_TX_buf[response_parse+1] == '8' && iot_TX_buf[response_parse+2] == '1' && iot_TX_buf[response_parse+3] == '9'){
                        response_parse = response_parse + 4;
                        running = TRUE;
                        distance = iot_TX_buf[response_parse+1]; // Convert to true int
                        char charDist[2];
                        charDist[0] = '#';
                        charDist[1] = (char) distance;
                        dispPrint(charDist, '3');
                        distance = distance - '0';

                        switch (iot_TX_buf[response_parse]){
                        case 'F':
                            run_time_flag = 1;
                            command_op = FORWARDS;
                            run_time = 0;
                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                            break;
                        case 'B':
                            run_time_flag = 1;
                            command_op = BACK;
                            run_time = 0;
                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                            break;
                        case 'R':
                            run_time_flag = 1;
                            command_op = RIGHT;
                            run_time = 0;
                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                            break;
                        case 'L':
                            run_time_flag = 1;
                            command_op = LEFT;
                            run_time = 0;
                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                            break;

                        case 'S':
                            command_op = WAIT;
                           run_time = 0;

                           for (vv = 0; vv < 32; vv++){
                               iot_TX_buf[vv] = 0;
                           }
                            break;

                        }
                    }
                }

                if (response_parse > 31) {
                    response_parse = 0;
                }

                char tempStr[10];
                switch (command_op){
                case WAIT:
                    PWM1_BOTH_OFF();
                    if (run_time < 50){
                        strcpy(display_line[0], "          ");
                        display_changed = TRUE;
        //                strcpy(display_line[1], " RUN CASE ");
        //                strcpy(display_line[0], ssid_display);
        //                strcpy(display_line[0], "ncsu      ");

                        dispPrint(ssid_display, '1');
                        dispPrint("IP address", '2');
                        dispPrint(ip_display1, '3');
                        dispPrint(ip_display2, '4');
        //                strcpy(display_line[2], ip_display1);
        //                strcpy(display_line[3], ip_display2);
                        display_changed = TRUE;
                    }
                    else{
                        strcpy(display_line[1], "WAIT CMPLTE");
                        strcpy(display_line[3], "         W");
                        display_changed = TRUE;
        //                run_time = 0;
                    }
                    break;
                case FORWARDS:
                    PWM1_BOTH_FWD();
                    strcpy(tempStr,ip_display2);
                    strcat(tempStr,"F");
                    dispPrint(tempStr, '4');
                    display_changed = TRUE;

                    if (run_time > distance){ // was originally 10
                        run_time_flag = 0;
                        PWM1_BOTH_OFF();
                        run_time = 0;
                        command_op = WAIT;
                        running = FALSE;
                    }
                    break;
                case BACK:
                    PWM1_BOTH_REV();
                    strcpy(tempStr,ip_display2);
                    strcat(tempStr,"B");
                    dispPrint(tempStr, '4');
                    display_changed = TRUE;
                    if (run_time > distance){ // was originally 5
                        run_time_flag = 0;
                        PWM1_BOTH_OFF();
                        run_time = 0;
                        command_op = WAIT;
                        running = FALSE;
                    }
                    break;
                case RIGHT:
                    PWM1_RIGHT_FWD();
                    strcpy(tempStr,ip_display2);
                    strcat(tempStr,"R");
                    dispPrint(tempStr, '4');
                    display_changed = TRUE;
                    display_changed = TRUE;
                    if (run_time > distance){ // was originally 5
                        run_time_flag = 0;
                        PWM1_BOTH_OFF();
                        run_time = 0;
                        command_op = WAIT;
                        running = FALSE;
                    }
                    break;
                case LEFT:
                    PWM1_LEFT_FWD();
                    strcpy(tempStr,ip_display2);
                    strcat(tempStr,"L");
                    dispPrint(tempStr, '4');
                    display_changed = TRUE;
                    display_changed = TRUE;
                    if (run_time > distance){// was originally 5
                        run_time_flag = 0;
                        PWM1_BOTH_OFF();
                        run_time = 0;
                        command_op = WAIT;
                        running = FALSE;
                    }

                    break;

                default: break;
                }
                break;
            }



        // ----- ----- ----- PROJECT 9 CODE ----- ----- ----- //




//        IOT_Process();

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
        // New way
//        if(iot_rx_wr != iot_rx_rd){
//            if(IOT_Ring_Rx[iot_rx_rd]!= 0x0D && IOT_Ring_Rx[iot_rx_rd+1]!= 0x0A ){
//                // 0x0D = CR, 0x0A = LF
//                process_buffer[pro_inc] = IOT_Ring_Rx[iot_rx_rd];
//                P6OUT ^= GRN_LED;
//            }else{
//                strcpy(display[3],"CPY CMPLTE");
//                P1OUT ^= RED_LED;
////                update_display = TRUE;
//            }
//            pro_inc++;
//            strncpy(display[1],process_buffer,10);
//            IOT_Ring_Rx[iot_rx_rd] = '\n';
//            iot_rx_wr = iot_rx_rd;
////            unsigned int slowDown = 0;
////            while(slowDown < 65000){
////                slowDown++;
////            }
//        }

// // // // // WORKS FOR PROJECT 8 // // // // //
//        if(IOT_Ring_Rx[0] != NULL && switch2_pressed == 0 && switch1_pressed == 0){
//            strcpy(process_buf, IOT_Ring_Rx);
//            strcpy(display_line[0], " Received");
//
//            if (baudLow == 0){
//                strcpy(display_line[2], "BR:115,200");
//            }
//            else {
//                strcpy(display_line[2], "BR:460,800");
//            }
//            strcpy(display_line[3], process_buf);
//
//            unsigned int i;
//            for (i = 0; i < sizeof(IOT_Ring_Rx); i++){
//                IOT_Ring_Rx[i] = '\0';
//                //process_buf[i] = '\0';
//            }
//            iot_rx_wr = BEGINNING;
//            // Added to force Transmit
////            switch1_pressed = TRUE;
//        }
//// // // // // WORKS FOR PROJECT 8 // // // // //
//
//        if (switch1_pressed == 1 && switch2_pressed == 0){
//            //Change display to enter current baud rate
//            strcpy(display_line[0], " Transmit ");
//
//            strcpy(iot_TX_buf, process_buf);
//
//            // Clear second line in case another message is there
//            //strcpy(display_line[1], "          ");
//
//            // Clear the third line
//            strcpy(display_line[3], "          ");
//
//            UCA0IE |= UCTXIE;
//
//            // Move message to second line
//            int i;
//            for (i = 0; i < sizeof(display_line[1]); i++){ // CHANGED strlen(process_buf) to sizeof(display_line[1]
//                display_line[1][i] = process_buf[i];
//            }
//            for (i = strlen(process_buf); i < sizeof(display_line[1]); i++){
//                display_line[1][i] = ' ';
//            }
//
//            display_changed = TRUE;
//
//            pb_index = 0;
//
//
//            for (i = 0; i < sizeof(IOT_Ring_Rx); i++){
//                IOT_Ring_Rx[i] = 0x00;
//                process_buf[i] = 0x00;
//            }
//            //iot_rx_wr = BEGINNING;
//
//            switch1_pressed = 0;
//            switch_debounce_active = 0;
//
//            // enable interrupts again
//            P4IE |= SW1;
//            P2IE |= SW2;
//        }
//
//        if (switch2_pressed == 1 && switch1_pressed == 0){
//            // If baud rate is currently 460800
//            if (baudLow == 1){
//                // Allows case 2 to occur when SW2 is pressed again
//                baudLow = 0;
//                strcpy(display_line[2], "BR:115,200");
//                UCA0BRW = 4;// 115,200 Baud
//                UCA0MCTLW = 0x5551;
//            }
//            // Otherwise the baud rate is currently 115200
//            else{
//                // Allows case 1 to occur when SW2 is pressed again
//                baudLow = 1;
//                strcpy(display_line[2], "BR:460,800");
//                UCA0BRW = 17;// 460,800 Baud
//                UCA0MCTLW = 0x4A00;
//            }
//            display_changed = TRUE;
//
//            switch2_pressed = 0;
//            switch_debounce_active = 0;
//
//            // enable interrupts again
//            P4IE |= SW1;
//            P2IE |= SW2;
//        }
        /// /// /// TEMP PROJECT 8 CODE /// /// ///
    }
    //------------------------------------------------------------------------------
}


/// /// /// TEMP PROJECT 8 CODE /// /// ///
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
/// /// /// TEMP PROJECT 8 CODE /// /// ///






// Project 9 State Machine
void Project9_StateMachine(void){
    strcpy(display_line[0],"Project 9 ");
}

void drunkDriver_StateMachine(void){
    // ----- DEMONSTRATION CODE FOR DRUNK DRIVING ----- //
    display_changed = TRUE;
    int thumbDisp = (ADC_Thumb >> 9);
    if(thumbDisp){
        strcpy(display_line[1],"   FAST   ");
    }else{
        strcpy(display_line[1],"   SLOW   ");
    }

    char thumbDisp1[2];
    sprintf(thumbDisp1, "%d", thumbDisp);

    dispPrint(thumbDisp1,'3');
}

//// Project 8 State Machine
//void Project8_StateMachine(void){
//    if(IOT_Ring_Rx[0] != NULL && switch2_pressed == 0 && switch1_pressed == 0){
//        strcpy(process_buf, IOT_Ring_Rx);
//        strcpy(display_line[0], " Received");
//
//        if (baudLow == 0){
//            strcpy(display_line[2], "BR:115,200");
//        }
//        else {
//            strcpy(display_line[2], "BR:460,800");
//        }
//        strcpy(display_line[3], process_buf);
//
//        unsigned int i;
//        for (i = 0; i < sizeof(IOT_Ring_Rx); i++){
//            IOT_Ring_Rx[i] = '\0';
//            //process_buf[i] = '\0';
//        }
//        iot_rx_wr = BEGINNING;
//    }
//
//    if (switch1_pressed == 1 && switch2_pressed == 0){
//        //Change display to enter current baud rate
//        strcpy(display_line[0], " Transmit ");
//
//        strcpy(iot_TX_buf, process_buf);
//
//        // Clear second line in case another message is there
//        //strcpy(display_line[1], "          ");
//
//        // Clear the third line
//        strcpy(display_line[3], "          ");
//
//        UCA0IE |= UCTXIE;
//
//        // Move message to second line
//        int i;
//        for (i = 0; i < strlen(process_buf); i++){
//            display_line[1][i] = process_buf[i];
//        }
//        for (i = strlen(process_buf); i < sizeof(display_line[1]); i++){
//            display_line[1][i] = ' ';
//        }
//
//        display_changed = TRUE;
//
//        pb_index = 0;
//
//
//        for (i = 0; i < sizeof(IOT_Ring_Rx); i++){
//            IOT_Ring_Rx[i] = 0x00;
//            process_buf[i] = 0x00;
//        }
//        //iot_rx_wr = BEGINNING;
//
//        switch1_pressed = 0;
//        switch_debounce_active = 0;
//
//        // enable interrupts again
//        P4IE |= SW1;
//        P2IE |= SW2;
//    }
//
//    if (switch2_pressed == 1 && switch1_pressed == 0){
//        // If baud rate is currently 460800
//        if (baudLow == 1){
//            // Allows case 2 to occur when SW2 is pressed again
//            baudLow = 0;
//            strcpy(display_line[2], "BR:115,200");
//            UCA0BRW = 4;// 115,200 Baud
//            UCA0MCTLW = 0x5551;
//        }
//        // Otherwise the baud rate is currently 115200
//        else{
//            // Allows case 1 to occur when SW2 is pressed again
//            baudLow = 1;
//            strcpy(display_line[2], "BR:460,800");
//            UCA0BRW = 17;// 460,800 Baud
//            UCA0MCTLW = 0x4A00;
//        }
//        display_changed = TRUE;
//
//        switch2_pressed = 0;
//        switch_debounce_active = 0;
//
//        // enable interrupts again
//        P4IE |= SW1;
//        P2IE |= SW2;
//    }
//}


// // Project 7 State Machine
//void StateMachine(void)
//{
//    switch (state){
//    case IDLE:
//        if(!IR_changed){
//            strcpy(display_line[0], "   IDLE   ");
//        }
//
//        PWM1_BOTH_OFF(); // motorsOFF();
//        display_changed = TRUE;
//        update_display = TRUE;
//        //        ADC_Update = TRUE; = TRUE;
//        // WAIT state called when SW1 is Pressed
//        break;
//
//    case WAIT:
//        strcpy(display_line[0], "   WAIT   ");
//        display_changed = TRUE;
//        update_display = TRUE;
//        //        ADC_Update = TRUE; = TRUE;
//        switch (Time_Sequence)        {   // Time_Sequence-State_Sequence
//        case 10: // Wait for 1 Second
//            PWM1_BOTH_OFF(); // motorsOFF();
//            state = FWD;
//            break;
//        default:
//            break;
//        }
//        break;
//
//        case FWD:
//            PWM1_BOTH_FWD(); // LRFwdON();
//            strcpy(display_line[0], "   FWD    ");
//            display_changed = TRUE;
//            update_display = TRUE;
//            //        ADC_Update = TRUE; = TRUE;
//            if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM)){
//                state = BLACKLINE; // Black Line Detected
//                PWM1_BOTH_OFF(); // motorsOFF();
//            }
//            break;
//
//        case BLACKLINE:
//            PWM1_BOTH_OFF(); // motorsOFF();
//            strcpy(display_line[0], " BLACKLINE");
//            display_changed = TRUE;
//            update_display = TRUE;
//            //        ADC_Update = TRUE; = TRUE;
//            Time_Sequence = 0; //        State_Sequence = Time_Sequence;
//            state = WAIT2;
//            break;
//
//        case WAIT2:
//            PWM1_BOTH_OFF(); // motorsOFF();
//            if(Time_Sequence < 20){
//                strcpy(display_line[0], "          ");
//                strcpy(display_line[1], "Black Line");
//                strcpy(display_line[2], " Detected ");
//                strcpy(display_line[3], "          ");
//            }
//            display_changed = TRUE;
//            update_display = TRUE;
//            switch (Time_Sequence){ // Time_Sequence-State_Sequence
//            case 20: // Wait for 3 Second
//                strcpy(display_line[0], "          ");
//                strcpy(display_line[1], "          ");
//                strcpy(display_line[2], "          ");
//                strcpy(display_line[3], "          ");
//                display_changed = TRUE;
//                update_display = TRUE;
//                PWM1_BOTH_FWD();
//                if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM)){
//                    Time_Sequence = 0;
//                    state = TURNL; // Black Line Detected
//                    PWM1_RIGHT_OFF(); // motorsOFF();
//                }
//                state = TURNL; // Black Line Detected
//                break;
//            default:
//                break;
//            }
//
//            break;
//
//        case TURNL:
//            PWM1_LEFT_FWD(); // LeftFwdON();
//            strcpy(display_line[0], " TURN LEFT");
//            display_changed = TRUE;
//            update_display = TRUE;
//            //        ADC_Update = TRUE; = TRUE;
//            if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM))
//            {
//                state = LINE1; // Black Line Detected
//                PWM1_BOTH_OFF(); // motorsOFF();
//                Time_Sequence = 0;
//            }
//            break;
//        case LINE1:
//            strcpy(display_line[0], "   LINE1  ");
//            display_changed = TRUE;
//            update_display = TRUE;
//            //        ADC_Update = TRUE; = TRUE;
//            if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM)
//                    && (Time_Sequence >= 10))
//            {
//                state = DONE; // Black Line Detected
//                Time_Sequence = 0; //  State_Sequence = Time_Sequence;
//                PWM1_BOTH_OFF(); // motorsOFF();
//            }
//        case DONE:
//            strcpy(display_line[0], "   DONE   ");
//            display_changed = TRUE;
//            update_display = TRUE;
//            if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//                // Currently on Black Line
//                PWM1_RIGHT_FWD(); //Get off Black Line
//            }else{
//                PWM1_BOTH_OFF();
//                state = RIGHT_SIDE; // Car found White on Left side of Line
//                //                    Time_Sequence = 0;
//            }
//            break;
//            //            case LEFT_SIDE:
//            //                if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//            //                    // Currently on White
//            //                    strcpy(display_line[0], " LEFT IF  ");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_BOTH_OFF();
//            ////                    PWM1_RIGHT_FWD(); //Go accross black line
//            //                    state = RIGHT_SIDE;
//            ////                    Time_Sequence = 0;
//            //                }else{
//            //                    strcpy(display_line[0], " LEFT_SIDE");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_RIGHT_OFF();
//            //                    PWM1_LEFT_FWD();
//            //                }
//            //            case RIGHT_SIDE:
//            //                if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//            //                    // Currently on White
//            //                    strcpy(display_line[0], " RIGHT IF ");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_BOTH_OFF();
//            ////                    PWM1_LEFT_FWD(); //Go accross black line
//            //                    state = LEFT_SIDE;
//            ////                    Time_Sequence = 0;
//            //                }else{
//            //                    strcpy(display_line[0], "RIGHT_SIDE");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_LEFT_OFF();
//            //                    PWM1_RIGHT_FWD();
//            //                }
//
//
//
//            //            case LEFT_SIDE:
//            //                if ((ADC_Left_Detect >= IR_MAGIC_NUM) && (ADC_Right_Detect >= IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//            //                    // Currently on BLACK
//            //                    strcpy(display_line[0], " LEFT IF  ");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_BOTH_OFF();
//            //                    //                      PWM1_RIGHT_FWD(); //Go accross black line
//            //                    state = MIDDLE_L;
//            //                    Time_Sequence = 0;
//            //                }else{
//            //                    strcpy(display_line[0], " LEFT_SIDE");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_RIGHT_OFF();
//            //                    PWM1_LEFT_FWD();
//            //                }
//            //            case MIDDLE_L:
//            //                if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM)){
//            //                    PWM1_BOTH_OFF(); // motorsOFF();
//            //                    // Car is on White
//            //                    state = RIGHT_SIDE; // Black Line Detected
//            //                }else{// Otherwise, car still moving Left from previous state
//            //                    strcpy(display_line[0], " MIDDLE_L ");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_RIGHT_OFF();
//            //                    PWM1_LEFT_FWD();
//            //                }
//        case RIGHT_SIDE:
//            if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//                strcpy(display_line[0], "RIGHT_SIDE");
//                display_changed = TRUE;
//                update_display = TRUE;
//                PWM1_LEFT_OFF();
//                PWM1_RIGHT_FWD();
//                state = LEFT_SIDE;
//                Time_Sequence = 0;
//            }else{
//                // Currently on Black Line
//                strcpy(display_line[0], " RIGHT_BLK");
//                display_changed = TRUE;
//                update_display = TRUE;
//                PWM1_BOTH_FWD();
//                if(Time_Sequence > 50){
//                    Time_Sequence = 0;
//                    state = LEFT_SIDE;
//                }
//
//            }
//        case LEFT_SIDE:
//            if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM) && (Time_Sequence >= 10)){
//                strcpy(display_line[0], " LEFT_SIDE");
//                display_changed = TRUE;
//                update_display = TRUE;
//                PWM1_LEFT_OFF();
//                PWM1_RIGHT_FWD();
//                state = RIGHT_SIDE;
//                Time_Sequence = 0;
//            }else{
//                // Currently on Black Line
//                strcpy(display_line[0], " LEFT_BLK ");
//                display_changed = TRUE;
//                update_display = TRUE;
//                //                    PWM1_RIGHT_OFF();
//                PWM1_BOTH_FWD();
//                //                    PWM1_LEFT_FWD(); //Go accross black line
//                //                    state = LEFT_SIDE;
//                if(Time_Sequence > 50){
//                    Time_Sequence = 0;
//                    state = RIGHT_SIDE;
//                }
//            }
//            //            case MIDDLE_R:
//            //                if ((ADC_Left_Detect < IR_MAGIC_NUM) && (ADC_Right_Detect < IR_MAGIC_NUM)){
//            //                    PWM1_BOTH_OFF(); // motorsOFF();
//            //                    // Car is on White
//            //                    state = LEFT_SIDE; // Black Line Detected
//            //                }else{// Otherwise, car still moving Left from previous state
//            //                    strcpy(display_line[0], " MIDDLE_R ");
//            //                    display_changed = TRUE;
//            //                    update_display = TRUE;
//            //                    PWM1_LEFT_OFF();
//            //                    PWM1_RIGHT_FWD();
//            //                }
//        default:
//            break;
//    }
//}

// Project 5
//void StateMachine(void){
//    if(activateSM){
//        switch(Time_Sequence){
//        case 10:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], " Forward  ");
//            strcpy(display_line[2], "   1 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            LRFwdON();
//            break;
//        case 15:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "    Wait  ");
//            strcpy(display_line[2], "   1 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            LRFwdOFF();
//            motorsOFF();
//            break;
//        case 20:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], " Reverse  ");
//            strcpy(display_line[2], "   2 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            LRRevON();
//            break;
//        case 30:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "    Wait  ");
//            strcpy(display_line[2], "   1 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            LRRevOFF();
//            motorsOFF();
//            break;
//        case 35:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], " Forward  ");
//            strcpy(display_line[2], "   1 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            LRFwdON();
//            break;
//        case 40:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "    Wait  ");
//            strcpy(display_line[2], "   1 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            motorsOFF();
//            break;
//        case 45:
//            strcpy(display_line[0], "Clockwise ");
//            strcpy(display_line[1], "    Spin  ");
//            strcpy(display_line[2], "   3 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            P6OUT  |=  R_FORWARD;
//            P6OUT  |=  L_REVERSE;
//            break;
//        case 60:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "    Wait  ");
//            strcpy(display_line[2], "   2 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            motorsOFF();
//            break;
//        case 70:
//            strcpy(display_line[0], "CounterCW ");
//            strcpy(display_line[1], "    Spin  ");
//            strcpy(display_line[2], "   3 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            P6OUT  |=  L_FORWARD;
//            P6OUT  |=  R_REVERSE;
//            break;
//        case 85:
//            strcpy(display_line[0], "          ");
//            strcpy(display_line[1], "    Wait  ");
//            strcpy(display_line[2], "   2 Sec  ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            motorsOFF();
//            break;
//        case 95:
//            strcpy(display_line[0], "    State ");
//            strcpy(display_line[1], "   Machine");
//            strcpy(display_line[2], " Complete ");
//            strcpy(display_line[3], "          ");
//            display_changed = TRUE;
//            motorsOFF();
//            break;
//        default: break;
//        }
//    }
//}

