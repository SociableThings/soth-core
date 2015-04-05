/*
 * SothCore.c
 *
 * Created: 2015/04/05 4:51:29
 *  Author: Hideyuki Takei <takehide22@gmail.com>
 */


// System Clock
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

// Universal macro
#define nop()           __asm__ __volatile__ ("nop")
#define _sbi(port, bit) (port) |= (1 << (bit))
#define _cbi(port, bit) (port) &= ~(1 << (bit))

// LED
#define LED_POWER_PORT  PORTC
#define LED_POWER_PIN   4
#define LED_STATUS_PORT PORTC
#define LED_STATUS_PIN  5

// USART
#define USART_DEBUG_PORT    PORTC
#define USART_DEBUG_RXD_PIN 2
#define USART_DEBUG_TXD_PIN 3

// Drive LED macro
#define onLedPower()   _sbi(LED_POWER_PORT.OUT, LED_POWER_PIN)
#define offLedPower()  _cbi(LED_POWER_PORT.OUT, LED_POWER_PIN)
#define onLedStatus()  _sbi(LED_STATUS_PORT.OUT, LED_STATUS_PIN)
#define offLedStatus() _cbi(LED_STATUS_PORT.OUT, LED_STATUS_PIN)


// Prototypes
void initClock();
void initPort();
void initUsart();


int main(void)
{
	initPort();
	initClock();
	initUsart();
	
	onLedPower();
	
    while(1)
    {
		onLedStatus();
        _delay_ms(500);
		offLedStatus();
		_delay_ms(1000);

		loop_until_bit_is_set(USARTC0.STATUS, USART_DREIF_bp);
		USARTC0.DATA = 'c';
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

void initUsart()
{
	// USARTCO, 115.2kbps, -0.08%
	// BSEL:11, BSCALE:-7
	USARTC0_BAUDCTRLA = 11;
	USARTC0_BAUDCTRLB = USART_BSCALE3_bm | USART_BSCALE0_bm;
	
	USARTC0_CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
	USARTC0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
}
