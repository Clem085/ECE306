//------------------------------------------------------------------------------
//  Description: Lights up red and green LEDs
//
//  Corinne McGuire
//  Date: 9/16 Fall 2024
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

#include "macros.h"
#include "ports.h"
#include "msp430.h"

//------------------------------------------------------------------------------
// Name: Init_LEDs()
// Description: Turns on both LEDs
//------------------------------------------------------------------------------
void Init_LEDs(){
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
}
