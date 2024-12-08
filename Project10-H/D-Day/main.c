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
#include "wheels.h"
#include "Timers.h"
#include  "DAC.h"
#include "switches.h"


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
extern unsigned int okay_to_look_at_switch1;
extern unsigned int sw1_position;
char forward;
extern char transmit_state;
extern unsigned int baud_toggle;
volatile char Rx_display[SMALL_RING_SIZE];

extern unsigned int event;
extern unsigned int state;



//IOT COMMUNICATION TYPE STUFF
volatile char stop_save_sett[] = "AT+SYSSTORE=0\r\n";
volatile char update_conn_sett[] = "AT+CIPMUX=1\r\n";
volatile char conf_server[] = "AT+CIPSERVER=1,8080\r\n";
volatile char SSID_access[] = "AT+CWJAP?\r\n";
volatile char IP_access[] = "AT+CIFSR\r\n";
volatile char check_okay[] = "AT\r\n";
volatile char pinging1[] = "AT+PING=\"www.google.com\"\r\n";
volatile char pinging2[] = "AT+PING=\"www.ncsu.edu\"\r\n";
//char pin[] = "0819";
char pin[] = "3061";


volatile char IOT_Ring_Rx[LARGE_RING_SIZE];
volatile char iot_TX_buf[LARGE_RING_SIZE];
volatile char ssid_display[SSID_SIZE];
volatile unsigned int tx_index;
unsigned int command4_flag;
volatile char ip_display1[SSID_SIZE];
volatile char ip_display2[SSID_SIZE];
extern unsigned int initialize_done;
extern unsigned int run_time;
volatile char commanding_send;
extern unsigned int run_time_flag;

extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int ADC_thumb;
unsigned int on_line;
extern unsigned int transmit_done;
unsigned int clear_display;

extern unsigned int iot_on_time;
unsigned int iot_start_count;

unsigned int response_parse;
unsigned int movement;
int vv;
unsigned int setTime;

char process_buffer[25];
unsigned int displayclr = 0;
unsigned int following = 0;
char sheet = '0';
unsigned int ir_setting = 0;
unsigned int black_low = 500;
unsigned int black_high = 600;
unsigned int ir_first = 0;




unsigned int arch_counter = 0;
unsigned int archNext = FALSE;
unsigned int archState;
unsigned int nextState;
unsigned int motorDrain;
unsigned int init_cmd_state;

