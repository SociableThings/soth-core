/*
 * UsartCmdServo.c
 *
 *  Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "UsartCmdServo.h"
#include "UsartCommunication.h"
#include <stdio.h>


void initUsartCmdServo()
{
	// USARTCO, 115.2kbps, -0.08%
	// BSEL:11, BSCALE:-7
	USARTD0_BAUDCTRLA = 11;
	USARTD0_BAUDCTRLB = USART_BSCALE3_bm | USART_BSCALE0_bm;
	
	disableUsartCmdServoRx();
	USARTD0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
}

void enableUsartCmdServoRx()
{
    USARTD0_CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
}

void disableUsartCmdServoRx()
{
    USARTD0_CTRLB = USART_TXEN_bm | USART_CLK2X_bm;
}

uint8_t recieveResponseCmdServo(uint8_t* data)
{
    uint8_t i;
    uint8_t totalLength;
    char dataString[100];
    uint8_t xor = 0;
    
    // Wait until TX buffer empty
    loop_until_bit_is_set(USARTD0.STATUS, USART_DREIF_bp);

    // Wait 50us
    _delay_us(50);
    
    // Enable Rx
    enableUsartCmdServoRx();

    // Wait until recv return packet
    while(1){
        data[0] = getDataCmdServo();
        if(data[0]==CMD_SERVO_RETURN_PACKET_HEADER_L){
            break;
        }
    }

    data[1] = getDataCmdServo();
    data[2] = getDataCmdServo();   // ID
    data[3] = getDataCmdServo();   // Flag
    data[4] = getDataCmdServo();   // Address
    data[5] = getDataCmdServo();   // Length
    data[6] = getDataCmdServo();   // Count

    for(i=0; i<data[5]; i++){
        data[7+i] = getDataCmdServo();
    }

    data[7+data[5]] = getDataCmdServo();

    totalLength = 7+data[5]+1;

    for(i=0; i<totalLength; i++){
        sprintf(dataString, "data[%d]: 0x%X", i, data[i]);
        sendStringToComm(dataString);
    }

    xor = data[2];
    for(i=3; i<totalLength-1; i++){
        xor ^= data[i];
    }

    // Disable Rx
    disableUsartCmdServoRx();

    if(xor==data[totalLength-1]){
        return CMD_SERVO_RECV_STATUS_OK;
    } else {
        return CMD_SERVO_RECV_STATUS_NG;
    }
}

uint8_t getDataCmdServo()
{
    loop_until_bit_is_set(USARTD0.STATUS, USART_RXCIF_bp);
    return USARTD0.DATA;
}

void sendDataToCmdServo(const char c)
{
	loop_until_bit_is_set(USARTD0.STATUS, USART_DREIF_bp);
	USARTD0.DATA = c;
}

void sendPacket(uint8_t id, uint8_t flag, uint8_t address, uint8_t length, uint8_t count, uint8_t* data)
{
	uint8_t i, j;
	uint8_t xor;
	
	// Header
	sendDataToCmdServo(CMD_SERVO_SHORT_PACKET_HEADER_L);
	sendDataToCmdServo(CMD_SERVO_SHORT_PACKET_HEADER_H);
	
	// ID
	sendDataToCmdServo(id);
	xor = id;
	
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

void writeFlashROM(uint8_t id)
{
    sendPacket(id, CMD_SERVO_SHORT_FLAG_WRITE_ROM, 0xFF, 0, 0, NULL);
}

void rebootCmdServo(uint8_t id)
{
    sendPacket(id, CMD_SERVO_SHORT_FLAG_REBOOT, 0xFF, 0, 0, NULL);
}

void changeIdCmdServo(uint8_t fromId, uint8_t toId)
{
    sendPacket(fromId, CMD_SERVO_SHORT_FLAG_NONE, CMD_SERVO_ADDRESS_SERVO_ID, 1, 1, toId);
    _delay_ms(1);
    //writeFlashROM(toId);
    //_delay_us(500);
    //rebootCmdServo(toId);
    //_delay_ms(10);
}

servo_status_t getServoStatus(uint8_t id)
{
    uint8_t data[200];
    servo_status_t status;

    sendPacket(id, CMD_SERVO_SHORT_FLAG_MEMORY_MAP_42_59, 0, 0, 1, NULL);
    recieveResponseCmdServo(data);

    status.position = (int16_t)(data[CMD_SERVO_DATA_INDEX+1] << 8 | data[CMD_SERVO_DATA_INDEX+0]);
    status.time = (uint16_t)(data[CMD_SERVO_DATA_INDEX+3] << 8 | data[CMD_SERVO_DATA_INDEX+2]) * 10;
    status.speed = (uint16_t)(data[CMD_SERVO_DATA_INDEX+5] << 8 | data[CMD_SERVO_DATA_INDEX+4]);
    status.current = (uint16_t)(data[CMD_SERVO_DATA_INDEX+7] << 8 | data[CMD_SERVO_DATA_INDEX+6]);
    status.temperature = (uint16_t)(data[CMD_SERVO_DATA_INDEX+9] << 8 | data[CMD_SERVO_DATA_INDEX+8]);
    status.volts = (uint16_t)(data[CMD_SERVO_DATA_INDEX+11] << 8 | data[CMD_SERVO_DATA_INDEX+10]) * 10;

    char dataString[200];
    sprintf(
    dataString,
    "position: %d, time: %d, speed: %d, current: %d, temp: %d, volts: %d",
    status.position,
    status.time,
    status.speed,
    status.current,
    status.temperature,
    status.volts
    );
    sendStringToComm(dataString);

    return status;
}


