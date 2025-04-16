// File Name : led.c
//
// Description: This file contains the Initialization for the LEDs
//
// Created on: Jan, 2025
// Author: Molana Torabi


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "ports.h"

// Function to initialize the LEDs by turning them off
void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}
