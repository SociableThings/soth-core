/*
 * UsartCmdServo.c
 *
 *  Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <avr/io.h>
#include "UsartCmdServo.h"
#include "UsartCommunication.h"
#include <stdio.h>

void initUsartCmdServo()
{
	// USARTCO, 115.2kbps, -0.08%
	// BSEL:11, BSCALE:-7
	USARTD0_BAUDCTRLA = 11;
	USARTD0_BAUDCTRLB = USART_BSCALE3_bm | USART_BSCALE0_bm;
	
	USARTD0_CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
	USARTD0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
}

void sendDataToCmdServo(const char c)
{
	loop_until_bit_is_set(USARTD0.STATUS, USART_DREIF_bp);
	USARTD0.DATA = c;
}

void sendPacket(uint8_t id, uint8_t flag, uint8_t address, uint8_t length, uint8_t count, uint8_t* data)
{
	uint8_t i, j;
	uint8_t xor = 0x00;
	
	// Header
	sendDataToCmdServo(CMD_SERVO_SHORT_PACKET_HEADER_L);
	sendDataToCmdServo(CMD_SERVO_SHORT_PACKET_HEADER_H);
	
	// ID
	sendDataToCmdServo(id);
	xor ^= id;
	
	// Flag
	sendDataToCmdServo(flag);
	xor ^= flag;
	
	// Address
	sendDataToCmdServo(address);
	xor ^= address;
	
	// Length
	sendDataToCmdServo(length);
	xor ^= length;
	
	// Count
	sendDataToCmdServo(count);
	xor ^= count;
	
	// Data
	for(i=0; i<count; i++){
		for(j=0; j<length; j++){
			sendDataToCmdServo(data[i*length + j]);
			xor ^= data[i*length + j];
		}
	}
	
	// Sum
	sendDataToCmdServo(xor);
}

void changeTorqueMode(uint8_t id, uint8_t mode)
{
	uint8_t data[] = {mode};	
	sendPacket(id, CMD_SERVO_SHORT_FLAG_NONE, CMD_SERVO_ADDRESS_TORQUE_ENABLE, 1, 1, data);
}

void setGoalPosition(uint8_t id, int16_t position)
{
    uint8_t data[] = {(uint8_t)(position), (uint8_t)(position>>8)};
    sendPacket(id, CMD_SERVO_SHORT_FLAG_NONE, CMD_SERVO_ADDRESS_GOAL_POSITION_L, 2, 1, data);
}