extern unsigned int Waitcount;
extern unsigned int FlagWait;


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
    Init_DAC();
    //  enable_interrupts();


    //P2OUT &= ~RESET_LCD;
    // Place the contents of what you want on the display, in between the quotes
    // Limited to 10 characters per line
    /*  strcpy(display_line[0], "   NCSU   ");
  strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  ECE306  ");
  strcpy(display_line[3], "  GP I/O  ");
  display_changed = TRUE;*/
    //  Display_Update(0,0,0,0);

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
    commanding_send = WAIT;
    state = WAIT; // For Black Line

    P2OUT |= IR_LED;

    //------------------------------------------------------------------------------
    // Begining of the "While" Operating System
    //------------------------------------------------------------------------------
    while(ALWAYS) {                      // Can the Operating system run
        //    Carlson_StateMachine();            // Run a Time Based State Machine
        //    Switches_Process();                // Check for switch state change
        Display_Process();                 // Update Display
        //motor_check();
        P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

        vrfyDirection(); // Protects against Magic Smoke


        if(displayclr == 1){
            Clr_Display();
        }

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

            else if (iot_on_time < 30){
                vv = 1;
            }


            else if (iot_on_time < 50){
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
                            tx_index = 0;
                            strcpy(IOT_Ring_Rx, update_conn_sett);
                            UCA1IE |= UCTXIE;
                            initial_process = 3;
                            response_parse = 0;
                        }
                    }
                    if (response_parse > 31){
                        response_parse = 0;
                    }
                    if (tx_index > 31){
                        tx_index = 0;
                    }
                    break;
                case 3:
                    vv= 0;
                    strcpy(display_line[3], "    3     ");
                    display_changed = TRUE;
                    if (iot_TX_buf[response_parse++] == '\r'){
                        vv = 0;
                        if (iot_TX_buf[response_parse-10] == 'X'){
                            tx_index = 0;
                            strcpy(IOT_Ring_Rx, conf_server);
                            UCA1IE |= UCTXIE;
                            initial_process = 4;
                            response_parse = 0;
                        }
                    }
                    if (response_parse > 31){
                        response_parse = 0;
                    }
                    if (tx_index > 31){
                        tx_index = 0;
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
                            tx_index = 0;
                            strcpy(IOT_Ring_Rx, SSID_access);
                            UCA1IE |= UCTXIE;
                            initial_process = 5;
                            response_parse = 0;
                        }
                    }
                    if (response_parse > 31){
                        response_parse = 0;
                    }
                    if (tx_index > 31){
                        tx_index = 0;
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

                    if (iot_TX_buf[response_parse++] == '\r'){
                        vv = 0;
                        if (iot_TX_buf[response_parse-9] == ','){
                            tx_index = 0;
                            strcpy(IOT_Ring_Rx, IP_access);
                            UCA1IE |= UCTXIE;
                            initial_process = 7;
                            response_parse = 0;
                            command4_flag = 0;
                            //                            strcpy(display_line[1], " SSID CAP ");
                            //                            strcpy(display_line[0], "          ");
                            //                            display_changed = TRUE;
                            //                            //                            strcpy(display_line[0], ssid_display);
                            //                            strcpy(display_line[0], "ncsu      ");
                            //                            strcpy(display_line[2], "          ");
                            //                            strcpy(display_line[3], "          ");
                            //                            display_changed = TRUE;
                        }
                    }
                    //            }
                    if (response_parse > 31){
                        response_parse = 0;
                    }
                    if (tx_index > 31){
                        tx_index = 0;
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
                            tx_index = 0;
                            strcpy(IOT_Ring_Rx, check_okay);
                            UCA1IE |= UCTXIE;
                            initial_process = 8;
                            response_parse = 0;
                            //                    command4_flag = 0;
                            strcpy(display_line[0], "          ");
                            strcpy(display_line[1], "          ");
                            strcpy(display_line[2], "          ");
                            strcpy(display_line[3], "          ");
                            display_changed = TRUE;
                        }
                    }
                    if (response_parse > 31){
                        response_parse = 0;
                    }
                    if (tx_index > 31){
                        tx_index = 0;
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
            pingpong();
            vv = 0;
            if (iot_TX_buf[response_parse++] == '^'){
                //            response_parse++;
                if(iot_TX_buf[response_parse] == pin[0] && iot_TX_buf[response_parse+1] == pin[1] && iot_TX_buf[response_parse+2] == pin[2] && iot_TX_buf[response_parse+3] == pin[3]){
                    response_parse = response_parse +4;
                    switch (iot_TX_buf[response_parse]){
                    case 'F':
                        if(!movement){
                            setTime = (int)iot_TX_buf[response_parse+1]-'0';
                            run_time_flag = 1;
                            commanding_send = FORWARDS;
                            run_time = 0;
                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                        }

                        break;
                    case 'B':
                        if(!movement){
                            setTime = (int)iot_TX_buf[response_parse+1]-'0';
                            run_time_flag = 1;
                            commanding_send = BACK;
                            run_time = 0;
                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                        }
                        break;
                    case 'R':
                        if(!movement){
                            setTime = (int)iot_TX_buf[response_parse+1]-'0';
                            run_time_flag = 1;
                            commanding_send = RIGHT;
                            run_time = 0;
                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                        }
                        break;
                    case 'L':
                        if(!movement){
                            setTime = (int)iot_TX_buf[response_parse+1]-'0';
                            run_time_flag = 1;
                            commanding_send = LEFT;
                            run_time = 0;
                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                        }
                        break;

                    case 'S':
                        if(!movement){
                            setTime = (int)iot_TX_buf[response_parse+1]-'0';
                            commanding_send = WAIT;
                            run_time = 0;

                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                        }
                        break;

                    case 'I':
                        if(!movement){
                            setTime = iot_TX_buf[response_parse+1];
                            commanding_send = INTERCEPT;
                            run_time = 0;
                            run_time_flag = 1;
                            init_cmd_state = 0;
                            state = WAIT;
                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                        }
                        break;

                    case 'A':
                        if(!movement){
                            setTime = 10;
                            sheet = iot_TX_buf[response_parse+1];
                            commanding_send = ARRIVED;
                            run_time = 0;

                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                        }
                        break;

                    case 'C':
                        if(!movement){
                            setTime = 10;
                            ir_setting = (int)iot_TX_buf[response_parse+1]-'0'; // 0=Clear Config, 1=Set Black Low, 2=Set Black High
                            commanding_send = IRCONF;
                            run_time = 0;
                            ir_first = TRUE;
                            strcpy(display_line[0], "          ");
                            strcpy(display_line[1], "          ");
                            strcpy(display_line[2], "          ");
                            strcpy(display_line[3], "          ");
                            display_changed = TRUE;

                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                        }
                        break;


                    case 'e':
                        // DANGER WARNING
                        if(!movement){
                            setTime = (int)iot_TX_buf[response_parse+1]-'0';

                            commanding_send = EXIT; // END
                            run_time = 0;
                            run_time_flag = 1;

                            motors_off();


                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                        }

                        break;


                    case 's':
                        if(!movement){
                            setTime = (int)iot_TX_buf[response_parse+1]-'0'; // 0=Clear Config, 1=Set Black Low, 2=Set Black High
                            commanding_send = SLOWRIGHT;
                            run_time = 0;
                            run_time_flag = 1;

                            // BEFORE ARCH
                            motors_off();
                            archState = 1;
                            arch_counter = 0;

                            strcpy(display_line[0], "          ");
                            strcpy(display_line[1], "          ");
                            strcpy(display_line[2], "          ");
                            strcpy(display_line[3], "          ");
                            display_changed = TRUE;

                            for (vv = 0; vv < 32; vv++){
                                iot_TX_buf[vv] = 0;
                            }
                        }
                        break;


                    default: break;
                    } // END of switch (iot_TX_buf[response_parse])


                } // End of PIN If statement
            } // End of if (iot_TX_buf[response_parse++] == '^')

            if (response_parse > 31) {
                response_parse = 0;
            }

            char tempStr[10];
            switch (commanding_send){
            case WAIT:
                displayclr = 0;
                motor_off();
                // Unsure why this if statement exists, value always seems to be < 50
                if (run_time < 50){
                    dispPrint(ssid_display, '1');
                    dispPrint("IP address", '2');
                    dispPrint(ip_display1, '3');
                    dispPrint(ip_display2, '4');

                    display_changed = TRUE;
                }else{                 // Commented out because I don't see the use of it
                    strcpy(display_line[1], " WAITDONE ");
                    strcpy(display_line[3], "         W");
                    display_changed = TRUE;
                    //                run_time = 0;
                }

                break;
            case FORWARDS:
                //                motor_run_forward();
                fwd_fast();
                strcpy(tempStr,ip_display2);
                strcat(tempStr,"F");
                dispPrint(tempStr, '4');
                display_changed = TRUE;
                if (run_time > setTime){ // was originally 10
                    run_time_flag = 0;
                    motor_off();
                    run_time = 0;
                    commanding_send = WAIT;
                    movement = 0;
                }
                break;
            case BACK:
                //                motor_run_backward();
                rev_fast();
                strcpy(tempStr,ip_display2);
                strcat(tempStr,"B");
                dispPrint(tempStr, '4');
                display_changed = TRUE;
                if (run_time > setTime){
                    run_time_flag = 0;
                    motor_off();
                    run_time = 0;
                    commanding_send = WAIT;
                    movement = 0;
                    strcpy(display_line[0],"  BL STOP ");
                    strcpy(display_line[1],"D-DAY OVER");
                    display_changed = TRUE;
                }
                break;
            case RIGHT:
                motor_run_right();
                strcpy(tempStr,ip_display2);
                strcat(tempStr,"R");
                dispPrint(tempStr, '4');
                display_changed = TRUE;
                display_changed = TRUE;
                if (run_time >= setTime){
                    run_time_flag = 0;
                    motor_off();
                    run_time = 0;
                    commanding_send = WAIT;
                    movement = 0;
                }
                break;
            case LEFT:
                motor_run_left();
                strcpy(tempStr,ip_display2);
                strcat(tempStr,"L");
                dispPrint(tempStr, '4');
                display_changed = TRUE;
                if (run_time >= setTime){
                    run_time_flag = 0;
                    motor_off();
                    run_time = 0;
                    commanding_send = WAIT;
                    movement = 0;
                }

                break;

            case INTERCEPT:
                // make the display cleared
                //                displayclr = displayclr +1;

                // Have it transmit the IR VALUES
                HexToBCD(ADC_Left_Detect);
                adc_line(2,2);

                HexToBCD(ADC_Right_Detect);
                adc_line(3,2);
                display_changed = TRUE;


                // Initial Movements (Calls Black Line State Machine After Initial Movement is Over)
                initialMovementBL();
                //                blacklinemachine();

                break;
            case ARRIVED:
                // make the display cleared
                strcpy(display_line[0], "          ");
                display_changed = TRUE;
                Display_complete();
                if (run_time >= setTime){
                    run_time_flag = 0;
                    motor_off();
                    run_time = 0;
                    commanding_send = WAIT;
                    movement = 0;
                }

                break;
            case IRCONF:
                if(ir_first){
                    ir_conf_display();
                    ir_first = FALSE;
                    dispPrint("Waiting",'3');
                    dispPrint("For Input",'4');
                }
                if (run_time > 1){
                    run_time_flag = 0;
                    motor_off();
                    run_time = 0;
                    commanding_send = WAIT;
                    movement = 0;
                }
                break;


            case EXIT:
                // NEW DANGER WARNING
                strcpy(display[0],"  BL Exit ");
                strcpy(display[1],"          ");
                strcpy(display[2],"          ");
                strcpy(display[3],"          ");
                display_changed = TRUE;

                fwd_fast();
//                strcpy(tempStr,ip_display2);
//                strcat(tempStr,"e");
//                dispPrint(tempStr, '4');


                if (run_time > setTime*10){
                    strcpy(display[0],"  BL Stop ");
                    strcpy(display[1],"  306 IS  ");
                    strcpy(display[2]," COMPLETE ");
                    strcpy(display[3],"          ");
                    run_time_flag = 0;
                    motor_off();
                    run_time = 0;
                    commanding_send = NONE;
                    movement = 0;
                }
                break;


            case SLOWRIGHT:
                strcpy(display_line[0], "Arrived 08");
                display_changed = TRUE;

                arch_movement();
                break;
                //            }
                //              TEST INCORRECT???? COmmented out 2 lines
                //                default: break;
                //            }




            default: break;
            } // switch (commanding_send)
            break;


            default: break;
        } // End of switch(initialize_done)



    } // End of while()
} // End of main()
//------------------------------------------------------------------------------





