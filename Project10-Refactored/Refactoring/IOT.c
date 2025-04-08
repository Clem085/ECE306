/* Initialize Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : IOT.c
  Description:  This file has code to communicate with the IOT and parse through IOT responses.
  Programmer: Connor Savugot
  Date Created: Nov 27, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */

// #include as of 11-27-24
//// Header Files
#include "ADC.h"
#include "DAC.h"
#include "Display.h"
#include "functions.h"
#include "interrupts.h"
#include "IR.h"
#include "IOT.h"
#include "LCD.h"
#include "macros.h"
#include "menu.h"
#include "motors.h"
#include "msp430.h"
#include "ports.h"
#include "serial.h"
#include "switches.h"
#include "timers.h"
//// Libraries
#include <stdio.h>
#include <string.h>

// Most Global Variables are Stored in IOT.h, but those with an initial Value are stored here.
char IOT_message_1[] = "AT+SYSSTORE=0\r\n";
char IOT_message_2[] = "AT+CIPMUX=1\r\n";
char IOT_message_3[] = "AT+CIPSERVER=1,7777\r\n";
char IOT_message_4[] = "AT+CWJAP?\r\n";
char IOT_message_5[] = "AT+CIFSR\r\n";
char IOT_message_6[] = "AT\r\n";
char IOT_message_7[] = "AT+PING=\"www.google.com\"\r\n";
char IOT_message_8[] = "AT+PING=\"www.whitehouse.gov\"\r\n";
char pin[] = "0819";
char pad_number = '0';
unsigned int ir_setting = 0;
unsigned int black_low = 500;
unsigned int black_high = 600;
unsigned int ir_first = 0;

void clear_display_buffer(void) {
    memset(iot_TX_buf, 0, sizeof(iot_TX_buf));
}

