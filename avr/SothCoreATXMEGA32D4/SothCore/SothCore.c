/*
 * SothCore.c
 *
 * Created: 2015/04/05 4:51:29
 *  Author: Hideyuki Takei
 */ 


#define F_CPU 2000000UL

#include <avr/io.h>
#include <util/delay.h>

#define nop() __asm__ __volatile__ ("nop")
#define _sbi(port, bit) (port) |= (1 << (bit))
#define _cbi(port, bit) (port) &= ~(1 << (bit))

#define LED_POWER_PORT PORTC_OUT
#define LED_POWER_PIN 4
#define LED_STATUS_PORT PORTC_OUT
#define LED_STATUS_PIN 5

#define onLedPower() _sbi(LED_POWER_PORT, LED_POWER_PIN)
#define offLedPower() _cbi(LED_POWER_PORT, LED_POWER_PIN)
#define onLedStatus() _sbi(LED_STATUS_PORT, LED_STATUS_PIN)
#define offLedStatus() _cbi(LED_STATUS_PORT, LED_STATUS_PIN)

// Prototype
void initPort();

int main(void)
{
	initPort();
	
	onLedPower();
	
    while(1)
    {
		onLedStatus();
        _delay_ms(500);
		offLedStatus();
		_delay_ms(1000);
    }
}

void initPort()
{
	PORTC_DIR = 0xff;
	
	offLedPower();
	offLedStatus();
}
