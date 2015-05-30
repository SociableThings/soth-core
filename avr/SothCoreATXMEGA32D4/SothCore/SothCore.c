/*
 * SothCore.c
 *
 *  Author: Hideyuki Takei <hide@soth.io>
 */


// System Clock
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "SothCore.h"
#include "UsartCommunication.h"
#include "UsartGPS.h"
#include "UsartCmdServo.h"
#include "xprintf.h"


// Prototypes
void initClock();
void initPort();


int main(void)
{	
	char jsonString[1000];	
	
	initPort();
	initClock();
	initUsartComm();
	initUsartGPS();
	initUsartCmdServo();
	
	onLedPower();
	
	_delay_ms(1000);

    //changeIdCmdServo(1, 4);
    //reverseDirection(1, CMD_SERVO_REVERSE);
    //setAngleLimit(1, CMD_SERVO_ANGLE_LIMIT_MAX, CMD_SERVO_ANGLE_LIMIT_MIN);

    _delay_ms(500);
	
    changeTorqueOnForAllServos();

    //getServoStatus(5);
	
    while(1)
    {
		_delay_ms(2000);

        setGoalPositionForAllServos(-20, -20, -20, -20, -20);
		
		onLedStatus();
        _delay_ms(1000);
		offLedStatus();
		_delay_ms(1000);

        setGoalPositionForAllServos(20, 20, 20, 20, 20);
		
		//getGPRMCInfoAsJson(jsonString);
		//sendStringToComm(jsonString);
    }
}

void initClock()
{
	// Set int. 2MHz Clock
	OSC.CTRL = OSC_RC2MEN_bm;
	loop_until_bit_is_set(OSC.STATUS, OSC_RC2MRDY_bp);

	// Set clock selector: Int. 2MHz
	CPU_CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC2M_gc;
	
	// Set Pre-scaler DIV2 (Sys CLK: 1MHz)
	CPU_CCP = CCP_IOREG_gc;
	CLK.PSCTRL = CLK_PSADIV_2_gc;
}

void initPort()
{
	// Port setting
	PORTC_DIR = 0b10111011;
	PORTC_OUT = 0b00001011;
	PORTD_DIR = 0b11111011;
	PORTD_OUT = 0b11111011;
			
	// Virtual port setting
	PORTCFG.VPCTRLA = 0x10;  // map PORTA and PORTB to VPORT0 and VPORT1
	PORTCFG.VPCTRLB = 0x32;  // map PORTC and PORTD to VPORT2 and VPORT3
		
	offLedPower();
	offLedStatus();
}
