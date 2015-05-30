/*
 * UsartCmdServo.h
 *
 *  Author: Hideyuki Takei <hide@soth.io>
 */ 


#ifndef USARTCMDSERVO_H_
#define USARTCMDSERVO_H_

// Header
#define CMD_SERVO_SHORT_PACKET_HEADER_L     0xFA
#define CMD_SERVO_SHORT_PACKET_HEADER_H     0xAF
#define CMD_SERVO_LONG_PACKET_HEADER_L      0xFA
#define CMD_SERVO_LONG_PACKET_HEADER_H      0xAF
#define CMD_SERVO_RETURN_PACKET_HEADER_L    0xFD
#define CMD_SERVO_RETURN_PACKET_HEADER_H    0xDF

// Short flag
#define CMD_SERVO_SHORT_FLAG_NONE           0b00000000
#define CMD_SERVO_SHORT_FLAG_WRITE_ROM      0b01000000
#define CMD_SERVO_SHORT_FLAG_REBOOT         0b00100000
#define CMD_SERVO_SHORT_FLAG_RESET_VALUE    0b00010000

#define CMD_SERVO_SHORT_FLAG_REQUEST_ACK_NACK   0b00000001
#define CMD_SERVO_SHORT_FLAG_MEMORY_MAP_00_29   0b00000011
#define CMD_SERVO_SHORT_FLAG_MEMORY_MAP_30_59   0b00000101
#define CMD_SERVO_SHORT_FLAG_MEMORY_MAP_20_29   0b00000111
#define CMD_SERVO_SHORT_FLAG_MEMORY_MAP_42_59   0b00001001
#define CMD_SERVO_SHORT_FLAG_MEMORY_MAP_30_41   0b00001011
#define CMD_SERVO_SHORT_FLAG_MEMORY_MAP_60_127  0b00001101
#define CMD_SERVO_SHORT_FLAG_MEMORY_MAP_SELECT  0b00001111

// Return flag
#define CMD_SERVO_RETURN_FLAG_TEMPERATURE_LIMIT_ERROR   0b10000000
#define CMD_SERVO_RETURN_FLAG_TEMPERATURE_LIMIT_ALERM   0b00100000
#define CMD_SERVO_RETURN_FLAG_FLASH_ROM_WRITE_ERROR     0b00001000
#define CMD_SERVO_RETURN_FLAG_RECEIVE_PACKET_ERROR      0b00000010

// Ack/Nack
#define CMD_SERVO_ACK   0x07
#define CMD_SERVO_NACK  0x08

// Immutable zone
#define CMD_SERVO_ADDRESS_MODEL_NUMBER_L    0x00
#define CMD_SERVO_ADDRESS_MODEL_NUMBER_H    0x01
#define CMD_SERVO_ADDRESS_FIRMWARE_VERSION  0x02

// ROM zone
#define CMD_SERVO_ADDRESS_SERVO_ID              0x04
#define CMD_SERVO_ADDRESS_REVERSE               0x05
#define CMD_SERVO_ADDRESS_BAUD_RATE             0x06
#define CMD_SERVO_ADDRESS_RETURN_DELAY          0x07
#define CMD_SERVO_ADDRESS_CW_ANGLE_LIMIT_L      0x08
#define CMD_SERVO_ADDRESS_CW_ANGLE_LIMIT_H      0x09
#define CMD_SERVO_ADDRESS_CCW_ANGLE_LIMIT_L     0x0A
#define CMD_SERVO_ADDRESS_CCW_ANGLE_LIMIT_H     0x0B
#define CMD_SERVO_ADDRESS_TEMPRERATURE_LIMIT_L  0x0E
#define CMD_SERVO_ADDRESS_TEMPRERATURE_LIMIT_H  0x0F
#define CMD_SERVO_ADDRESS_TORQUE_IN_SILENCE     0x16
#define CMD_SERVO_ADDRESS_WARM_UP_TIME          0x17
#define CMD_SERVO_ADDRESS_CW_COMPLIANCE_MARGIN  0x18
#define CMD_SERVO_ADDRESS_CCW_COMPLIANCE_MARGIN 0x19
#define CMD_SERVO_ADDRESS_CW_COMPLIANCE_SLOPE   0x1A
#define CMD_SERVO_ADDRESS_CCW_COMPLIANCE_SLOPE  0x1B
#define CMD_SERVO_ADDRESS_PUNCH_L               0x1C
#define CMD_SERVO_ADDRESS_PUNCH_H               0x1D

