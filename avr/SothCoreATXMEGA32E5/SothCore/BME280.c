/*
 * BME280.c
 *
 * Author:    Hideyuki Takei <hide@soth.io>
 * Reference: http://trac.switch-science.com/wiki/BME280
 *            https://github.com/SWITCHSCIENCE/BME280/blob/master/Arduino/BME280_I2C/BME280_I2C.ino
 *            https://developer.mbed.org/users/MACRUM/code/BME280/
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "BME280.h"
#include "I2C.h"
#include "xprintf.h"

// Variables
uint8_t bme280TrimDataIndex;
uint8_t bme280TrimData[BME280_TRIM_DATA_LENGTH];

uint16_t digT1;
int16_t  digT2, digT3;

uint16_t digP1;
int16_t  digP2, digP3, digP4, digP5, digP6, digP7, digP8, digP9;

uint16_t digH1, digH3;
int16_t  digH2, digH4, digH5, digH6;

int32_t tFine;

void (*onReadDataFunction)(float temperature, float pressure, float humidity);


// prototypes
void onReadTrimData(uint8_t length, uint8_t* data);
void onReadRawData(uint8_t length, uint8_t* data);
float readTemperature(uint32_t rawTemperature);
float readPressure(uint32_t rawPressur);
float readHumidity(uint32_t rawHumidity);


void initBME280()
{
    uint8_t data[2];

    uint8_t osrsT = 3;     // Temperature oversampling x 4
    uint8_t osrsP = 3;     // Pressure oversampling x 4
    uint8_t osrsH = 3;     // Humidity oversampling x 4
    uint8_t mode = 3;      // Normal mode
    uint8_t tSb = 5;       // Tstandby 1000ms
    uint8_t filter = 3;    // Filter 8 coeffient
    uint8_t spi3wEn = 0;   // 3-wire SPI Disable
    
    uint8_t ctrlMeasReg = (osrsT << 5) | (osrsP << 2) | mode;
    uint8_t configReg    = (tSb << 5) | (filter << 2) | spi3wEn;
    uint8_t ctrlHumReg  = osrsH;

    //xprintf("meas: %X, config: %X, hum: %X\n", ctrlMeasReg, configReg, ctrlHumReg);

    data[0] = 0xF2;
    data[1] = ctrlHumReg;
    addQueueForWrite(BME280_ADDRESS, 2, data);

    data[0] = 0xF4;
    data[1] = ctrlMeasReg;
    addQueueForWrite(BME280_ADDRESS, 2, data);

    data[0] = 0xF5;
    data[1] = configReg;
    addQueueForWrite(BME280_ADDRESS, 2, data);

    // Read trim
    bme280TrimDataIndex = 0;
    data[0] = 0x88;
    addQueue(BME280_ADDRESS, 1, data, 24, onReadTrimData);

    data[0] = 0xA1;
    addQueue(BME280_ADDRESS, 1, data, 1, onReadTrimData);

    data[0] = 0xE1;
    addQueue(BME280_ADDRESS, 1, data, 7, onReadTrimData);
}

void readTemperaturePressureHumidity(void (*func)(float temperature, float pressure, float humidity))
{
    uint8_t data[1];

    onReadDataFunction = func;
    
    data[0] = 0xF7;
    addQueue(BME280_ADDRESS, 1, data, 8, onReadRawData);
}

void onReadTrimData(uint8_t length, uint8_t* data)
{
    for(uint8_t i=0; i<length; i++){
        bme280TrimData[bme280TrimDataIndex++] = data[i];
        //xprintf("bme280TrimDataIndex[%d] = 0x%02X\n", bme280TrimDataIndex-1, data[i]);
    }

    if(bme280TrimDataIndex+1>=BME280_TRIM_DATA_LENGTH){
        digT1 = (bme280TrimData[1] << 8) | bme280TrimData[0];
        digT2 = (bme280TrimData[3] << 8) | bme280TrimData[2];
        digT3 = (bme280TrimData[5] << 8) | bme280TrimData[4];

        digP1 = (bme280TrimData[7] << 8) | bme280TrimData[6];
        digP2 = (bme280TrimData[9] << 8) | bme280TrimData[8];
        digP3 = (bme280TrimData[11]<< 8) | bme280TrimData[10];
        digP4 = (bme280TrimData[13]<< 8) | bme280TrimData[12];
        digP5 = (bme280TrimData[15]<< 8) | bme280TrimData[14];
        digP6 = (bme280TrimData[17]<< 8) | bme280TrimData[16];
        digP7 = (bme280TrimData[19]<< 8) | bme280TrimData[18];
        digP8 = (bme280TrimData[21]<< 8) | bme280TrimData[20];
        digP9 = (bme280TrimData[23]<< 8) | bme280TrimData[22];

        digH1 = bme280TrimData[24];
        digH2 = (bme280TrimData[26]<< 8) | bme280TrimData[25];
        digH3 = bme280TrimData[27];
        digH4 = (bme280TrimData[28]<< 4) | (bme280TrimData[29] & 0x0F);
        digH5 = (bme280TrimData[30]<< 4) | ((bme280TrimData[29] >> 4) & 0x0F);
        digH6 = bme280TrimData[31];
        xprintf("Finish\n");
    }
}

void onReadRawData(uint8_t length, uint8_t* data)
{
    uint32_t rawTemperature, rawPressure, rawHumidity;
    float temperature, pressure, humidity;

    //for(uint8_t i=0; i<length; i++){
    //    xprintf("rawData[%d] = 0x%02X\n", i, data[i]);
    //}

    rawPressure    = (uint32_t)(((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | ((uint32_t)data[2] >> 4));
    rawTemperature = (uint32_t)(((uint32_t)data[3] << 12) | ((uint32_t)data[4] << 4) | ((uint32_t)data[5] >> 4));
    rawHumidity    = (uint32_t)(((uint32_t)data[6] << 8) | (uint32_t)data[7]);
    
    //char p[40];
    //ultoa(rawPressure, p, 16);
    //xprintf("Pressure: %s\n", p);

    temperature = readTemperature(rawTemperature);
    pressure    = readPressure(rawPressure);
    humidity    = readHumidity(rawHumidity);

    if(onReadDataFunction){
        // callback
        onReadDataFunction(temperature, pressure, humidity);
        onReadDataFunction = NULL;
    }
}

float readTemperature(uint32_t rawTemperature)
{
    int32_t temp;
    float tempf;

    temp =
        (((((rawTemperature >> 3) - (digT1 << 1))) * digT2) >> 11) +
        ((((((rawTemperature >> 4) - digT1) * ((rawTemperature >> 4) - digT1)) >> 12) * digT3) >> 14);

    tFine = temp;

    temp = (temp * 5 + 128) >> 8;
    tempf = (float)temp;
    tempf = tempf / 100.0f;

    return tempf;
}

float readPressure(uint32_t rawPressure)
{
    int32_t var1, var2;
    uint32_t press;
    float pressf;
    
    var1 = (tFine >> 1) - 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * digP6;
    var2 = var2 + ((var1 * digP5) << 1);
    var2 = (var2 >> 2) + (digP4 << 16);
    var1 = (((digP3 * (((var1 >> 2)*(var1 >> 2)) >> 13)) >> 3) + ((digP2 * var1) >> 1)) >> 18;
    var1 = ((32768 + var1) * digP1) >> 15;
    if (var1 == 0) {
        return 0;
    }
    press = (((1048576 - rawPressure) - (var2 >> 12))) * 3125;
    if(press < 0x80000000) {
        press = (press << 1) / var1;
    } else {
        press = (press / var1) * 2;
    }
    var1 = ((int32_t)digP9 * ((int32_t)(((press >> 3) * (press >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(press >> 2)) * (int32_t)digP8) >> 13;
    press = (press + ((var1 + var2 + digP7) >> 4));
    
    pressf = (float)press;

    return (pressf/100.0f);
}

float readHumidity(uint32_t rawHumidity)
{
    float humf;    
    int32_t vX1;
    
    vX1 = tFine - 76800;
    vX1 =  (((((rawHumidity << 14) -(((int32_t)digH4) << 20) - (((int32_t)digH5) * vX1)) +
           ((int32_t)16384)) >> 15) * (((((((vX1 * (int32_t)digH6) >> 10) *
           (((vX1 * ((int32_t)digH3)) >> 11) + 32768)) >> 10) + 2097152) *
           (int32_t)digH2 + 8192) >> 14));
    vX1 = (vX1 - (((((vX1 >> 15) * (vX1 >> 15)) >> 7) * (int32_t)digH1) >> 4));
    vX1 = (vX1 < 0 ? 0 : vX1);
    vX1 = (vX1 > 419430400 ? 419430400 : vX1);
    
    humf = (float)(vX1 >> 12);
    
    return (humf/1024.0f);
}
