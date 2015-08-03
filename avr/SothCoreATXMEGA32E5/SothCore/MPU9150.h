/*
 * MPU9150.h
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 


#ifndef MPU9150_H_
#define MPU9150_H_


#define MPU9150_ACC_GYRO_ADDRESS 0b11010000
#define MPU9150_COMPASS_ADDRESS  0b00011000

void initMPU9150();
void readAccGyro(void (*func)(int16_t accX, double accY, double accZ));

#endif /* MPU9150_H_ */