/*
 * I2C.h

 * Author: Hideyuki Takei
 */ 


#ifndef I2C_H_
#define I2C_H_

#define I2C_FREQ 100000
#define I2C_QUEUE_SIZE 10
#define I2C_DATA_SIZE 10

typedef struct {
    uint8_t  address;
    uint8_t  writeLength;
    uint8_t* writeData;
    uint8_t readLength;
    void     (*func)(uint8_t length, uint8_t* data);
} i2c_message_t;

typedef enum {
    IDLE,
    WRITING,
    READING
} i2c_state_t;

void initI2C();
void addQueue(uint8_t address, uint8_t writeLength, uint8_t* writeData, uint8_t readLength, void (*func)(uint8_t length, uint8_t* data));


#endif /* I2C_H_ */