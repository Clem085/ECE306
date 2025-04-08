

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
