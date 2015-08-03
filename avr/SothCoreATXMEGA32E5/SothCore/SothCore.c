/*
 * SothCore.c
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>  
#include <stdlib.h>
#include "SothCore.h"
#include "UsartCommunication.h"
#include "UsartGPS.h"
#include "I2C.h"
#include "BME280.h"
#include "UsartCmdServo.h"
#include "xprintf.h"
#include "MotionControl.h"
#include "MPU9150.h"

#define WAIT_TIME 200
#define UP_ANGLE 900
#define DOWN_ANGLE 700
#define CLOSE_ANGLE 0
#define RIGHT_YAW 200
#define LEFT_YAW -200
#define CENTER_YAW -30

// Prototypes
void initClock();
void initPort();
void onReadSensor(float temperature, float pressure, float humidity);
void onReadAccSensor(int16_t accX, double accY, double accZ);

int main(void)
{	
	char jsonString[1000];
    uint8_t servoData[200];

    // Initialization
	initPort();
	initClock();
	initUsartComm();
	initUsartGPS();
    initI2C();
	initUsartCmdServo();
    initBME280();
    initMPU9150();

    // Enable interrupt
    PMIC_CTRL =  PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;  // enable high/middle level interrupt executing
    sei();

    _delay_ms(1000);

    /*while(1){
        readAccGyro(onReadAccSensor);
        _delay_ms(2000);
    }*/

    //setStandaloneMotion();
    setOldStandaloneMotion();

    // LED test
    /*
    while(1){
        offLedStatus();
        _delay_ms(1000);
        onLedStatus();
        _delay_ms(1000);
        xprintf("hogege\r\n");
        //_delay_ms(1000);
        //xprintf("R,1\r\n");
    }
    */
    
    /*// GPS test
    powerOnGPS();
    while(1)
    {
		onLedStatus();
		_delay_ms(100);
		offLedStatus();
		_delay_ms(100);
        
        getGPRMCInfoAsJson(jsonString);
        //sendStringToComm(jsonString);

        _delay_ms(1000);
    }*/
    
    /*// I2C test
    uint8_t data[] = {0x20, 0x90};
    addQueue(0b10111000, 2, data, 0, testI2C);

    _delay_ms(2000);
    
    uint8_t data2[] = {0x28};
    addQueue(0b10111000, 1, data2, 1, testI2C);

    uint8_t data3[] = {0x29};
    addQueue(0b10111000, 1, data3, 1, testI2C);

    uint8_t data4[] = {0x2A};
    addQueue(0b10111000, 1, data4, 1, testI2C);*/

    
    /*// Temperature, pressure, humidity
    _delay_ms(1000);

    initBME280();

    while(1){
        //uint8_t address[] = {0xD0};
        //addQueue(0b11101100, 1, address, 1, testI2C);
        //address[0] = 0x75;
        //addQueue(0b11010000, 1, address, 1, testI2C);
        readTemperaturePressureHumidity(onReadSensor);

        _delay_ms(5000);

        onLedStatus();
        _delay_ms(100);
        offLedStatus();
        _delay_ms(100);
    }*/
    

    /*// Proto3 test
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

    /*
    // pb test
    _delay_ms(500);
    
    changeTorqueOnForAllServos();

    while(1){
        //onLedStatus();
        _delay_ms(500);
        //offLedStatus();
    }*/
}

void onReadAccSensor(int16_t accX, double accY, double accZ)
{

}

void onReadSensor(float temperature, float pressure, float humidity)
{
    char temperatureString[20], pressureString[20], humidityString[20];
    
    xprintf("## Recv sensor\n");
    
    dtostrf(temperature, 2, 2, temperatureString);
    dtostrf(pressure, 2, 2, pressureString);
    dtostrf(humidity, 2, 2, humidityString);
    xprintf("Temperature: %sdeg, Pressure: %shPa, Humidity: %s%%\n", temperatureString, pressureString, humidityString);
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
    //            |+------- PC6(10): RXC0 Command Servo (One wire half duplex)
    //            +-------- PC7( 9): Status LED(Yellow)
	PORTC_DIR = 0b11111110;
	PORTC_OUT = 0b11111110;
    PORTC_REMAP = 0b00010000; // Remap USARTC0 from PC0-3 to PC4-7
	
    // PORTD:     11011000
    //            |||||||+- PD0(28): Port of Bluetooth Connected LED(Red)
    //            ||||||+-- PD1(27): Port of Bluetooth Connect LED(Green)
    //            |||||+--- PD2(26): RXD0 Bluetooth SPP
    //            ||||+---- PD3(25): TXD0 Bluetooth SPP
    //            |||+----- PD4(24): Power Control of GPS (Active low)
    //            ||+------ PD5(23): Software UART RX for GPS
    //            |+------- PD6(22): NC
    //            +-------- PD7(21): NC
    PORTD_DIR = 0b11011000;
	PORTD_OUT = 0b11001000;

	offLedStatus();
    powerOffGPS();

    // Virtual Port
    // VPORT0: Virtual Port A
    // VPORT1: Virtual Port C
    // VPORT2: Virtual Port D
    // VPORT3: Virtual Port R
}
