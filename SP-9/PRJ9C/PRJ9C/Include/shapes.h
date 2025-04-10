/*
 * shapes.h
 *
 *  Created on: Feb 11, 2025
 *      Author: price
 */

#ifndef INCLUDE_SHAPES_H_
#define INCLUDE_SHAPES_H_


#define NONE ('N')
#define STRAIGHT (0)
#define CIRCLE (1)
#define TRIANGLE (3)
#define FIGURE8 (2)
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')
#define START_SCREEN


#define IDLE ('I')
#define Configure_Wheel_Speeds ('C')
#define Forward_Move_Start ('F')
#define Forward_Adjust ('A')
#define Reverse_Move_Start ('M')
#define Reverse_Adjust ('B')
#define Initiate_Stop ('L')
#define Readjust ('J')
#define Stop ('T')
#define Wait ('Q')

//DONT CHANGE
#define RIGHT_COUNT_TIME_CIRCLE (9)   //
#define LEFT_COUNT_TIME_CIRCLE (1)   //
#define TRAVEL_DISTANCE_CIRCLE (80)
#define WHEEL_COUNT_TIME_CIRCLE (10)

//DONT CHANGE
#define WHEEL_COUNT_TIME_FIGURE8 (80)
#define LEFT_COUNT_TIME_FIGURE8_1 (7)   //
#define RIGHT_COUNT_TIME_FIGURE8_1 (65)//
#define TRAVEL_DISTANCE_FIGURE8_1 (6)

#define RIGHT_COUNT_TIME_FIGURE8_2 (7)   //
#define LEFT_COUNT_TIME_FIGURE8_2 (65)   //
#define TRAVEL_DISTANCE_FIGURE8_2 (7)

#define LEFT_COUNT_TIME_FIGURE8_3 (7)   //
#define RIGHT_COUNT_TIME_FIGURE8_3 (60)//
#define TRAVEL_DISTANCE_FIGURE8_3 (6)

#define RIGHT_COUNT_TIME_FIGURE8_4 (10)   //
#define LEFT_COUNT_TIME_FIGURE8_4 (70)   //
#define TRAVEL_DISTANCE_FIGURE8_4 (7)

#define WHEEL_COUNT_TIME_TRIANGLE (10) //repeat how many times travel distance is
#define RIGHT_COUNT_TIME_TRIANGLE (7)   //
#define LEFT_COUNT_TIME_TRIANGLE (8)  //
#define TRAVEL_DISTANCE_TRIANGLE (7)

#define WHEEL_COUNT_TIME_PIVOT (15) //repeat how many times travel distance is
#define RIGHT_COUNT_TIME_PIVOT (10)   //
#define LEFT_COUNT_TIME_PIVOT (0)  //
#define TRAVEL_DISTANCE_PIVOT (13)

#define WHEEL_COUNT_TIME (10) //repeat how many times travel distance is
#define RIGHT_COUNT_TIME (7)   //
#define LEFT_COUNT_TIME (8)  //
#define TRAVEL_DISTANCE (10)
#define WAITING2START (100)



#endif /* INCLUDE_SHAPES_H_ */
