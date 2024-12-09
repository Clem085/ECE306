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
char stop_save_sett[] = "AT+SYSSTORE=0\r\n";
char update_conn_sett[] = "AT+CIPMUX=1\r\n";
char conf_server[] = "AT+CIPSERVER=1,8080\r\n";
char SSID_access[] = "AT+CWJAP?\r\n";
char IP_access[] = "AT+CIFSR\r\n";
char check_okay[] = "AT\r\n";
char pinging1[] = "AT+PING=\"www.google.com\"\r\n";
char pinging2[] = "AT+PING=\"www.whitehouse.gov\"\r\n";
char pin[] = "0819";
char sheet = '0';
unsigned int ir_setting = 0;
unsigned int black_low = 500;
unsigned int black_high = 600;
unsigned int ir_first = 0;

void iot_process(void){
    switch (initialize_done){
    case 0:
        if(iot_on_time < 15){
            iot_on();
        }
        else if(iot_on_time < 30){
            iot_parsing_counter = 1;
        }
        else if(iot_on_time < 50){
            iot_boot_sequence();
        }
        break;
    case 1:
        ping_function();
        iot_parsing_counter = 0;
        if(iot_TX_buf[response_parse++] == '^'){
            if(iot_TX_buf[response_parse] == pin[0] && iot_TX_buf[response_parse + 1] == pin[1] && iot_TX_buf[response_parse + 2] == pin[2] && iot_TX_buf[response_parse + 3] == pin[3]){
                response_parse = response_parse + 4;
                iot_command_parse();
            } // End of PIN If statement
        } // End of if(iot_TX_buf[response_parse++] == '^')

        if(response_parse > 31){
            response_parse = 0;
        }

        char tempStr[10];
        switch (commanding_send){
        case WAIT:
            displayclr = 0;
            motors_off();
            // Unsure why this if statement exists, value always seems to be < 50
            if(run_time < 50){
                dispPrint(ssid_display, '1');
                dispPrint("IP address", '2');
                dispPrint(ip_display1, '3');
                dispPrint(ip_display2, '4');
                display_changed = TRUE;
            }else{
                strcpy(display_line[1], " WAITDONE ");
                strcpy(display_line[3], "         W");
                display_changed = TRUE;
            }

            break;
        case FORWARDS:
            fwd_fast();
            strcpy(tempStr, ip_display2);
            strcat(tempStr, "F");
            dispPrint(tempStr, '4');
            display_changed = TRUE;
            if(run_time > setTime){ // was originally 10
                run_time_flag = 0;
                motors_off();
                run_time = 0;
                commanding_send = WAIT;
                movement = 0;
            }
            break;
        case BACK:
            rev_fast();
            strcpy(tempStr, ip_display2);
            strcat(tempStr, "B");
            dispPrint(tempStr, '4');
            display_changed = TRUE;
            if(run_time > setTime){
                run_time_flag = 0;
                motors_off();
                run_time = 0;
                commanding_send = WAIT;
                movement = 0;
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
            if(run_time >= setTime){
                run_time_flag = 0;
                motors_off();
                run_time = 0;
                commanding_send = WAIT;
                movement = 0;
            }
            break;
        case LEFT:
            spin_left();
            strcpy(tempStr, ip_display2);
            strcat(tempStr, "L");
            dispPrint(tempStr, '4');
            display_changed = TRUE;
            if(run_time >= setTime){
                run_time_flag = 0;
                motors_off();
                run_time = 0;
                commanding_send = WAIT;
                movement = 0;
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
        case ARRIVED:
            // make the display cleared
            strcpy(display_line[0], "          ");
            display_changed = TRUE;
            Display_complete();
            if(run_time >= setTime)
            {
                run_time_flag = 0;
                motors_off();
                run_time = 0;
                commanding_send = WAIT;
                movement = 0;
            }

            break;
        case IRCONF:
            if(ir_first)
            {
                ir_conf_display();
                ir_first = FALSE;
                dispPrint("Waiting", '3');
                dispPrint("For Input", '4');
            }
            if(run_time > 1)
            {
                run_time_flag = 0;
                motors_off();
                run_time = 0;
                commanding_send = WAIT;
                movement = 0;
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

            if(run_time > setTime * 10){
                strcpy(display_line[0], "  BL Stop ");
                strcpy(display_line[1], "  306 IS  ");
                strcpy(display_line[2], " COMPLETE ");
                strcpy(display_line[3], "          ");
                run_time_flag = 0;
                motors_off();
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

        default:
            break;
    } // End of switch(initialize_done)
}

void iot_on(void){
    if(iot_on_time <= 10){
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
        initial_process = 1;
    }
}

void iot_boot_sequence(void){
    switch (initial_process){
    case 1:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    1     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_parse++] == '\r'){
            //                        response_parse--;
            if(iot_TX_buf[response_parse - 2] == 'P'){
                strcpy(IOT_Ring_Rx, stop_save_sett);
                UCA1IE |= UCTXIE;
                initial_process = 2;
                response_parse = 0;

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
        if(iot_TX_buf[response_parse++] == '\r'){
            if(iot_TX_buf[response_parse - 2] == 'K'){
                tx_index = 0;
                strcpy(IOT_Ring_Rx, update_conn_sett);
                UCA1IE |= UCTXIE;
                initial_process = 3;
                response_parse = 0;
            }
        }
        if(response_parse > 31){
            response_parse = 0;
        }
        if(tx_index > 31){
            tx_index = 0;
        }
        break;

    case 3:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    3     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_parse++] == '\r'){
            iot_parsing_counter = 0;
            if(iot_TX_buf[response_parse - 10] == 'X'){
                tx_index = 0;
                strcpy(IOT_Ring_Rx, conf_server);
                UCA1IE |= UCTXIE;
                initial_process = 4;
                response_parse = 0;
            }
        }
        if(response_parse > 31){
            response_parse = 0;
        }
        if(tx_index > 31){
            tx_index = 0;
        }
        break;

    case 4:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    4     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_parse++] == '\r'){
            iot_parsing_counter = 0;
            //                if(response_parse-9 < 0){
            if(iot_TX_buf[response_parse + 23] == '8'){
                tx_index = 0;
                strcpy(IOT_Ring_Rx, SSID_access);
                UCA1IE |= UCTXIE;
                initial_process = 5;
                response_parse = 0;
            }
        }
        if(response_parse > 31){
            response_parse = 0;
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
        strcpy(display_line[3], "          ");
        display_changed = TRUE;
        initial_process = 6;
        break;

    case 6:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    6     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_parse++] == '\r'){
            iot_parsing_counter = 0;
            if(iot_TX_buf[response_parse - 9] == ','){
                tx_index = 0;
                strcpy(IOT_Ring_Rx, IP_access);
                UCA1IE |= UCTXIE;
                initial_process = 7;
                response_parse = 0;
                command4_flag = 0;
            }
        }
        if(response_parse > 31){
            response_parse = 0;
        }
        if(tx_index > 31){
            tx_index = 0;
        }
        break;

    case 7:
        iot_parsing_counter = 0;
        strcpy(display_line[3], "    7     ");
        display_changed = TRUE;
        if(iot_TX_buf[response_parse++] == '\r'){
            iot_parsing_counter = 0;
            if(iot_TX_buf[response_parse - 8] == '"'){
                tx_index = 0;
                strcpy(IOT_Ring_Rx, check_okay);
                UCA1IE |= UCTXIE;
                initial_process = 8;
                response_parse = 0;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
            }
        }
        if(response_parse > 31){
            response_parse = 0;
        }
        if(tx_index > 31){
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

void iot_command_parse(void){
    switch (iot_TX_buf[response_parse]){
    case 'F':
        if(!movement){
            setTime = (int)iot_TX_buf[response_parse + 1] - '0';
            run_time_flag = 1;
            commanding_send = FORWARDS;
            run_time = 0;
            for(iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'B':
        if(!movement){
            setTime = (int)iot_TX_buf[response_parse + 1] - '0';
            run_time_flag = 1;
            commanding_send = BACK;
            run_time = 0;
            for(iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'R':
        if(!movement){
            setTime = (int)iot_TX_buf[response_parse + 1] - '0';
            run_time_flag = 1;
            commanding_send = RIGHT;
            run_time = 0;
            for(iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'L':
        if(!movement){
            setTime = (int)iot_TX_buf[response_parse + 1] - '0';
            run_time_flag = 1;
            commanding_send = LEFT;
            run_time = 0;
            for (iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'S':
        if(!movement){
            setTime = (int)iot_TX_buf[response_parse + 1] - '0';
            commanding_send = WAIT;
            run_time = 0;
            for (iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'I':
        if(!movement){
            setTime = iot_TX_buf[response_parse + 1];
            commanding_send = INTERCEPT;
            run_time = 0;
            run_time_flag = 1;
            init_cmd_state = 0;
            state = WAIT;
            for (iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'A':
        if(!movement){
            setTime = 10;
            sheet = iot_TX_buf[response_parse + 1];
            commanding_send = ARRIVED;
            run_time = 0;

            for (iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'C':
        if(!movement){
            setTime = 10;
            ir_setting = (int)iot_TX_buf[response_parse + 1] - '0'; // 0=Clear Config, 1=Set Black Low, 2=Set Black High
            commanding_send = IRCONF;
            run_time = 0;
            ir_first = TRUE;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "          ");
            strcpy(display_line[2], "          ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;

            for (iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 'e':
        // DANGER WARNING
        if(!movement){
            setTime = (int)iot_TX_buf[response_parse + 1] - '0';

            commanding_send = EXIT; // END
            run_time = 0;
            run_time_flag = 1;
            motors_off();

            for (iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    case 's':
        if(!movement){
            setTime = (int)iot_TX_buf[response_parse + 1] - '0'; // 0=Clear Config, 1=Set Black Low, 2=Set Black High
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

            for (iot_parsing_counter = 0; iot_parsing_counter < 32; iot_parsing_counter++){
                iot_TX_buf[iot_parsing_counter] = 0;
            }
        }
        break;

    default:
        break;
    } // END of switch (iot_TX_buf[response_parse])
}
