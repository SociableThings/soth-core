/*
 * BME280.c
 *
 * Author:    Hideyuki Takei <hide@soth.io>
 * Reference: http://trac.switch-science.com/wiki/BME280
 *            https://github.com/SWITCHSCIENCE/BME280/blob/master/Arduino/BME280_I2C/BME280_I2C.ino
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "BME280.h"
#include "I2C.h"
#include "xprintf.h"

// Global
uint8_t bme280TrimDataIndex;
uint8_t bme280TrimData[BME280_TRIM_DATA_LENGTH];

uint16_t digT1;
int16_t  digT2, digT3;

uint16_t digP1;
int16_t  digP2, digP3, digP4, digP5, digP6, digP7, digP8, digP9;

uint16_t digH1, digH3;
int16_t  digH2, digH4, digH5, digH6;

signed long int   tFine;


void (*onReadDataFunction)(double temperature, double pressure, double humidity);

// prototypes
void _onReadTrimData(uint8_t length, uint8_t* data);
void _onReadRawData(uint8_t length, uint8_t* data);
signed long int _calibrationTemperature(unsigned long int adcTemperature);
unsigned long int _calibrationPressure(unsigned long int adcPressur);
unsigned long int _calibrationHumidity(unsigned long int adcHumidity);

void initBME280()
{
    uint8_t data[2];

    uint8_t osrsT = 4;     // Temperature oversampling x 8
    uint8_t osrsP = 4;     // Pressure oversampling x 8
    uint8_t osrsH = 4;     // Humidity oversampling x 8
    uint8_t mode = 3;       // Normal mode
    uint8_t tSb = 5;       // Tstandby 1000ms
    uint8_t filter = 1;     // Filter 2 coeffient
    uint8_t spi3wEn = 0;   // 3-wire SPI Disable
    
    uint8_t ctrlMeasReg = (osrsT << 5) | (osrsP << 2) | mode;
    uint8_t configReg    = (tSb << 5) | (filter << 2) | spi3wEn;
    uint8_t ctrlHumReg  = osrsH;

    xprintf("meas: %X, config: %X, hum: %X\n", ctrlMeasReg, configReg, ctrlHumReg);

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
    addQueue(BME280_ADDRESS, 1, data, 24, _onReadTrimData);

    data[0] = 0xA1;
    addQueue(BME280_ADDRESS, 1, data, 1, _onReadTrimData);

    data[0] = 0xE1;
    addQueue(BME280_ADDRESS, 1, data, 7, _onReadTrimData);
}

void readTemperaturePressureHumidity(void (*func)(double temperature, double pressure, double humidity))
{
    uint8_t data[1];

    onReadDataFunction = func;
    
    data[0] = 0xF7;
    addQueue(BME280_ADDRESS, 1, data, 8, _onReadRawData);
}

void _onReadTrimData(uint8_t length, uint8_t* data)
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

void _onReadRawData(uint8_t length, uint8_t* data)
{
    for(uint8_t i=0; i<length; i++){
        xprintf("rawData[%d] = 0x%02X\n", i, data[i]);
    }

    double actTemperature = 0.0, actPressure = 0.0, actHumidity = 0.0;
    signed long int calTemperature;
    unsigned long int calPressure, calHumidity;
    uint32_t rawHumidity, rawTemperature, rawPressure;

    rawPressure =    (uint)((data[0] << 12) | (data[1] << 4) | (data[2] >> 4));
    rawTemperature = (uint32_t)((data[3] << 12) | (data[4] << 4) | (data[5] >> 4));
    rawHumidity  =   (uint32_t)((data[6] << 8) | data[7]);

    //char p[20];
    //ultoa(rawPressure, p, 10);
    xprintf("Pressure: %016b\n", (rawPressure>>16)&0xFFFF);

    calTemperature = _calibrationTemperature(rawTemperature);
    //calPressure = _calibrationPressure(rawPressure);
    //calHumidity = _calibrationHumidity(rawHumidity);

    actTemperature = (double)calTemperature / 100.0;
    //actPressure = (double)calPressure / 100.0;
    //actHumidity = (double)calHumidity / 1024.0;

    char tmp[20], prs[20], hum[20];
    dtostrf(actTemperature, 10, 3, tmp);
    dtostrf(actPressure, 10, 3, prs);
    dtostrf(actHumidity, 10, 3, hum);
    xprintf("Temperature: %s deg, Pressure: %s hPa, Humidity: %s %\n", tmp, prs, hum);
}

signed long int _calibrationTemperature(unsigned long int adcTemperature)
{
    signed long int var1, var2, T;
    var1 = (((((signed long int)adcTemperature >> 3) - ((signed long int)digT1 << 1))) * ((signed long int)digT2)) >> 11;
    var2 = ((((((signed long int)adcTemperature >> 4) - ((signed long int)digT1)) * (((signed long int)adcTemperature >> 4) - ((signed long int)digT1))) >> 12) * ((signed long int)digT3)) >> 14;

    tFine = var1 + var2;
    T = (tFine * 5 + 128) >> 8;

    return T;
}

unsigned long int _calibrationPressure(unsigned long int adcPressur)
{
    signed long int var1, var2;
    unsigned long int P;
    var1 = (((signed long int)tFine)>>1) - (signed long int)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11) * ((signed long int)digP6);
    var2 = var2 + ((var1*((signed long int)digP5))<<1);
    var2 = (var2>>2)+(((signed long int)digP4)<<16);
    var1 = (((digP3 * (((var1>>2)*(var1>>2)) >> 13)) >>3) + ((((signed long int)digP2) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((signed long int)digP1))>>15);
    if (var1 == 0)
    {
        return 0;
    }
    P = (((unsigned long int)(((signed long int)1048576)-adcPressur)-(var2>>12)))*3125;
    if(P<0x80000000)
    {
        P = (P << 1) / ((unsigned long int) var1);
    }
    else
    {
        P = (P / (unsigned long int)var1) * 2;
    }
    var1 = (((signed long int)digP9) * ((signed long int)(((P>>3) * (P>>3))>>13)))>>12;
    var2 = (((signed long int)(P>>2)) * ((signed long int)digP8))>>13;
    P = (unsigned long int)((signed long int)P + ((var1 + var2 + digP7) >> 4));
    return P;
}

unsigned long int _calibrationHumidity(unsigned long int adcHumidity)
{
    signed long int vX1;
    
    vX1 = (tFine - ((signed long int)76800));
    vX1 = (((((adcHumidity << 14) -(((signed long int)digH4) << 20) - (((signed long int)digH5) * vX1)) +
        ((signed long int)16384)) >> 15) * (((((((vX1 * ((signed long int)digH6)) >> 10) *
        (((vX1 * ((signed long int)digH3)) >> 11) + ((signed long int) 32768))) >> 10) + (( signed long int)2097152)) *
        ((signed long int) digH2) + 8192) >> 14));
    vX1 = (vX1 - (((((vX1 >> 15) * (vX1 >> 15)) >> 7) * ((signed long int)digH1)) >> 4));
    vX1 = (vX1 < 0 ? 0 : vX1);
    vX1 = (vX1 > 419430400 ? 419430400 : vX1);
    return (unsigned long int)(vX1 >> 12);
}