void iot_process(void){
    switch (system_initialization){
    case 0:
        if(iot_boot_time < 15){
            iot_on();
        }
        else if(iot_boot_time < 30){
            iot_parsing_counter = 1;
        }
        else if(iot_boot_time < 50){
            iot_boot_sequence();
        }
        break;
    case 1:
        ping_function();
        iot_parsing_counter = 0;
        if(iot_TX_buf[response_index++] == '^'){
            if(iot_TX_buf[response_index] == pin[0] && iot_TX_buf[response_index + 1] == pin[1] && iot_TX_buf[response_index + 2] == pin[2] && iot_TX_buf[response_index + 3] == pin[3]){
                response_index = response_index + 4;
                iot_command_parse();
            } // End of PIN If statement
        } // End of if(iot_TX_buf[response_index++] == '^')

        if(response_index > 31){
            response_index = 0;
        }

        char tempStr[10];
        switch (command){
        case WAIT:
            display_clear_flag = 0;
            motors_off();
            if(moving < 50){
                dispPrint(ssid_display, '1');
                dispPrint("IP address", '2');
                dispPrint(ip_display1, '3');
                dispPrint(ip_display2, '4');
                display_changed = TRUE;
            }
            break;
        case FORWARDS:
            fwd_fast();
            strcpy(tempStr, ip_display2);
            strcat(tempStr, "F");
            dispPrint(tempStr, '4');
            display_changed = TRUE;
            if(moving > stopMoving){ // was originally 10
                moving_flag = 0;
                motors_off();
                moving = 0;
                command = WAIT;
                movement_flag = 0;
            }
            break;
        case BACK:
            rev_fast();
            strcpy(tempStr, ip_display2);
            strcat(tempStr, "B");
            dispPrint(tempStr, '4');
            display_changed = TRUE;
            if(moving > stopMoving){
                moving_flag = 0;
                motors_off();
                moving = 0;
                command = WAIT;
                movement_flag = 0;
                strcpy(display_line[0], "  BL STOP ");
                strcpy(display_line[1], "D-DAY OVER");
                display_changed = TRUE;
            }
            break;
        case RIGHT:
            spin_right();
            strcpy(tempStr, ip_display2);
            strcat(tempStr, "R");
            dispPrint(tempStr, '4');
            display_changed = TRUE;
            display_changed = TRUE;
            if(moving >= stopMoving){
                moving_flag = 0;
                motors_off();
                moving = 0;
                command = WAIT;
                movement_flag = 0;
            }
            break;
        case LEFT:
            spin_left();
            strcpy(tempStr, ip_display2);
            strcat(tempStr, "L");
            dispPrint(tempStr, '4');
            display_changed = TRUE;
            if(moving >= stopMoving){
                moving_flag = 0;
                motors_off();
                moving = 0;
                command = WAIT;
                movement_flag = 0;
            }
            break;

        case INTERCEPT:
            // Have it transmit the IR VALUES
            HexToBCD(ADC_Left_Detect);
            adc_line(2, 2);

            HexToBCD(ADC_Right_Detect);
            adc_line(3, 2);
            display_changed = TRUE;

            // Initial Movements (Calls Black Line State Machine After Initial Movement is Over)
            initialMovementBL();

            break;
        case PAD_MESSAGE:
            // make the display cleared
            strcpy(display_line[0], "          ");
            display_changed = TRUE;
            print_pad_message();
            if(moving >= stopMoving){
                moving_flag = 0;
                motors_off();
                moving = 0;
                command = WAIT;
                movement_flag = 0;
            }

            break;
        case IRCONF:
            if(ir_first){
                ir_conf_display();
                ir_first = FALSE;
                dispPrint("Waiting", '3');
                dispPrint("For Input", '4');
            }
            if(moving > 1){
                moving_flag = 0;
                motors_off();
                moving = 0;
                command = WAIT;
                movement_flag = 0;
            }
            break;

        case EXIT:
            // NEW DANGER WARNING
            strcpy(display_line[0], "  BL Exit ");
            strcpy(display_line[1], "          ");
            strcpy(display_line[2], "          ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;

            fwd_fast();
            //                strcpy(tempStr,ip_display2);
            //                strcat(tempStr,"e");
            //                dispPrint(tempStr, '4');

            if(moving > stopMoving * 10){
                strcpy(display_line[0], "  BL Stop ");
                strcpy(display_line[1], "  306 IS  ");
                strcpy(display_line[2], " COMPLETE ");
                strcpy(display_line[3], "          ");
                moving_flag = 0;
                motors_off();
                moving = 0;
                command = NONE;
                movement_flag = 0;
            }
            break;

        case SLOWRIGHT:
            strcpy(display_line[0], "PAD_MESSAGE 08");
            display_changed = TRUE;

            arch_movement();
            break;

        default:
            break;
        } // switch (command)
        break;

        default:
            break;
    } // End of switch(system_initialization)
}

void iot_on(void){
    if(iot_boot_time <= 10){
        strcpy(display_line[0], "   IOT    ");
        strcpy(display_line[1], "   OFF    ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;
    }else{
        P3OUT |= IOT_EN;
        strcpy(display_line[0], "   IOT    ");
        strcpy(display_line[1], "   ON     ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;
        system_initialization = 1;
    }
}

void iot_boot_sequence(void){
    switch (system_initialization){
    case 1:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    1     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_index++] == '\r'){
            if(iot_TX_buf[response_index - 2] == 'P'){
                strcpy(IOT_Ring_Rx, IOT_message_1);
                UCA1IE |= UCTXIE;
                system_initialization = 2;
                response_index = 0;

                while (IOT_Ring_Rx[iot_parsing_counter] == 0x00)        {
                    IOT_Ring_Rx[iot_parsing_counter++] = 0;
                }
            }
        }
        break;

    case 2:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    2     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_index++] == '\r'){
            if(iot_TX_buf[response_index - 2] == 'K'){
                tx_index = 0;
                strcpy(IOT_Ring_Rx, IOT_message_2);
                UCA1IE |= UCTXIE;
                system_initialization = 3;
                response_index = 0;
            }
        }
        if(response_index > 31){
            response_index = 0;
        }
        if(tx_index > 31){
            tx_index = 0;
        }
        break;

    case 3:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    3     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_index++] == '\r'){
            iot_parsing_counter = 0;
            if(iot_TX_buf[response_index - 10] == 'X'){
                tx_index = 0;
                strcpy(IOT_Ring_Rx, IOT_message_3);
                UCA1IE |= UCTXIE;
                system_initialization = 4;
                response_index = 0;
            }
        }
        if(response_index > 31){
            response_index = 0;
        }
        if(tx_index > 31){
            tx_index = 0;
        }
        break;

    case 4:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    4     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_index++] == '\r'){
            iot_parsing_counter = 0;
            //                if(response_index-9 < 0){
            if(iot_TX_buf[response_index + 23] == '8'){
                tx_index = 0;
                strcpy(IOT_Ring_Rx, IOT_message_4);
                UCA1IE |= UCTXIE;
                system_initialization = 5;
                response_index = 0;
            }
        }
        if(response_index > 31){
            response_index = 0;
        }
        if(tx_index > 31){
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
        // strcpy(display_line[3], "          ");
        display_changed = TRUE;
        system_initialization = 6;
        break;

    case 6:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    6     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_index++] == '\r'){
            iot_parsing_counter = 0;
            if(iot_TX_buf[response_index - 9] == ','){
                tx_index = 0;
                strcpy(IOT_Ring_Rx, IOT_message_5);
                UCA1IE |= UCTXIE;
                system_initialization = 7;
                response_index = 0;
                command_execution_flag = 0;
            }
        }
        if(response_index > 31){
            response_index = 0;
        }
        if(tx_index > 31){
            tx_index = 0;
        }
        break;

    case 7:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    7     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_index++] == '\r'){
            iot_parsing_counter = 0;
            if(iot_TX_buf[response_index - 8] == '"'){
                tx_index = 0;
                strcpy(IOT_Ring_Rx, IOT_message_6);
                UCA1IE |= UCTXIE;
                system_initialization = 8;
                response_index = 0;
                Clear_Display();
            }
        }
        if(response_index > 31){
            response_index = 0;
        }
        if(tx_index > 31){
            tx_index = 0;
        }
        break;

    case 8:
        strcpy(display_line[3], "    8     ");
        display_changed = TRUE;
        system_initialization = 1;
        strcpy(display_line[4], "  READY!  ");
        display_changed = TRUE;
        break;
    }
}

