/* Program Information Header
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- ---- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : switches.c
  Description:  This file contains the code to control the 3 switches on the MSP430 Board
      >>> As of now, Switch 3 (S3) is used to select a shape to draw. Switch 2 (S2) is used to confirm the Shape. Switch 1 (S1) is unused.
  Programmer: Connor Savugot
  Date: Sep 20, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- ---- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 */

select_shape(int presses){
    if(presses > 0){
        // Update LCD
        backlight(1);
    }
}