// Aligns text to Middle of Display
void dispPrint(char *line, char lineToUpdate) {
    char tempLine[11]; // Temporary line buffer
    int lineIndex = -1; // Index of the line to be updated

    // Determine which line to update based on the input character (1-4)
    switch (lineToUpdate) {
    case '1':
        lineIndex = 0;
        break;
    case '2':
        lineIndex = 1;
        break;
    case '3':
        lineIndex = 2;
        break;
    case '4':
        lineIndex = 3;
        break;
    default:
        // Handle invalid input
        strcpy(display_line[0], "-Invalid--");
        display_changed = TRUE;
        return;
    }

    // If the provided line is not NULL, update the corresponding display line
    if (line != NULL) {
        int spaces = (10 - strlen(line)) >> 1; // Calculate spaces for centering
        int i;
        // Initialize tempLine with spaces
        for (i = 0; i < 10; i++) {
            tempLine[i] = ' ';
        }
        tempLine[10] = '\0'; // Null-terminate

        // Copy the line string into the center of tempLine
        strncpy(tempLine + spaces, line, strlen(line));

        // Ensure the string is null-terminated
        tempLine[10] = '\0'; // Null-terminate explicitly

        // Copy the temporary line to the corresponding display line
        strcpy(display_line[lineIndex], tempLine);

        // Indicate that the display has changed
        display_changed = TRUE;
    } else {
        // Handle null parameters by displaying an error message on the selected line
        strcpy(display_line[lineIndex], " NULL Line ");
        display_changed = TRUE;
    }
}

