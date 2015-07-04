/*
 * UsartCmdServo.c
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "UsartCmdServo.h"
#include "SothCore.h"
#include "xprintf.h"

void initUsartCmdServo()
{
	// USARTCO, 115.2kbps, -0.08%
	// BSEL:11, BSCALE:-7
	USARTC0_BAUDCTRLA = 11;
	USARTC0_BAUDCTRLB = USART_BSCALE3_bm | USART_BSCALE0_bm;

	disableUsartCmdServoRx();

	USARTC0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
}

void enableUsartCmdServoRx()
{
    USARTC0_CTRLB = USART_ONEWIRE_bm | USART_RXEN_bm | USART_CLK2X_bm;
    _cbi(PORTC_DIR, USART_SERVO_PIN);
}

void disableUsartCmdServoRx()
{
    USARTC0_CTRLB = USART_ONEWIRE_bm | USART_TXEN_bm | USART_CLK2X_bm;
    _sbi(PORTC_DIR, USART_SERVO_PIN);
}

uint8_t recieveResponseCmdServo(uint8_t* data)
{
    uint8_t i;
    uint8_t totalLength;
    uint8_t xor = 0;
    
    // Wait until TX buffer empty
    loop_until_bit_is_set(USARTC0.STATUS, USART_DREIF_bp);

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

    //for(i=0; i<totalLength; i++){
    //    xprintf("data[%d]: 0x%X\r\n", i, data[i]);
    //}

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
    loop_until_bit_is_set(USARTC0.STATUS, USART_RXCIF_bp);
    return USARTC0.DATA;
}

void sendDataToCmdServo(const char c)
{
	loop_until_bit_is_set(USARTC0.STATUS, USART_DREIF_bp);
	USARTC0.DATA = c;

    //xprintf("%X\n", (uint8_t)c);
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
    sendPacket(id, CMD_SERVO_SHORT_FLAG_WRITE_ROM, 0xFF, 0, 0, CMD_SERVO_NO_DATA);
}

void rebootCmdServo(uint8_t id)
{
    sendPacket(id, CMD_SERVO_SHORT_FLAG_REBOOT, 0xFF, 0, 0, CMD_SERVO_NO_DATA);
}

void changeIdCmdServo(uint8_t fromId, uint8_t toId)
{
    uint8_t data[] = {toId};
    sendPacket(fromId, CMD_SERVO_SHORT_FLAG_NONE, CMD_SERVO_ADDRESS_SERVO_ID, 1, 1, data);
    
    _delay_ms(10);
    writeFlashROM(toId);
    _delay_ms(10);
    rebootCmdServo(toId);
    _delay_ms(100);
}

void reverseDirection(uint8_t id, uint8_t isReverse)
{
    uint8_t data[1];

    if(isReverse>0){
        // reverse
        data[0] = 0x01;
    } else {
        // not reverse (default)
        data[0] = 0x00;
    }
    sendPacket(id, CMD_SERVO_SHORT_FLAG_NONE, CMD_SERVO_ADDRESS_REVERSE, 1, 1, data);

    _delay_ms(1);
    writeFlashROM(id);
    _delay_ms(1);
    rebootCmdServo(id);
    _delay_ms(10);
}

void setAngleLimit(uint8_t id, int16_t cwAngleLimit, int16_t ccwAngleLimit)
{
    uint8_t dataCW[] = {(uint8_t)(cwAngleLimit), (uint8_t)(cwAngleLimit>>8)};
    sendPacket(id, CMD_SERVO_SHORT_FLAG_NONE, CMD_SERVO_ADDRESS_CW_ANGLE_LIMIT_L, 2, 1, dataCW);

    _delay_ms(1);

    uint8_t dataCCW[] = {(uint8_t)(ccwAngleLimit), (uint8_t)(ccwAngleLimit>>8)};
    sendPacket(id, CMD_SERVO_SHORT_FLAG_NONE, CMD_SERVO_ADDRESS_CCW_ANGLE_LIMIT_L, 2, 1, dataCCW);

    _delay_ms(1);
    writeFlashROM(id);
    _delay_ms(1);
    rebootCmdServo(id);
    _delay_ms(10);
}

void setGoalPositionForAllServos(int16_t position1, int16_t position2, int16_t position3, int16_t position4, int16_t position5)
{
    uint8_t data[] = {
        (uint8_t)1, (uint8_t)(position1), (uint8_t)(position1>>8),
        (uint8_t)2, (uint8_t)(position2), (uint8_t)(position2>>8),
        (uint8_t)3, (uint8_t)(position3), (uint8_t)(position3>>8),
        (uint8_t)4, (uint8_t)(position4), (uint8_t)(position4>>8),
        (uint8_t)5, (uint8_t)(position5), (uint8_t)(position5>>8)
    };
    // Use long packet
    sendPacket(0, CMD_SERVO_SHORT_FLAG_NONE, CMD_SERVO_ADDRESS_GOAL_POSITION_L, 3, CMD_SERVO_COUNT, data);
}

void changeTorqueModeForAllServos(uint8_t mode1, uint8_t mode2, uint8_t mode3, uint8_t mode4, uint8_t mode5)
{
    uint8_t data[] = {
        (uint8_t)1, (uint8_t)(mode1),
        (uint8_t)2, (uint8_t)(mode2),
        (uint8_t)3, (uint8_t)(mode3),
        (uint8_t)4, (uint8_t)(mode4),
        (uint8_t)5, (uint8_t)(mode5)
    };
    // Use long packet
    sendPacket(0, CMD_SERVO_SHORT_FLAG_NONE, CMD_SERVO_ADDRESS_TORQUE_ENABLE, 2, CMD_SERVO_COUNT, data);
}

void changeTorqueOnForAllServos()
{
    changeTorqueModeForAllServos(CMD_SERVO_TORQUE_MODE_ON, CMD_SERVO_TORQUE_MODE_ON, CMD_SERVO_TORQUE_MODE_ON, CMD_SERVO_TORQUE_MODE_ON, CMD_SERVO_TORQUE_MODE_ON);
}

void changeTorqueOffForAllServos()
{
    changeTorqueModeForAllServos(CMD_SERVO_TORQUE_MODE_OFF, CMD_SERVO_TORQUE_MODE_OFF, CMD_SERVO_TORQUE_MODE_OFF, CMD_SERVO_TORQUE_MODE_OFF, CMD_SERVO_TORQUE_MODE_OFF);
}

servo_status_t getServoStatus(uint8_t id)
{
    uint8_t data[200];
    servo_status_t status;

    sendPacket(id, CMD_SERVO_SHORT_FLAG_MEMORY_MAP_42_59, 0, 0, 1, CMD_SERVO_NO_DATA);
    recieveResponseCmdServo(data);

    status.position = (int16_t)(data[CMD_SERVO_DATA_INDEX+1] << 8 | data[CMD_SERVO_DATA_INDEX+0]);
    status.time = (uint16_t)(data[CMD_SERVO_DATA_INDEX+3] << 8 | data[CMD_SERVO_DATA_INDEX+2]) * 10;
    status.speed = (uint16_t)(data[CMD_SERVO_DATA_INDEX+5] << 8 | data[CMD_SERVO_DATA_INDEX+4]);
    status.current = (uint16_t)(data[CMD_SERVO_DATA_INDEX+7] << 8 | data[CMD_SERVO_DATA_INDEX+6]);
    status.temperature = (uint16_t)(data[CMD_SERVO_DATA_INDEX+9] << 8 | data[CMD_SERVO_DATA_INDEX+8]);
    status.volts = (uint16_t)(data[CMD_SERVO_DATA_INDEX+11] << 8 | data[CMD_SERVO_DATA_INDEX+10]) * 10;

    xprintf(
        "position: %d, time: %d, speed: %d, current: %d, temp: %d, volts: %d\r\n",
        status.position,
        status.time,
        status.speed,
        status.current,
        status.temperature,
        status.volts
    );

    return status;
}


