/*
 * SothCore.c
 *
 * Created: 2015/04/05 4:51:29
 *  Author: Hideyuki Takei <hide@soth.io>
 */


// System Clock
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "SothCore.h"
#include "UsartCommunication.h"
#include "UsartGPS.h"


// Prototypes
void initClock();
void initPort();


int main(void)
{
	initPort();
	initClock();
	initUsartCommunication();
	
	onLedPower();
	
    while(1)
    {
		onLedStatus();
        _delay_ms(500);
		offLedStatus();
		_delay_ms(1000);

		loop_until_bit_is_set(USARTC0.STATUS, USART_DREIF_bp);
		USARTC0.DATA = 'd';
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
	PORTC_DIR = 0b11111011;
	PORTC_OUT = 0b11001011;
	
	offLedPower();
	offLedStatus();
}