void blacklinemachine(void){
    following = 1;
    switch(state){
    case WAIT:
        motors_off();
        strcpy(display_line[0], "   Start  ");
        display_changed = TRUE;
        FlagWait = TRUE;
        if(Waitcount >= PAUSE){
            FlagWait = FALSE;
            Waitcount=0;
            state = START; // ARCH
        }
        //            archState = 1;
        //            arch_counter = 0;
        break;
    case START:
        start_movement();
        break;
    case DETECTED:
        detect_movement();
        break;
    case SPIN:
        spinning_movement();
        break;
    case TRACKWAIT:
        track_wait();
        break;
    case TRACK:
        tracking_movement();
        break;
        //    case END:
        //        end_state();
        //
        //        run_time_flag = 0;
        //        run_time = 0;
        //        commanding_send = WAIT;
        //        movement = 0;
        //
        //        break;
    default: break;
    }
}






void arch_movement(void){
    double sec_fwd1 = 2.4; // Seconds For FWD
    double sec_fwd2 = 2.75; // Seconds For FWD2 // 2.50 WAS SHORT
    double sec_fwd3 = 1.25; // Seconds For FWD3 // was 1.0
    double sec_spin = 0.29; // Seconds For SPIN // was 0.30
    double sec_wait = 1.00; // Seconds for Wait


    switch(archState){
    case 0: // WAIT CASE
        strcpy(display_line[3],"   WAIT   ");
        motors_off();
        if(motorDrain > 20*sec_wait){
            arch_counter = 0;
            motorDrain = 0;
            archState = nextState;
        }
        break;
    case 1: // FORWARD
        strcpy(display_line[3],"FORWARD #1");
        display_changed = TRUE;
        LEFT_FORWARD_SPEED = LSLOWCIRCLE; //LEFTARCHFWD; //
        RIGHT_FORWARD_SPEED = RSLOWCIRCLE; // RIGHTARCHFWD;
        if(arch_counter > 20*sec_fwd1){
            motors_off();
            arch_counter = 0;
            motorDrain = 0;
            archState = 0;
            nextState = 2;
        }
        break;
    case 2: // SPIN 90 CLOCKWISE
        strcpy(display_line[3],"90 SPIN #1");
        display_changed = TRUE;
        LEFT_REVERSE_SPEED = LEFTARCHFWD;
        RIGHT_FORWARD_SPEED = RIGHTARCHFWD;
        if(arch_counter > 20*sec_spin){
            motors_off();
            archState = 0; // Enter Wait Case
            arch_counter = 0;
            motorDrain = 0;
            nextState = 3;
        }
        break;
    case 3: // FORWARD AGAIN
        strcpy(display_line[3],"FORWARD #2");
        display_changed = TRUE;
        LEFT_FORWARD_SPEED = LSLOWCIRCLE; //LEFTARCHFWD;
        RIGHT_FORWARD_SPEED = RSLOWCIRCLE; //RIGHTARCHFWD;
        if(arch_counter > 20*sec_fwd2){
            motors_off();
            archState = 0; // Enter Wait Case
            arch_counter = 0;
            motorDrain = 0;

            nextState = 4;
        }
        break;
    case 4: // SPIN 90 CLOCKWISE AGAIN
        strcpy(display_line[3],"90 SPIN #2");
        display_changed = TRUE;
        LEFT_REVERSE_SPEED = LEFTARCHFWD;
        RIGHT_FORWARD_SPEED = RIGHTARCHFWD;
        if(arch_counter > 20*sec_spin){
            motors_off();
            archState = 0; // Enter Wait Case
            arch_counter = 0;
            motorDrain = 0;

            nextState = 5;
        }
        break;
    case 5: // FORWARD FINAL
        strcpy(display_line[3],"FORWARD #3");
        display_changed = TRUE;
        LEFT_FORWARD_SPEED = LSLOWCIRCLE; //LEFTARCHFWD;
        RIGHT_FORWARD_SPEED = RSLOWCIRCLE; //RIGHTARCHFWD;
        if(arch_counter > 20*sec_fwd3){
            motors_off();
            arch_counter = 0;
            archState = 0;
            state = START;
            dispPrint(" ",'4');
            display_changed = TRUE;

            run_time_flag = 0;
            motor_off();
            run_time = 0;
            commanding_send = WAIT;
            movement = 0;

        }
        break;


    default: break;
    } // END of ARCH switch(archState)
}


