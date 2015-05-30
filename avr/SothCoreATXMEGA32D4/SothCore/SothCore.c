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
#include "suart.h"
#include <stdio.h>


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
	
	_delay_ms(500);

    //getServoStatus(1);
    //changeIdCmdServo(1, 5);

    _delay_ms(500);
	
	changeTorqueMode(1, CMD_SERVO_TORQUE_MODE_ON);
    //getServoStatus(5);
	
    while(1)
    {
		_delay_ms(2000);
			
        setGoalPosition(1, -160);
		
		onLedStatus();
        _delay_ms(1000);
		offLedStatus();
		_delay_ms(1000);
		
	    setGoalPosition(1, 1100);
		
		//getGPRMCInfoAsJson(jsonString);
		//sendStringToComm(jsonString);
		
		//xmitstr(PSTR("hoge=4\r\n"));
		//xmitf(PSTR("hoge=%c\r\n"), a[0]);
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
	PORTC_OUT = 0b01001011;
	PORTD_DIR = 0b11111011;
	PORTD_OUT = 0b11111011;
			
	// Virtual port setting
	PORTCFG.VPCTRLA = 0x10;  // map PORTA and PORTB to VPORT0 and VPORT1
	PORTCFG.VPCTRLB = 0x32;  // map PORTC and PORTD to VPORT2 and VPORT3
		
	offLedPower();
	offLedStatus();
}
