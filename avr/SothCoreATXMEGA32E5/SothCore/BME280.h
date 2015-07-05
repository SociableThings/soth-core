/*
 * BME280.h
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 


#ifndef BME280_H_
#define BME280_H_


#define BME280_ADDRESS 0b11101100
#define BME280_TRIM_DATA_LENGTH 32

void initBME280();
void readTemperaturePressureHumidity(void (*func)(double temperature, double pressure, double humidity));


#endif /* BME280_H_ */