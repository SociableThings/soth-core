/*
 * SothCore.c
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */


// System Clock
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>  
#include "SothCore.h"
#include "UsartCommunication.h"
//#include "UsartGPS.h"
//#include "I2C.h"
//#include "UsartCmdServo.h"
#include "xprintf.h"
//#include "pb_decode.h"
//#include "pb_encode.h"
//#include "soth.pb.h"

#define WAIT_TIME 150
#define UP_ANGLE 900
#define DOWN_ANGLE 700
#define RIGHT_YAW 200
#define LEFT_YAW -200
#define CENTER_YAW -30

// Prototypes
void initClock();
void initPort();

int main(void)
{	
	char jsonString[1000];
    uint8_t servoData[200];
	
    // Initialization
	initPort();
	initClock();
	initUsartComm();
	//initUsartGPS();
    //initI2C();
	//initUsartCmdServo();

    // Enable interrupt
    PMIC_CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEX_bm;  // enable high/middle level interrupt executing
    sei();
	
    _delay_ms(200);
    enableResetBluetooth();
	_delay_ms(200);
    disableResetBluetooth();
    //xprintf("$$$");
    _delay_ms(1000);
    //xprintf("SF,1\r\n");
    //xprintf("SN,SothBon\r\n");
    //xprintf("R,1\r\n");

    // LED test
    while(1){
        offLedStatus();
        _delay_ms(1000);
        onLedStatus();
        _delay_ms(1000);
        xprintf("aaa");
        //_delay_ms(1000);
        //xprintf("R,1\r\n");
    }


    // Servo test
    /*
    //changeIdCmdServo(1, 4);
    //reverseDirection(1, CMD_SERVO_REVERSE);
    //setAngleLimit(1, CMD_SERVO_ANGLE_LIMIT_MAX, CMD_SERVO_ANGLE_LIMIT_MIN);

    _delay_ms(500);
	
    changeTorqueOnForAllServos();

    _delay_ms(2000);

    setGoalPosition(5, CENTER_YAW);

    _delay_ms(2000);

    setGoalPositionForAllServos(-100, -100, -100, -100, CENTER_YAW);

    _delay_ms(3000);

    onLedStatus();

    _delay_ms(2000);

    setGoalPositionForAllServos(UP_ANGLE, UP_ANGLE, UP_ANGLE, UP_ANGLE, CENTER_YAW);
    _delay_ms(2000);

    //getServoStatus(5);
	
    
    for(int8_t i=0; i<5; i++)
    {
        setGoalPositionForAllServos(DOWN_ANGLE, UP_ANGLE, UP_ANGLE, DOWN_ANGLE, CENTER_YAW);

		_delay_ms(WAIT_TIME);	    

        setGoalPositionForAllServos(DOWN_ANGLE, UP_ANGLE, UP_ANGLE, DOWN_ANGLE, RIGHT_YAW);

        _delay_ms(WAIT_TIME);

        setGoalPositionForAllServos(UP_ANGLE, UP_ANGLE, UP_ANGLE, UP_ANGLE, RIGHT_YAW);

        _delay_ms(WAIT_TIME);

        setGoalPositionForAllServos(UP_ANGLE, DOWN_ANGLE, DOWN_ANGLE, UP_ANGLE, RIGHT_YAW);

        _delay_ms(WAIT_TIME);

        setGoalPositionForAllServos(UP_ANGLE, DOWN_ANGLE, DOWN_ANGLE, UP_ANGLE, CENTER_YAW);

        _delay_ms(WAIT_TIME);

        setGoalPositionForAllServos(UP_ANGLE, DOWN_ANGLE, DOWN_ANGLE, UP_ANGLE, LEFT_YAW);

        _delay_ms(WAIT_TIME);

        setGoalPositionForAllServos(UP_ANGLE, UP_ANGLE, UP_ANGLE, UP_ANGLE, LEFT_YAW);

        _delay_ms(WAIT_TIME);

        setGoalPositionForAllServos(DOWN_ANGLE, UP_ANGLE, UP_ANGLE, DOWN_ANGLE, LEFT_YAW);

        _delay_ms(WAIT_TIME);
    }*/

    /*
    // GPS test
    while(1)
    {
		onLedStatus();
		_delay_ms(100);
		offLedStatus();
		_delay_ms(100);
        
        getGPRMCInfoAsJson(jsonString);
        sendStringToComm(jsonString);

        _delay_ms(1000);
    }
    */

    /*
    // I2C test
    xprintf("write\n");
    uint8_t data[] = {0x20, 0x90};
    addQueue(0b10111000, 2, data, 0, testI2C);

    _delay_ms(2000);
    
    uint8_t data2[] = {0x28};
    addQueue(0b10111000, 1, data2, 1, testI2C);

    uint8_t data3[] = {0x29};
    addQueue(0b10111000, 1, data3, 1, testI2C);

    uint8_t data4[] = {0x2A};
    addQueue(0b10111000, 1, data4, 1, testI2C);

    while(1){
        _delay_ms(1000);

        onLedStatus();
        _delay_ms(100);
        offLedStatus();
        _delay_ms(100);
    }
    */

    /*
    uint8_t buffer[128];
    size_t message_length;
    bool status;

    RobotInfoRequest request;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    request.robot_id = 10;

    status = pb_encode(&stream, RobotInfoRequest_fields, &request);
    message_length = stream.bytes_written;

    if (!status)
    {
        xprintf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return 1;
    }

    RobotInfoRequest req2;
    pb_istream_t stream2 = pb_istream_from_buffer(buffer, message_length);
    status = pb_decode(&stream2, RobotInfoRequest_fields, &req2);

    if (!status)
    {
        xprintf("Decoding failed: %s\n", PB_GET_ERROR(&stream2));
        return 1;
    }

    onLedStatus();
    while(1){
        uint16_t length = recvCharacters(servoData, 3);
        if(length>0){
            break;
        }
    }
    _delay_ms(1000);

    while(1){
        onLedStatus();
        _delay_ms(500);
        offLedStatus();
        _delay_ms(500);
        xprintf("Your lucky number was %d!\n", req2.robot_id);
    }*/
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
    // PORTA:     11101111
    //            |||||||+- PA0( 6): NC
    //            ||||||+-- PA1( 5): NC
    //            |||||+--- PA2( 4): NC
    //            ||||+---- PA3( 3): NC
    //            |||+----- PA4( 2): Power voltage ADC
    //            ||+------ PA5(31): NC
    //            |+------- PA6(30): NC
    //            +-------- PA7(29): NC
    PORTA_DIR = 0b11101111;
    PORTA_OUT = 0b11101111;

    // PORTC:     11111110
    //            |||||||+- PC0(16): SDA I2C for sensors
    //            ||||||+-- PC1(15): SCL I2C for sensors
    //            |||||+--- PC2(14): Bluetooth /RESET (Active low)
    //            ||||+---- PC3(13): Eye LED(Red)
    //            |||+----- PC4(12): Eye LED(Green)
    //            ||+------ PC5(11): Eye LED(Blue)
    //            |+------- PC6(10): TXC0 Command Servo
    //            +-------- PC7( 9): Status LED(Yellow)
	PORTC_DIR = 0b11111110;
	PORTC_OUT = 0b11111110;
	
    // PORTD:     11011000
    //            |||||||+- PD0(28): Port of Bluetooth Connected LED(Red)
    //            ||||||+-- PD1(27): Port of Bluetooth Connect LED(Green)
    //            |||||+--- PD2(26): RXD0 Bluetooth SPP
    //            ||||+---- PD3(25): TXD0 Bluetooth SPP
    //            |||+----- PD4(24): Power Control of GPS
    //            ||+------ PD5(23): Software UART RX for GPS
    //            |+------- PD6(22): NC
    //            +-------- PD7(21): NC
    PORTD_DIR = 0b11011000;
	PORTD_OUT = 0b11011000;

	offLedStatus();
}
