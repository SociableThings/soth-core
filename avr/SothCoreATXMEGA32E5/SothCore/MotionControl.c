/*
 * MotionControl.c
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <util/delay.h>
#include "UsartCmdServo.h"
#include "TransitionEasing.h"
#include "xprintf.h"

#define WAIT_TIME 350
#define UP_ANGLE 900
#define DOWN_ANGLE 700
#define CLOSE_ANGLE 0
#define RIGHT_YAW 200
#define LEFT_YAW -200
#define CENTER_YAW -30

void setStandaloneMotion()
{
    uint16_t goal;
    for(uint8_t a=0; a<255; a++){
        xprintf("%d\n", cubicEaseInOut(a, 300, 400, 255));
    }
    xprintf("\n");
    for(uint8_t a=0; a<255; a++){
        xprintf("%d\n", cubicEaseInOut(a, 700, -400, 255));
    }

    _delay_ms(500);
 
    changeTorqueOnForAllServos();

    _delay_ms(2000);

    setGoalPosition(5, CENTER_YAW);

    while(1){
        for(uint16_t i=0; i<1000; i++){
            goal = cubicEaseInOut(i, 300, 400, 1000);
            setGoalPositionForAllServos(goal, goal, goal, goal, CENTER_YAW);
            _delay_ms(1);
        }

        for(uint16_t i=0; i<1000; i++){
            goal = cubicEaseInOut(i, 700, -400, 1000);
            setGoalPositionForAllServos(goal, goal, goal, goal, CENTER_YAW);
            _delay_ms(1);
        }
    }

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
