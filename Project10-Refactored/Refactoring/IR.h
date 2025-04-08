//===========================================================================
//  File Name: IR.h
//  Description: Header file for IR.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Oct 19, 2024
//===========================================================================


#ifndef IR_H_
#define IR_H_


// Global Variables
//// Internally Defined
char IR_status; // Change this turn toggle the IR emitter LED on or off
char IR_changed;
//// Externally Defined



// Macro Definitions
#define IR_MAGIC_NUM (500)



// Function Declarations
void IR_control(void);



#endif /* IR_H_ */