void iot_command_parse(void){
    switch (iot_TX_buf[response_index]){
    case 'F':
        if(!movement_flag){
            stopMoving = (int)iot_TX_buf[response_index + 1] - '0';
            moving_flag = 1;
            command = FORWARDS;
            moving = 0;
            for(iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'B':
        if(!movement_flag){
            stopMoving = (int)iot_TX_buf[response_index + 1] - '0';
            moving_flag = 1;
            command = BACK;
            moving = 0;
            for(iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'R':
        if(!movement_flag){
            stopMoving = (int)iot_TX_buf[response_index + 1] - '0';
            moving_flag = 1;
            command = RIGHT;
            moving = 0;
            for(iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'L':
        if(!movement_flag){
            stopMoving = (int)iot_TX_buf[response_index + 1] - '0';
            moving_flag = 1;
            command = LEFT;
            moving = 0;
            clear_display_buffer();
        }
        break;

    case 'I':
        if(!movement_flag){
            stopMoving = iot_TX_buf[response_index + 1];
            command = INTERCEPT;
            moving = 0;
            moving_flag = 1;
            init_cmd_state = 0;
            state = WAIT;
            clear_display_buffer();
        }
        break;

    case 'A':
        if(!movement_flag){
            stopMoving = 10;
            pad_number = iot_TX_buf[response_index + 1];
            command = PAD_MESSAGE;
            moving = 0;

            clear_display_buffer();
        }
        break;

    case 'C':
        if(!movement_flag){
            stopMoving = 10;
            ir_setting = (int)iot_TX_buf[response_index + 1] - '0'; // 0=Clear Config, 1=Set Black Low, 2=Set Black High
            command = IRCONF;
            moving = 0;
            ir_first = TRUE;
            Clear_Display();

            clear_display_buffer();
        }
        break;

    case 'e':
        // DANGER WARNING
        if(!movement_flag){
            stopMoving = (int)iot_TX_buf[response_index + 1] - '0';

            command = EXIT; // END
            moving = 0;
            moving_flag = 1;
            motors_off();

            clear_display_buffer();
        }
        break;

    case 's':
        if(!movement_flag){
            stopMoving = (int)iot_TX_buf[response_index + 1] - '0'; // 0=Clear Config, 1=Set Black Low, 2=Set Black High
            command = SLOWRIGHT;
            moving = 0;
            moving_flag = 1;

            // BEFORE ARCH
            motors_off();
            archState = 1;
            arch_counter = 0;

            Clear_Display();

            clear_display_buffer();
        }
        break;

    default:
        break;
    } // END of switch (iot_TX_buf[response_index])
}
