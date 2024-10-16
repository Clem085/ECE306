/*
 * ThumbWheel.c
 *
 *  Created on: Oct 14, 2024
 *      Author: jammi
 */

// Includes
// Custom
#include "Display.h"
#include "LCD.h"
#include "functions.h"
#include "macros.h"
#include "motors.h"
#include "msp430.h"
#include "ports.h"
#include "switches.h"
#include "timersB0.h"
#include "ThumbWheel.h"

// Libraries
#include <string.h>
#include <stdio.h>

// Globals
unsigned int thousands;
unsigned int hundreds;
unsigned int tens;
unsigned int ones;
char adc_char[10];

//Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
void HexToBCD(int hex_value){
    int value;
    int i;
    for(i=0; i < 4; i++) {
        adc_char[i] = '0';
    }
    while (hex_value > 999){
        hex_value = hex_value - 1000;
        value = value + 1;
        adc_char[0] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 99){
        hex_value = hex_value - 100;
        value = value + 1;
        adc_char[1] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 9){
        hex_value = hex_value - 10;
        value = value + 1;
        adc_char[2] = 0x30 + value;
    }
    adc_char[3] = 0x30 + hex_value;
}


