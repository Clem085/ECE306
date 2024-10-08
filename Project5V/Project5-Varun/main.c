//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "strings.h"

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Init_Timer_B0(void);
void Carlson_StateMachine(void);
void Wheels_Stop(void);
void Wheels_Forward(void);
void Wheels_Reverse(void);
void Wheels_clkw(void);
void Wheels_counterclkw(void);
void Run_Straight(void);
void Run_Circle(void);
void Run_Figure_Eight(void);
void Run_Triangle(void);
void motor_run_forward(void);

  // Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern unsigned int test_value;
extern unsigned int tmr_ms_count;
char chosen_direction;
char change;
unsigned int wheel_move;
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;
extern unsigned int cir_count;
extern unsigned int tri_count;
extern unsigned int okay_to_look_at_switch1;
extern unsigned int sw1_position;
char forward;
extern char transmit_state;
extern unsigned int baud_toggle;
volatile char Rx_display[SMALL_RING_SIZE];

extern unsigned int event;
extern unsigned int state;
unsigned int looking;

//IOT COMMUNICATION TYPE STUFF
volatile char stop_save_sett[] = "AT+SYSSTORE=0\r\n";
volatile char update_conn_sett[] = "AT+CIPMUX=1\r\n";
volatile char conf_server[] = "AT+CIPSERVER=1,8080\r\n";
volatile char SSID_access[] = "AT+CWJAP?\r\n";
volatile char IP_access[] = "AT+CIFSR\r\n";
volatile char check_okay[] = "AT\r\n";
volatile char IOT_Ring_Rx[LARGE_RING_SIZE];
volatile char iot_TX_buf[LARGE_RING_SIZE];
volatile char ssid_display[SSID_SIZE];
volatile unsigned int legacy;
unsigned int command4_flag;
volatile char ip_display1[SSID_SIZE];
volatile char ip_display2[SSID_SIZE];
extern unsigned int initialize_done;
extern unsigned int run_time;
volatile char command_op;
extern unsigned int run_time_flag;
//extern unsigned int ip_index;
unsigned int line_flag;
char track_state;
extern unsigned int step_counter;
extern unsigned int step_flag;
unsigned int first_display;
extern unsigned int detect_flag;
unsigned int detect_time;
unsigned int wait_flag;
unsigned int wait_count;
unsigned int turn_flag;
unsigned int turn_count;
unsigned int disp_flag;
unsigned int disp_count;

extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int ADC_thumb;
unsigned int on_line;
extern unsigned int transmit_done;
unsigned int clear_display;

extern unsigned int transmit_count;
extern unsigned int iot_on_time;
unsigned int iot_start_count;

unsigned int response_parse;
int vv;

char process_buffer[25];
//extern char NCSU_str[10] = "NCSU  #1";

//void main(void){
void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();
  Wheels_Stop();
  Init_LCD();                          // Initialize LCD
  Init_ADC();
  Init_Serial_UCA0();
  Init_Serial_UCA1();

  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;


  USCI_A0_transmit();


  transmit_state = WAIT;

  motor_off();
  wheel_move = 0;
  forward = TRUE;

  okay_to_look_at_switch1 = 1;
  sw1_position = 1;

  iot_on_time = 0;
unsigned int initial_process = 0;
  initialize_done = 0;
  command_op = WAIT;

