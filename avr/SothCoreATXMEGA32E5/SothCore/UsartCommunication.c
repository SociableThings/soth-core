/*
 * UsartCommunication.c
 *
 *  Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "UsartCommunication.h"
#include "xprintf.h"
#include "SothCore.h"
#include "soth.pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "UsartCmdServo.h"

uint8_t commBuffer[256];
uint8_t commBufferIndex;

// prototypes
void decodeProtocolBuffers();

void initUsartComm()
{
    commBufferIndex = 0;

	// USARTCO, 115.2kbps, -0.08%
	// BSEL:11, BSCALE:-7
	USARTD0_BAUDCTRLA = 11;
	USARTD0_BAUDCTRLB = USART_BSCALE3_bm | USART_BSCALE0_bm;
	
    USARTD0_CTRLA = USART_DRIE_bm | USART_RXCINTLVL_HI_gc;
    USARTD0_CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
	USARTD0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;

    // initialization for xprintf
    xdev_out(sendCharacterToComm);

    _delay_ms(200);
    enableResetBluetooth();
    _delay_ms(200);
    disableResetBluetooth();
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

ISR(USARTD0_RXC_vect)
{
    uint8_t data = USARTD0.DATA;

    cli();

    if(data == '\0'){
        decodeProtocolBuffers();

        commBufferIndex = 0;
    }
    else{
        commBuffer[commBufferIndex++] = data;
    }

    sei();
}

void decodeProtocolBuffers()
{
    bool status;
    ServoPosition servoPosition;

    pb_istream_t stream = pb_istream_from_buffer(commBuffer, commBufferIndex);

    status = pb_decode(&stream, ServoPosition_fields, &servoPosition);
    if (status){
        /*xprintf("## fr: %d ", servoPosition.servo_front_right);
        xprintf("fl: %d ", servoPosition.servo_front_left);
        xprintf("br: %d ", servoPosition.servo_back_right);
        xprintf("bl: %d\n", servoPosition.servo_back_left);*/
        
        setGoalPositionForAllServos(
            servoPosition.servo_front_right,
            servoPosition.servo_front_left,
            servoPosition.servo_back_right,
            servoPosition.servo_back_left,
            servoPosition.servo_yaw
        );
    }
}
