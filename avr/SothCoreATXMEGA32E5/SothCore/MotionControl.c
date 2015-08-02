/*
 * MotionControl.c
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <util/delay.h>
#include "UsartCmdServo.h"
#include "TransitionEasing.h"
#include "xprintf.h"

#define WAIT_TIME 250
#define UP_ANGLE 900
#define DOWN_ANGLE 700
#define CLOSE_ANGLE 0
#define RIGHT_YAW 200
#define LEFT_YAW -250
#define CENTER_YAW -30

// prototypes
void setPositionWithEasing(
    uint16_t currentPosition1, uint16_t goalPosition1,
    uint16_t currentPosition2, uint16_t goalPosition2,
    uint16_t currentPosition3, uint16_t goalPosition3,
    uint16_t currentPosition4, uint16_t goalPosition4,
    uint16_t currentPosition5, uint16_t goalPosition5,
    uint16_t timeMs
);

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

    _delay_ms(2000);
    setGoalPositionForAllServos(CLOSE_ANGLE, CLOSE_ANGLE, CLOSE_ANGLE, CLOSE_ANGLE, CENTER_YAW);
    _delay_ms(5000);
 
    while(1){
        // move forward
     
        //setGoalPositionAndTimeForAllServos(DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, CENTER_YAW, 1000);
        setPositionWithEasing(
            CLOSE_ANGLE, DOWN_ANGLE,
            CLOSE_ANGLE, DOWN_ANGLE,
            CLOSE_ANGLE, DOWN_ANGLE,
            CLOSE_ANGLE, DOWN_ANGLE,
            CENTER_YAW, CENTER_YAW,
            1000
        );

        _delay_ms(2000);

        setPositionWithEasing(
            DOWN_ANGLE, DOWN_ANGLE,
            DOWN_ANGLE, UP_ANGLE,
            DOWN_ANGLE, UP_ANGLE,
            DOWN_ANGLE, DOWN_ANGLE,
            CENTER_YAW, RIGHT_YAW,
            WAIT_TIME
        );
     
        for(int8_t i=0; i<3; i++)
        {
            //setGoalPositionAndTimeForAllServos(UP_ANGLE, UP_ANGLE, UP_ANGLE, UP_ANGLE, RIGHT_YAW, WAIT_TIME);
            setPositionWithEasing(
                DOWN_ANGLE, UP_ANGLE,
                UP_ANGLE, DOWN_ANGLE,
                UP_ANGLE, DOWN_ANGLE,
                DOWN_ANGLE, UP_ANGLE,
                RIGHT_YAW, RIGHT_YAW,
                WAIT_TIME
            );

            //setGoalPositionAndTimeForAllServos(UP_ANGLE, DOWN_ANGLE, DOWN_ANGLE, UP_ANGLE, CENTER_YAW, WAIT_TIME);
            setPositionWithEasing(
                UP_ANGLE, UP_ANGLE,
                DOWN_ANGLE, DOWN_ANGLE,
                DOWN_ANGLE, DOWN_ANGLE,
                UP_ANGLE, UP_ANGLE,
                RIGHT_YAW, LEFT_YAW,
                WAIT_TIME*2
            );

            //setGoalPositionAndTimeForAllServos(UP_ANGLE, UP_ANGLE, UP_ANGLE, UP_ANGLE, LEFT_YAW, WAIT_TIME);
            setPositionWithEasing(
                UP_ANGLE, DOWN_ANGLE,
                DOWN_ANGLE, UP_ANGLE,
                DOWN_ANGLE, UP_ANGLE,
                UP_ANGLE, DOWN_ANGLE,
                LEFT_YAW, LEFT_YAW,
                WAIT_TIME
            );

            //setGoalPositionAndTimeForAllServos(DOWN_ANGLE, UP_ANGLE, UP_ANGLE, DOWN_ANGLE, CENTER_YAW, WAIT_TIME);
            setPositionWithEasing(
                DOWN_ANGLE, DOWN_ANGLE,
                UP_ANGLE, UP_ANGLE,
                UP_ANGLE, UP_ANGLE,
                DOWN_ANGLE, DOWN_ANGLE,
                LEFT_YAW, RIGHT_YAW,
                WAIT_TIME*2
            );
        }

        setPositionWithEasing(
            DOWN_ANGLE, UP_ANGLE,
            UP_ANGLE, UP_ANGLE,
            UP_ANGLE, UP_ANGLE,
            DOWN_ANGLE, UP_ANGLE,
            RIGHT_YAW, CENTER_YAW,
            WAIT_TIME
        );

        setPositionWithEasing(
            UP_ANGLE, CLOSE_ANGLE,
            UP_ANGLE, CLOSE_ANGLE,
            UP_ANGLE, CLOSE_ANGLE,
            UP_ANGLE, CLOSE_ANGLE,
            CENTER_YAW, CENTER_YAW,
            2000
        );

        _delay_ms(5000);

        //_delay_ms(500);

        // move backward

        /*setGoalPositionAndTimeForAllServos(DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, DOWN_ANGLE, CENTER_YAW, 2000);

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
        }*/
    }
}

void setPositionWithEasing(
    uint16_t currentPosition1, uint16_t goalPosition1,
    uint16_t currentPosition2, uint16_t goalPosition2,
    uint16_t currentPosition3, uint16_t goalPosition3,
    uint16_t currentPosition4, uint16_t goalPosition4,
    uint16_t currentPosition5, uint16_t goalPosition5,
    uint16_t timeMs
)
{
    for(uint16_t i=0; i<timeMs/12; i++){
        setGoalPositionForAllServos(
            cubicEaseInOut(i, currentPosition1, (int16_t)goalPosition1-(int16_t)currentPosition1, timeMs/10),
            cubicEaseInOut(i, currentPosition2, (int16_t)goalPosition2-(int16_t)currentPosition2, timeMs/10),
            cubicEaseInOut(i, currentPosition3, (int16_t)goalPosition3-(int16_t)currentPosition3, timeMs/10),
            cubicEaseInOut(i, currentPosition4, (int16_t)goalPosition4-(int16_t)currentPosition4, timeMs/10),
            cubicEaseInOut(i, currentPosition5, (int16_t)goalPosition5-(int16_t)currentPosition5, timeMs/10)
        );
        _delay_ms(10);
    }
}