void initialMovementBL(void){
    switch(setTime){
    case '0': {
        // NONE
        if(init_cmd_state==0){
            init_cmd_state++;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "          ");
            strcpy(display_line[2], "          ");
            strcpy(display_line[3], "          ");
            motor_off();
            display_changed = TRUE;
            state = WAIT;
        }else{
            setTime = 'I';
        }
        break;
    }
    case '1': {
        // FWD1
        fwd_fast();
        //        slow_forward();
        dispPrint("FWD1: 5", '1');
        display_changed = TRUE;
        if (run_time > FWD1_DURATION){ // Change Hard Coded Value Based on Testing
            // Ensure Black Line still runs fine after this
            run_time_flag = 0;
            motor_off();
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                motor_off();
                display_changed = TRUE;
                state = WAIT;
            }else{
                setTime = 'I';
                run_time = 0;
            }
        }
        break;
    }
    case '2': {
        // FWD2
        fwd_fast();
        //        slow_forward();
        dispPrint("FWD2: 9", '1');
        display_changed = TRUE;
        if (run_time > FWD2_DURATION){ // Change Hard Coded Value Based on Testing
            run_time_flag = 0;
            //            motor_off();

            //            commanding_send = INTERCEPT;
            movement = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                motor_off();
                display_changed = TRUE;
                state = WAIT;
            }else{
                setTime = 'I';
                run_time = 0;
            }
        }
        break;
    }
    case '3': {
        // BACK1
        slow_backward();
        dispPrint("BACK1: 5", '1');
        display_changed = TRUE;
        if (run_time > BACK1_DURATION){ // Change Hard Coded Value Based on Testing
            run_time_flag = 0;
            //            motor_off();
            //            run_time = 0;
            //            commanding_send = WAIT;
            movement = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                motor_off();
                display_changed = TRUE;
                state = WAIT;
            }else{
                setTime = 'I';
                run_time = 0;
            }
        }
        break;
    }
    case '4':{
        // BACK2
        //        slow_backward();
        rev_fast();
        dispPrint("BACK2: 9", '1');
        display_changed = TRUE;
        if (run_time > BACK2_DURATION){ // Change Hard Coded Value Based on Testing
            run_time_flag = 0;
            motor_off();
            //            run_time = 0;
            //            commanding_send = WAIT;
            movement = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                motor_off();
                display_changed = TRUE;
                state = WAIT;
            }else{
                setTime = 'I';
                run_time = 0;
            }
        }
        break;
    }
    case '5':{
        // 180
        spin_left();
        dispPrint("180", '1');
        display_changed = TRUE;
        if (run_time > S180_DURATION){ // Change Hard Coded Value Based on Testing
            run_time_flag = 0;
            motor_off();
            //            run_time = 0;
            //            commanding_send = WAIT;
            movement = 0;
            // STATE MACHINE
            if(init_cmd_state==0){
                init_cmd_state++;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                motor_off();
                display_changed = TRUE;
                state = WAIT;
            }else{
                setTime = 'I';
                run_time = 0;
            }
        }
        break;
    }
    case '6': {
        // LEFT1
        switch(init_cmd_state){
        case 0: {
            spin_left();
            dispPrint("LEFT1 LEFT", '1');
            display_changed = TRUE;
            if (run_time > LEFT1_DURATION_LEFT){ // Change Hard Coded Value Based on Testing
                motor_off();
                movement = 0;
                init_cmd_state++;
                run_time = 0;
                //                run_time_flag = 0;
                //                commanding_send = WAIT;
            }
            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("LEFT1 FWD", '1');
            display_changed = TRUE;
            if (run_time > LEFT1_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                run_time_flag = 0;
                motor_off();
                //                run_time = 0;
                //                commanding_send = WAIT;
                movement = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    strcpy(display_line[0], "          ");
                    strcpy(display_line[1], "          ");
                    strcpy(display_line[2], "          ");
                    strcpy(display_line[3], "          ");
                    motor_off();
                    display_changed = TRUE;
                    state = WAIT;
                }else{
                    setTime = 'I';
                    run_time = 0;
                }
            }
            break;
        }
        default: break;
        }
        break;
    }

    case '7': {
        // LEFT2
        switch(init_cmd_state){
        case 0: {
            spin_left();
            dispPrint("LEFT2 LEFT", '1');
            display_changed = TRUE;
            if (run_time > LEFT2_DURATION_LEFT){ // Change Hard Coded Value Based on Testing
                motor_off();
                movement = 0;
                init_cmd_state++;
                run_time = 0;
                //                run_time_flag = 0;
                //                commanding_send = WAIT;
            }

            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("LEFT2 FWD", '1');
            display_changed = TRUE;
            if (run_time > LEFT2_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                run_time_flag = 0;
                motor_off();
                //                run_time = 0;
                //                    commanding_send = WAIT;
                movement = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    strcpy(display_line[0], "          ");
                    strcpy(display_line[1], "          ");
                    strcpy(display_line[2], "          ");
                    strcpy(display_line[3], "          ");
                    motor_off();
                    display_changed = TRUE;
                    state = WAIT;
                }else{
                    setTime = 'I';
                    run_time = 0;
                }
            }
            break;
        }
        default: break;
        }
        break;
    }


    case '8': {
        // RIGHT1
        switch(init_cmd_state){
        case 0: {
            spin_right();
            dispPrint("RIGHT1 RIGHT", '1');
            display_changed = TRUE;
            if (run_time > RIGHT1_DURATION_RIGHT){ // Change Hard Coded Value Based on Testing
                motor_off();
                movement = 0;
                init_cmd_state++;
                run_time = 0;
                //                run_time_flag = 0;
                //                commanding_send = WAIT;
            }

            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("RIGHT1 FWD", '1');
            display_changed = TRUE;
            if (run_time > RIGHT1_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                run_time_flag = 0;
                motor_off();
                //                run_time = 0;
                //                        commanding_send = WAIT;
                movement = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    strcpy(display_line[0], "          ");
                    strcpy(display_line[1], "          ");
                    strcpy(display_line[2], "          ");
                    strcpy(display_line[3], "          ");
                    motor_off();
                    display_changed = TRUE;
                    state = WAIT;
                }else{
                    setTime = 'I';
                    run_time = 0;
                }
            }
            break;
        }
        default: break;
        }
        break;
    }

    case '9': {
        // RIGHT2
        switch(init_cmd_state){
        case 0: {
            spin_right();
            dispPrint("RIGHT2 RIGHT", '1');
            display_changed = TRUE;
            if (run_time > RIGHT2_DURATION_RIGHT){ // Change Hard Coded Value Based on Testing
                motor_off();
                movement = 0;
                init_cmd_state++;
                run_time = 0;
                //                run_time_flag = 0;
                //                commanding_send = WAIT;
            }
            break;
        }
        case 1: {
            //            slow_forward();
            fwd_fast();
            dispPrint("RIGHT2 FWD", '1');
            display_changed = TRUE;
            if (run_time > RIGHT2_DURATION_FWD){ // Change Hard Coded Value Based on Testing
                run_time_flag = 0;
                motor_off();
                //                run_time = 0;
                //                            commanding_send = WAIT;
                movement = 0;
                // STATE MACHINE
                if(init_cmd_state==0){
                    init_cmd_state++;
                    strcpy(display_line[0], "          ");
                    strcpy(display_line[1], "          ");
                    strcpy(display_line[2], "          ");
                    strcpy(display_line[3], "          ");
                    motor_off();
                    display_changed = TRUE;
                    state = WAIT;
                }else{
                    setTime = 'I';
                    run_time = 0;
                }
            }
            break;
        }
        default: break;
        }
        break;
    }

    case 'I': {
        blacklinemachine();
        break;
    }

    default: break;
    }
}
