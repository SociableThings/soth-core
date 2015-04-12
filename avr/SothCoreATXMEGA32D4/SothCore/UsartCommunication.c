/*
 * UsartCommunication.c
 *
 *  Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <avr/io.h>
#include "UsartCommunication.h"

void initUsartCommunication()
{
	// USARTCO, 115.2kbps, -0.08%
	// BSEL:11, BSCALE:-7
	USARTC0_BAUDCTRLA = 11;
	USARTC0_BAUDCTRLB = USART_BSCALE3_bm | USART_BSCALE0_bm;
	
	USARTC0_CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
	USARTC0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
}
