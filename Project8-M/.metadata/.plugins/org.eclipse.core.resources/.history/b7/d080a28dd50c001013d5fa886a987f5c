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
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "switches.h"
#include "wheels.h"
#include  <stdio.h>



// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);
void Init_DAC(void);

// Global Variables

//Display board
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned char backlight = 0;
extern volatile unsigned char backlight_changed = 0;
extern volatile char one_time;
unsigned int test_value;

// WHEEL INFO
char chosen_direction;
char change;
unsigned int wheel_move;
char forward;
extern volatile unsigned char state;


extern unsigned int Last_Time_Sequence = 0;  // To track changes in Time_Sequence
extern unsigned int cycle_time = 0;          // Controls shape timings
extern unsigned int time_change = 0;         // Flag to detect time sequence change
extern volatile unsigned int Time_Sequence; // Already existing
volatile unsigned char event;  // Event variable from switches.c (e.g., STRAIGHT, CIRCLE)
extern volatile unsigned int event_Counter = 0; //picking the event based on increment


extern unsigned int okay_to_look_at_switch1;
extern unsigned int sw1_position;

// SERIAL COMM

extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int ADC_thumb;
unsigned int on_line;
extern unsigned int transmit_done;
unsigned int clear_display;

extern unsigned int transmit_count;

extern char transmit_state;
extern unsigned int baud_toggle;
volatile char Rx_display[SMALL_RING_SIZE];

char process_buffer[25];
//extern char NCSU_str[10] = "NCSU  #1";




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
    Init_Timers();                       // Initialize Timers
    Init_LCD();                          // Initialize LCD
    Init_ADC();
    Init_Serial_UCA0();
    // Place the contents of what you want on the display, in between the quotes

    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;

    clear_display = 1;
    baud_toggle = 0;

    USCI_A0_transmit();


    transmit_state = WAIT;

//    motor_off();
    wheel_move = 0;
    forward = TRUE;

//    cir_count = 0;
//    tri_count = 0;
    state = IDLE;
    event = IDLE;
    okay_to_look_at_switch1 = 1;
    sw1_position = 1;

    //    backlight_changed = TRUE;
    //    backlight = TRUE;
    //    Backlight_Process();               // Turn Backlight on/off


    P2OUT |= IR_LED;
    //------------------------------------------------------------------------------
    // Beginning of the "While" Operating System
    //------------------------------------------------------------------------------
    while(ALWAYS) {                      // Can the Operating system run

        //      Carlson_StateMachine();            // Run a Time Based State Machine
        Display_Process();                 // Update Display
        Backlight_Process();
        P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

        // Time Sequence Handling
        if(Last_Time_Sequence != Time_Sequence){
            Last_Time_Sequence = Time_Sequence;
            cycle_time++;
            time_change = 1;            // Flag to indicate a time change occurred
        }



        switch(transmit_state){
        case WAIT:
            strcpy(display_line[0], "  WAITING ");
            strcpy(display_line[1], "          ");
            if (!baud_toggle){
                strcpy(display_line[2], "  115200  ");
            }
            else {
                strcpy(display_line[2], "  460800  ");
            }
            strcpy(display_line[3], "          ");
            display_changed = TRUE;
            break;
        case RECEIVE:
            strcpy(display_line[0], "  RECEIVE ");

            if(baud_toggle){
                strcpy(display_line[0], "Carlson   ");
            }else{

               unsigned int k = 0;
               unsigned int i =0;

               while (i < sizeof(Rx_display) && k < sizeof(display_line[3]) - 1) {
                   // Check for "CL/" followed by Return (0x0D)
                   if ( Rx_display[i] == 0x0A || Rx_display[i] == 0x0D) {
                       // Stop processing and skip over "CL/" and Return
                       i += 4;  // Move index past "CL/" and Return
                       break;
                   }

                   // Copy character to display_line[3] and increment indices
                   display_line[3][k++] = Rx_display[i++];
               }

               // Null-terminate the line to ensure it’s a valid string
               display_line[3][k] = '\0';
            }
            display_changed = TRUE;
            break;
        case TRANSMIT:
            strcpy(display_line[0], " TRANSMIT ");
            unsigned int y = 0;
            unsigned int z =0;

            if(baud_toggle){

                Rx_display[0] = "Carlson   ";
                x_display[0]
               x_display[0]
            x_display[0]
     x_display[0]
                                                        x_display[0]
                                                                  x_display[0]
                                                                            x_display[0]
                                                                                      x_display[0]
                                                                                                x_display[0]

                                                                                                          strcpy(display_line[3], "Carlson   ");

            }else{

                while (z < sizeof(Rx_display) && y < sizeof(display_line[3]) - 1) {
                // Check for "CL/" followed by Return (0x0D)
                if (Rx_display[z] == 0x0A || Rx_display[z] == 0x0D) {
                    // Stop processing and skip over "CL/" and Return
                    z += 4;  // Move index past "CL/" and Return
                    break;
                }

                // Copy character to display_line[3] and increment indices
                display_line[3][y++] = Rx_display[z++];
            }

            // Null-terminate the line to ensure it’s a valid string
            display_line[3][y] = '\0';
            }
            display_changed = TRUE;
            if (transmit_count >= 3){
                transmit_state = WAIT;
                transmit_count = 0;
                int ride = 0;
                while (Rx_display[ride] != 0x00){
                    Rx_display[ride++] = 0;
                }
            }



            break;
        default: break;
        }
        //        motorDirec();
        Debounce_State();
        //        StateMachine();

        //        LEFT_FORWARD_SPEED = 12000;


    }





    //------------------------------------------------------------------------------

}
void StateMachine(void){
    switch(state){
    case WAIT:
        Off_Case();
        state = START;
        break;
    case DETECTED:
        detect_movement();
        break;
    case START:
        start_movement();
        break;
    case SPIN:
        spinning_movement();
        break;
    case TRACK:
        tracking_movement();
        break;
    case END:
        end_state();
        break;
    default: break;
    }
}