// RAM zone
#define CMD_SERVO_ADDRESS_GOAL_POSITION_L       0x1E
#define CMD_SERVO_ADDRESS_GOAL_POSITION_H       0x1F
#define CMD_SERVO_ADDRESS_GOAL_TIME_L           0x20
#define CMD_SERVO_ADDRESS_GOAL_TIME_H           0x21
#define CMD_SERVO_ADDRESS_MAX_TORQUE            0x23
#define CMD_SERVO_ADDRESS_TORQUE_ENABLE         0x24
#define CMD_SERVO_ADDRESS_PRESENT_POSITION_L    0x2A
#define CMD_SERVO_ADDRESS_PRESENT_POSITION_H    0x2B
#define CMD_SERVO_ADDRESS_PRESENT_TIME_L        0x2C
#define CMD_SERVO_ADDRESS_PRESENT_TIME_H        0x2D
#define CMD_SERVO_ADDRESS_PRESENT_SPEED_L       0x2E
#define CMD_SERVO_ADDRESS_PRESENT_SPEED_H       0x2F
#define CMD_SERVO_ADDRESS_PRESENT_CURRENT_L     0x30
#define CMD_SERVO_ADDRESS_PRESENT_CURRENT_H     0x31
#define CMD_SERVO_ADDRESS_PRESENT_TEMPERATURE_L 0x32
#define CMD_SERVO_ADDRESS_PRESENT_TEMPERATURE_H 0x33
#define CMD_SERVO_ADDRESS_PRESENT_VOLTS_L       0x34
#define CMD_SERVO_ADDRESS_PRESENT_VOLTS_H       0x35

// Torque ON
#define CMD_SERVO_TORQUE_MODE_OFF   0x00
#define CMD_SERVO_TORQUE_MODE_ON    0x01
#define CMD_SERVO_TORQUE_MODE_BRAKE 0x02

#define CMD_SERVO_HEADER_PACKET_WAIT_MAX 50
#define CMD_SERVO_RECV_STATUS_OK 0
#define CMD_SERVO_RECV_STATUS_NG 1

#define CMD_SERVO_DATA_INDEX 7

// Servo ID
#define CMD_SERVO_ID_FRONT_RIGHT    1
#define CMD_SERVO_ID_FRONT_LEFT     2
#define CMD_SERVO_ID_BACK_RIGHT     3
#define CMD_SERVO_ID_BACK_LEFT      4
#define CMD_SERVO_ID_YAW            5
#define CMD_SERVO_ID_DEFAULT        1
#define CMD_SERVO_COUNT             5

// Servo limit angle
#define CMD_SERVO_ANGLE_LIMIT_MIN  -160
#define CMD_SERVO_ANGLE_LIMIT_MAX  1100

// Reverse
#define CMD_SERVO_REVERSE       1
#define CMD_SERVO_NOT_REVERSE   0


typedef struct servo_status {
    int16_t position;        // * 1/10 degree
    uint16_t time;           // * 10ms
    uint16_t speed;          // degree/sec
    uint16_t current;        // mA
    uint16_t temperature;    // deg C
    uint16_t volts;          // * 10mV
} servo_status_t;


void initUsartCmdServo();
void enableUsartCmdServoRx();
void disableUsartCmdServoRx();

uint8_t getDataCmdServo();
uint8_t recieveResponseCmdServo(uint8_t* data);

void sendDataToCmdServo(const char c);
void sendPacket(uint8_t id, uint8_t flag, uint8_t address, uint8_t length, uint8_t count, uint8_t* data);
void changeTorqueMode(uint8_t id, uint8_t mode);
void setGoalPosition(uint8_t id, int16_t position);
void writeFlashROM(uint8_t id);
void rebootCmdServo(uint8_t id);
void changeIdCmdServo(uint8_t fromId, uint8_t toId);
void reverseDirection(uint8_t id, uint8_t isReverse);
void setAngleLimit(uint8_t id, int16_t cwAngleLimit, int16_t ccwAngleLimit);
void setGoalPositionForAllServos(int16_t position1, int16_t position2, int16_t position3, int16_t position4, int16_t poistion5);
void changeTorqueModeForAllServos(uint8_t mode1, uint8_t mode2, uint8_t mode3, uint8_t mode4, uint8_t mode5);
void changeTorqueOnForAllServos();
void changeTorqueOffForAllServos();

servo_status_t getServoStatus(uint8_t id);


#endif /* USARTCMDSERVO_H_ */