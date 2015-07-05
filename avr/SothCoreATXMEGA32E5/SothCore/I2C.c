/*
 * I2C.c
 *
 * Author: Hideyuki Takei
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include "I2C.h"
#include "xprintf.h"

// Prototypes
void forceIdle();
void forceStop();
void exceptionI2C(char* message);
void doMessage();
i2c_message_t getCurrentMessage();
void popAndDoNext();

// Variables
i2c_message_t i2cQueue[I2C_QUEUE_SIZE];
uint8_t i2cQueueIndex = 0;
i2c_state_t i2cState = IDLE;
uint8_t i2cIndex = 0;
uint8_t i2cData[I2C_DATA_SIZE];


void initI2C()
{
    //TWIC.CTRL = TWI_SDAHOLD_bm;
    TWIC.MASTER.BAUD = 0; // F_CPU / (2*I2C_FREQ) - 5 = 0
    TWIC.MASTER.CTRLB = TWI_MASTER_TIMEOUT_200US_gc;
    TWIC.MASTER.CTRLA = TWI_MASTER_INTLVL1_bm | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;

    forceIdle();
}

void addQueue(uint8_t address, uint8_t writeLength, uint8_t* writeData, uint8_t readLength, void (*func)(uint8_t length, uint8_t* data))
{
    uint8_t* writeDataCopy = (uint8_t *)malloc(sizeof(uint8_t) * writeLength);
    memcpy(writeDataCopy, writeData, sizeof(uint8_t) * writeLength);

    i2c_message_t message = {
        address,
        writeLength,
        writeDataCopy,
        readLength,
        func
    };
    i2cQueue[i2cQueueIndex++] = message;

    doMessage();
}

void addQueueForWrite(uint8_t address, uint8_t writeLength, uint8_t* writeData)
{
    addQueue(address, writeLength, writeData, 0, NULL);
}

void forceIdle()
{
    TWIC.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
    //xprintf("## forceIdle\n");
}

void forceStop()
{
    TWIC.MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
    //xprintf("## forceStop\n");
}

void exceptionI2C(char* message)
{
    //xprintf("exceptionI2C message: %s\n", message);

    forceIdle();

    popAndDoNext();
}

void doMessage()
{
    if(i2cState!=IDLE || i2cQueueIndex==0){
        //xprintf("doMessage queue is empty\n");
        return;
    }

    // write
    i2cState = WRITING;
    i2cIndex = 0;
    TWIC.MASTER.ADDR = getCurrentMessage().address | TW_WRITE;
}

i2c_message_t getCurrentMessage()
{
    return i2cQueue[0];
}

void popAndDoNext()
{    
    // Free current message
    free(getCurrentMessage().writeData);

    // Pop first message from i2c queue
    for(uint8_t i=0; i<I2C_QUEUE_SIZE-1; i++) i2cQueue[i] = i2cQueue[i + 1];
    i2cQueueIndex--;
    i2cState = IDLE;

    //xprintf("popAndDoNext i2cQueueIndex: %d\n", i2cQueueIndex);

    doMessage();
}

ISR(TWIC_TWIM_vect)
{
    if(bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_ARBLOST_bp) || bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_BUSERR_bp)){
        TWIC.MASTER.STATUS |= TWI_MASTER_ARBLOST_bm | TWI_MASTER_BUSERR_bm;

        exceptionI2C("Bus Error");
    }
    else{
        if(bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_WIF_bp)){
            TWIC.MASTER.STATUS |= TWI_MASTER_WIF_bm;
            //xprintf("TWIC_TWIM_VECT WIF\n");

            if(i2cState==WRITING){
                if(bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_RXACK_bp)){
                    // Recv NACK
                    //xprintf("TWIC_TWIM_VECT Recv NACK\n");
                    forceStop();
                    popAndDoNext();
                }
                else if(i2cIndex<getCurrentMessage().writeLength){
                    uint8_t data = getCurrentMessage().writeData[i2cIndex++];
                    TWIC.MASTER.DATA = data;

                    //xprintf(" >write data: 0x%X\n", data);
                }
                else {
                    // finished writing, next read
                    if(getCurrentMessage().readLength>0){
                        TWIC.MASTER.ADDR = getCurrentMessage().address | TW_READ;
                        i2cIndex = 0;
                        i2cState = READING;
                    }
                    else{
                        // no reading data
                        if(getCurrentMessage().func){
                            // callback
                            getCurrentMessage().func(0, i2cData);
                        }
                        
                        forceStop();
                        popAndDoNext();
                    }
                }
            }
            else{
                exceptionI2C("Writing Error");
            }
        }
        else if(bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_RIF_bp)){
            i2cData[i2cIndex] = TWIC.MASTER.DATA;
            TWIC.MASTER.STATUS |= TWI_MASTER_RIF_bm;
            //xprintf("TWIC_TWIM_VECT RIF[%d]: %X\n", i2cIndex, i2cData[i2cIndex]);

            i2cIndex++;

            if(i2cState==READING){
                if(i2cIndex<getCurrentMessage().readLength){
                    // continue reading
                    //xprintf("TWIC_TWIM_VECT RIF continue\n");
                    TWIC.MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
                }
                else{
                    // finished reading
                    //xprintf("TWIC_TWIM_VECT RIF finished. send Nack\n");

                    // send nack
                    TWIC.MASTER.CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;

                    if(getCurrentMessage().func){
                        // callback
                        getCurrentMessage().func(getCurrentMessage().readLength, i2cData);
                    }

                    popAndDoNext();
                }
            }
            else{
                exceptionI2C("Reading Error");
            }
        }
        else if(TWIC.MASTER.STATUS & TWI_MASTER_BUSSTATE_IDLE_gc){
            //xprintf("TWIC_TWIM_VECT Transaction unknown failure\n");
            exceptionI2C("BusState Idle Error");
        }
        else{
            //xprintf("TWIC_TWIM_VECT failure\n");
            exceptionI2C("Unknown Error");
        }
    }
}

