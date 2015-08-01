/*
 * MotionControl.c
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <util/delay.h>
#include "UsartCmdServo.h"

#define WAIT_TIME 350
#define UP_ANGLE 900
#define DOWN_ANGLE 700
#define CLOSE_ANGLE 0
#define RIGHT_YAW 200
#define LEFT_YAW -200
#define CENTER_YAW -30

void setStandaloneMotion(){
     _delay_ms(500);
 
     changeTorqueOnForAllServos();

     _delay_ms(2000);

     setGoalPosition(5, CENTER_YAW);

     _delay_ms(2000);
     setGoalPositionAndTimeForAllServos(CLOSE_ANGLE, CLOSE_ANGLE, CLOSE_ANGLE, CLOSE_ANGLE, CENTER_YAW, 1500);
     _delay_ms(5000);
 
     while(1){
         // move forward
     
         //setGoalPositionForAllServos(-100, -100, -100, -100, CENTER_YAW);
         setGoalPositionAndTimeForAllServos(DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, CENTER_YAW, 1000);

         _delay_ms(2000);
     
         for(int8_t i=0; i<4; i++)
         {
             setGoalPositionAndTimeForAllServos(DOWN_ANGLE, UP_ANGLE, UP_ANGLE, DOWN_ANGLE, CENTER_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(DOWN_ANGLE, UP_ANGLE, UP_ANGLE, DOWN_ANGLE, RIGHT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(UP_ANGLE, UP_ANGLE, UP_ANGLE, UP_ANGLE, RIGHT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(UP_ANGLE, DOWN_ANGLE, DOWN_ANGLE, UP_ANGLE, RIGHT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(UP_ANGLE, DOWN_ANGLE, DOWN_ANGLE, UP_ANGLE, CENTER_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(UP_ANGLE, DOWN_ANGLE, DOWN_ANGLE, UP_ANGLE, LEFT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(UP_ANGLE, UP_ANGLE, UP_ANGLE, UP_ANGLE, LEFT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(DOWN_ANGLE, UP_ANGLE, UP_ANGLE, DOWN_ANGLE, LEFT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);
         }

         //_delay_ms(500);

         // move backward

         setGoalPositionAndTimeForAllServos(DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, CENTER_YAW, 2000);

         _delay_ms(2000);

         setGoalPositionAndTimeForAllServos(CLOSE_ANGLE, CLOSE_ANGLE, CLOSE_ANGLE, CLOSE_ANGLE, CENTER_YAW, 1500);

         _delay_ms(5000);
         setGoalPositionAndTimeForAllServos(DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, CENTER_YAW, 1000);
         _delay_ms(1000);

         for(int8_t i=0; i<4; i++)
         {
             setGoalPositionAndTimeForAllServos(DOWN_ANGLE, UP_ANGLE, UP_ANGLE, DOWN_ANGLE, LEFT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(UP_ANGLE, UP_ANGLE, UP_ANGLE, UP_ANGLE, LEFT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(UP_ANGLE, DOWN_ANGLE, DOWN_ANGLE, UP_ANGLE, LEFT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(UP_ANGLE, DOWN_ANGLE, DOWN_ANGLE, UP_ANGLE, CENTER_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(UP_ANGLE, DOWN_ANGLE, DOWN_ANGLE, UP_ANGLE, RIGHT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(UP_ANGLE, UP_ANGLE, UP_ANGLE, UP_ANGLE, RIGHT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             setGoalPositionAndTimeForAllServos(DOWN_ANGLE, UP_ANGLE, UP_ANGLE, DOWN_ANGLE, RIGHT_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);

             // top
             setGoalPositionAndTimeForAllServos(DOWN_ANGLE, UP_ANGLE, UP_ANGLE, DOWN_ANGLE, CENTER_YAW, WAIT_TIME);

             _delay_ms(WAIT_TIME);
         }

         setGoalPositionAndTimeForAllServos(DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, CENTER_YAW, 1000);

         _delay_ms(2000);
         setGoalPositionAndTimeForAllServos(CLOSE_ANGLE, CLOSE_ANGLE, CLOSE_ANGLE, CLOSE_ANGLE, CENTER_YAW, 1500);
         _delay_ms(5000);
     }
 }
