/*
 * MPU9150.c
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "I2C.h"
#include "xprintf.h"
#include "MPU9150.h"

// prototypes
void onReadAccData(uint8_t length, uint8_t* data);

void initMPU9150()
{
    uint8_t data[3];
    
    /*data[0] = 0x6B;
    data[1] = 0x00;
    data[2] = 0x00;
    addQueueForWrite(MPU9150_ACC_GYRO_ADDRESS, 3, data);

    _delay_ms(10);

    data[0] = 0x37;
    data[1] = 0x02;
    addQueueForWrite(MPU9150_ACC_GYRO_ADDRESS, 2, data);*/

    //addQueue(MPU9150_ACC_GYRO_ADDRESS, 1, data, )

    xprintf("222\n");
}
 
void readAccGyro(void (*func)(int16_t accX, double accY, double accZ))
{
    xprintf("readAccGyro\n");
    uint8_t data[3];

    data[0] = 0x6B;
    data[1] = 0x00;
    addQueueForWrite(MPU9150_ACC_GYRO_ADDRESS, 2, data);

    data[0] = 0x37;
    data[1] = 0x02;
    addQueueForWrite(MPU9150_ACC_GYRO_ADDRESS, 2, data);

    data[0] = 0x3B;
    addQueue(MPU9150_ACC_GYRO_ADDRESS, 1, data, 14, onReadAccData);
}

void onReadAccData(uint8_t length, uint8_t* data)
{
    xprintf("onReadAccData\n");
    for(uint8_t i=0; i<length; i++){
      xprintf("acc %d 0x%X\n", data[i], data[i]);
    }
}
