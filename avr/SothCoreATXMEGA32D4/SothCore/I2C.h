/*
 * I2C.h

 * Author: Hideyuki Takei
 */ 


#ifndef I2C_H_
#define I2C_H_

#define I2C_FREQ 100000

void initI2C();
void read(uint8_t address);
void write(uint8_t address);


#endif /* I2C_H_ */