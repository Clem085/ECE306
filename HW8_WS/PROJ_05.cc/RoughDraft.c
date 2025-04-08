//The Display functions are controlled by:
// display_changed = 1; and update_display = 1;
//The signal
//update_display = 1;
//is controlled in TimerB0. It is set true every 200msec.
//The lines
//display_changed = 1; and update_display = 1;
//Are both evaluated in Display_ Process.
//The variable
//update_display
//ensures enough time has passed since the last display update. This needs to be at least 200msec. Doing
//this in the TimerB0 ensures that this time requirement is met.
//The variable
//display_changed
//Identifies the display contents has changed and needs to be updated.
//Both of these have to be true.
//void Display_Process(void){
// if(update_display){
// update_display = 0;
// if(display_changed){
// display_changed = 0;
// Display_Update(0,0,0,0);
// }
// }
//}
//
//If they are true, then the display is updated with the function.
// Display_Update(0,0,0,0)
//Configure Timer B0 CCR0 to be a 200msec timer
//Desired Time Between Interrupts =
// = 8,000,000 / 8 / 8 / ( 1 / 200msec )
// = 8,000,000 / 8 / 8 / 5
// = 1,000,000 / 8 / 5
// = 200,000 / 5
// = 25,000
//Example for Timer B0 Capture Compare Register 0 for desired interrupt every 200 msec.
////#define TB0CCR0_INTERVAL (25000) // 8,000,000 / 8 / 8 / (1 / 200msec)
//OR
//Configure Timer B2 CCR0 to be a 200msec timer
//Desired Time Between Interrupts =
// = 32,768 / 1 / 2 / ( 1 / 200msec )
// = 32,768 / 1 / 3 / 5
// = 10,923 / 3 / 5
// = 10,923 / 5
// = 2,185
//Example for Timer B2 Capture Compare Register 0 for desired interrupt every 200 msec.
////#define TB0CCR0_INTERVAL 2,185 ) // 32.768 / 1 / 3 / (1 / 200msec)
//You now own Timers, you will need to maintain a timer that is doing this same function.
//The function call
//Display_Update(0,0,0,0);
//Cannot be executed in an interrupt. It is calling SPI bus functions that take
//a significant amount of time.