//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF


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
                if (iot_TX_buf[response_parse++] == '\r'){
                    if (iot_TX_buf[response_parse-2] == 'P'){
                        strcpy(IOT_Ring_Rx, stop_save_sett);
                        UCA1IE |= UCTXIE;
                        initial_process = 2;
                        response_parse = 0;
                        while (IOT_Ring_Rx[vv] == 0x00){
                            IOT_Ring_Rx[vv++] = 0;
                        }
                    }
                }
                break;
            case 2:
                vv= 0;
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
                if (iot_TX_buf[response_parse++] == '\r'){
                    vv = 0;
                        if (iot_TX_buf[response_parse + 23] == '8'){
                            legacy = 0;
                            strcpy(IOT_Ring_Rx, SSID_access);
                            UCA1IE |= UCTXIE;
                            initial_process = 5;
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
            case 5:
                // printing the ssid
                strcpy(display_line[1], "          ");
                strcpy(display_line[0], "          ");
                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                initial_process = 6;
                break;
            case 6:
                vv= 0;
                    if (iot_TX_buf[response_parse++] == '\r'){
                        vv = 0;
                        if (iot_TX_buf[response_parse-9] == ','){
                            legacy = 0;
                            strcpy(IOT_Ring_Rx, IP_access);
                            UCA1IE |= UCTXIE;
                            initial_process = 7;
                            response_parse = 0;
                            command4_flag = FALSE;
                            strcpy(display_line[1], " SSID CAP ");
                            strcpy(display_line[0], ssid_display);
                            strcpy(display_line[2], "          ");
                            strcpy(display_line[3], "          ");
                            display_changed = TRUE;
                        }
                    }
                if (response_parse > 31){
                    response_parse = 0;
                }
                if (legacy > 31){
                    legacy = 0;
                }
                break;
            case 7:
                vv= 0;
                if (iot_TX_buf[response_parse++] == '\r'){
                    vv = 0;
                    if (iot_TX_buf[response_parse-8] == '"'){
                        legacy = 0;
                        strcpy(IOT_Ring_Rx, check_okay);
                        UCA1IE |= UCTXIE;
                        initial_process = 8;
                        response_parse = 0;
                        strcpy(display_line[3], " SSID CAP ");
                        strcpy(display_line[0], ssid_display);
                        strcpy(display_line[1], ip_display1);
                        strcpy(display_line[2], ip_display2);
                        display_changed = TRUE;
                    }
                }
                if (response_parse > 31){
                    response_parse = 0;
                }
                if (legacy > 31){
                    legacy = 0;
                }
                break;
            case 8:
                initialize_done = 1;
                strcpy(display_line[3], " SSID CAP ");
                strcpy(display_line[0], ssid_display);
                strcpy(display_line[1], ip_display1);
                strcpy(display_line[2], ip_display2);
                display_changed = TRUE;
                break;

            }
        }
        break;
    case 1:
        vv = 0;
        if (iot_TX_buf[response_parse++] == '^'){
            switch (iot_TX_buf[response_parse]){
            case 'F':
                run_time_flag = TRUE;
                command_op = FORWARDS;
                run_time = 0;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case 'B':
                run_time_flag = TRUE;
                command_op = BACK;
                run_time = 0;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case 'R':
                run_time_flag = TRUE;
                command_op = RIGHT;
                run_time = 0;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case 'L':
                run_time_flag = TRUE;
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
            case '1':
                command_op = WAIT;
                strcpy(display_line[0], "ARRIVED 01");
                strcpy(display_line[1], ip_display1);
                strcpy(display_line[2], ip_display2);
                display_changed = TRUE;
                update_display = TRUE;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case '2':
                command_op = WAIT;
                strcpy(display_line[0], "ARRIVED 02");
                strcpy(display_line[1], ip_display1);
                strcpy(display_line[2], ip_display2);
                display_changed = TRUE;
                update_display = TRUE;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case '3':
                command_op = WAIT;
                strcpy(display_line[0], "ARRIVED 03");
                strcpy(display_line[1], ip_display1);
                strcpy(display_line[2], ip_display2);
                display_changed = TRUE;
                update_display = TRUE;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case '4':
                command_op = WAIT;
                strcpy(display_line[0], "ARRIVED 04");
                strcpy(display_line[1], ip_display1);
                strcpy(display_line[2], ip_display2);
                display_changed = TRUE;
                update_display = TRUE;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case '5':
                command_op = WAIT;
                strcpy(display_line[0], "ARRIVED 05");
                strcpy(display_line[1], ip_display1);
                strcpy(display_line[2], ip_display2);
                display_changed = TRUE;
                update_display = TRUE;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case '6':
                command_op = WAIT;
                strcpy(display_line[0], "ARRIVED 06");
                strcpy(display_line[1], ip_display1);
                strcpy(display_line[2], ip_display2);
                display_changed = TRUE;
                update_display = TRUE;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case '7':
                command_op = WAIT;
                strcpy(display_line[0], "ARRIVED 07");
                strcpy(display_line[1], ip_display1);
                strcpy(display_line[2], ip_display2);
                display_changed = TRUE;
                update_display = TRUE;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case '8':
                command_op = WAIT;
                strcpy(display_line[0], "ARRIVED 08");
                strcpy(display_line[1], ip_display1);
                strcpy(display_line[2], ip_display2);
                display_changed = TRUE;
                update_display = TRUE;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;
            case 'T':
                command_op = TRACKING;
                looking = TRUE;
                track_state = START;
                for (vv = 0; vv < 32; vv++){
                    iot_TX_buf[vv] = 0;
                }
                break;


            }
        }

        if (response_parse > 31) {
            response_parse = 0;
        }


        switch (command_op){
        case WAIT:
            motor_off();
            if (first_display = 0){
                strcpy(display_line[3], " SSID CAP ");
                strcpy(display_line[0], ssid_display);
                strcpy(display_line[1], ip_display1);
                strcpy(display_line[2], ip_display2);
                display_changed = TRUE;
                first_display++;
            }
            run_time = 0;
            break;
        case FORWARDS:
            motor_run_forward();
            strcpy(display_line[3], "         F");
            display_changed = TRUE;
            if (run_time > 50){
                run_time_flag = FALSE;
                motor_off();
                run_time = 0;
                command_op = WAIT;
            }
            break;
        case BACK:
            motor_run_backward();
            strcpy(display_line[3], "         B");
            display_changed = TRUE;
            if (run_time > 30){
                run_time_flag = FALSE;
                motor_off();
                run_time = 0;
                command_op = WAIT;
            }
            break;
        case RIGHT:
            motor_run_right();
            strcpy(display_line[3], "         R");
            display_changed = TRUE;
            if (run_time > 10){
                run_time_flag = FALSE;
                motor_off();
                run_time = 0;
                command_op = WAIT;
            }
            break;
        case LEFT:
            motor_run_left();
            strcpy(display_line[3], "         L");
            display_changed = TRUE;
            if (run_time > 10){
                run_time_flag = FALSE;
                motor_off();
                run_time = 0;
                command_op = WAIT;
            }

            break;
        case TRACKING:
            strcpy(display_line[3],"T         ");
            display_changed = TRUE;
            update_display = TRUE;
            step_flag = FALSE;
            step_counter = 0;
            looking = FALSE;
            line_flag = TRUE;

            switch(track_state){
            case START:
                detect_flag = TRUE;
                if (detect_time > 50){
                strcpy(display_line[0], "BL START  ");
                start_moving();
                detect_flag = FALSE;
                }
                break;
            case DETECT:
                update_display = TRUE;
                wait_flag = TRUE;
                detect_function();
                if (wait_count > 300){
                    track_state = LINE_TURN;
                    wait_flag = FALSE;
                }
                break;
            case LINE_TURN:
                update_display = TRUE;
                turn_flag = TRUE;
                if (turn_count > 400){
                    line_turn();
                    turn_flag = FALSE;
                }
                break;
            case TRACK:
                disp_flag = TRUE;
                if (disp_count < 800){
                    strcpy(display_line[0], "BL TRAVEL ");
                    update_display = TRUE;
                    display_changed = TRUE;
                }
                else{
                    strcpy(display_line[0], "BL CIRCLE ");
                    update_display = TRUE;
                    display_changed = TRUE;
                }
                update_display = TRUE;
                track_line();
                break;

            default: break;
        }
        break;
    }
        break;

  //P1OUT ^= RED_LED;
  }
}
}
//------------------------------------------------------------------------------






