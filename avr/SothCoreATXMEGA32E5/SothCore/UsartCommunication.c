/*
 * UsartCommunication.c
 *
 *  Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <avr/io.h>
#include "UsartCommunication.h"
#include "xprintf.h"

void initUsartComm()
{
	// USARTCO, 115.2kbps, -0.08%
	// BSEL:11, BSCALE:-7
	USARTD0_BAUDCTRLA = 11;
	USARTD0_BAUDCTRLB = USART_BSCALE3_bm | USART_BSCALE0_bm;
	
	USARTD0_CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
	USARTD0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;

    // initialization for xprintf
    xdev_out(sendCharacterToComm);
}

void sendCharacterToComm(const char c)
{
	loop_until_bit_is_set(USARTD0.STATUS, USART_DREIF_bp);
	USARTD0.DATA = c;
}

void sendStringToComm(const char *str)
{
    xputs(str);
}

uint16_t recvCharacters(char* buf, uint16_t length)
{
    uint8_t timeout;
    uint16_t i;

    for(i=0; i<length; i++){
        timeout = 1;
        
        for(uint16_t j=0; j<5000; j++){
            if(bit_is_set(USARTD0.STATUS, USART_RXCIF_bp)){
                timeout = 0;
                break;
            }
        }

        if(timeout==1){
            break;
        }
        else{
            buf[i] = USARTD0.DATA;
        }
    }

    return i;
}

