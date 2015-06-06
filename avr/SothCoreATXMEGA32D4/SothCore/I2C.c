/*
 * I2C.c
 *
 * Author: Hideyuki Takei
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <avr/sfr_defs.h>
#include "I2C.h"
#include "xprintf.h"

// Prototypes
void doMessage();
i2c_message_t getCurrentMessage();
void popAndDoNext();

// Variables
i2c_message_t i2cQueue[I2C_QUEUE_SIZE];
uint8_t i2cQueueIndex = 0;
i2c_state_t i2cState = IDLE;
uint8_t i2cIndex = 0;
uint8_t* i2cData[I2C_DATA_SIZE];

void initI2C()
{
    TWIC.CTRL = 0x00;
    TWIC.MASTER.BAUD = 0; // F_CPU / (2*I2C_FREQ) - 5 = 0
    TWIC.MASTER.CTRLB = 0x00;
    TWIC.MASTER.CTRLC = 0x00;
    TWIC.MASTER.CTRLA = TWI_MASTER_INTLVL1_bm | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;

    TWIC.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}

void addQueue(uint8_t address, uint8_t writeLength, uint8_t* writeData, uint8_t readLength, void (*func)(uint8_t length, uint8_t* data))
{
    i2c_message_t message = {
        address,
        writeLength,
        writeData,
        readLength,
        func
    };
    i2cQueue[i2cQueueIndex++] = message;

    doMessage();
}

void doMessage()
{
    if(i2cState!=IDLE || i2cQueueIndex==0) return;

    // write
    i2cState = WRITING;
    TWIC.MASTER.ADDR = getCurrentMessage().address | TW_WRITE;
    i2cIndex = 0;
}

i2c_message_t getCurrentMessage()
{
    return i2cQueue[0];
}

void popAndDoNext()
{
    // Pop first message from i2c queue
    for(uint8_t i=0; i<I2C_QUEUE_SIZE-1; i++) i2cQueue[i] = i2cQueue[i + 1];
    i2cQueueIndex--;
    doMessage();
}

ISR(TWIC_TWIM_vect)
{
    if(bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_ARBLOST_bp) || bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_BUSERR_bp)){
        TWIC.MASTER.STATUS |= TWI_MASTER_ARBLOST_bm | TWI_MASTER_BUSERR_bm;

        xprintf("TWIC_TWIM_VECT ERROR\n");
    }
    else{
        if(bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_WIF_bp)){
            TWIC.MASTER.STATUS |= TWI_MASTER_WIF_bm;

            xprintf("TWIC_TWIM_VECT WIF\n");

            if(i2cIndex<getCurrentMessage().writeLength){
                uint8_t data = getCurrentMessage().writeData[i2cIndex++];
                TWIC.MASTER.DATA = data;

                xprintf(" >recv data: 0x%X\n", data);
            }
            else {
                // finished writing, next read
                TWIC.MASTER.ADDR = getCurrentMessage().address | TW_READ;
                i2cIndex = 0;
                i2cState = READING;
            } 

        }
        else if(bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_RIF_bp)){
            i2cData[i2cIndex] = TWIC.MASTER.DATA;
            TWIC.MASTER.STATUS |= TWI_MASTER_RIF_bm;

            xprintf("TWIC_TWIM_VECT RIF: %X\n", i2cData[i2cIndex]);

            i2cIndex++;
            if(i2cIndex<getCurrentMessage().readLength){
                // continue reading
            }
            else{
                // finished reading
                i2cState = IDLE;
                i2cIndex = 0;
                
                if(getCurrentMessage().func){
                    // callback
                    getCurrentMessage().func(getCurrentMessage().readLength, i2cData);
                }

                popAndDoNext();
            }
        }
        else if(TWIC.MASTER.STATUS & TWI_MASTER_BUSSTATE_IDLE_gc){
            xprintf("TWIC_TWIM_VECT Transaction unknown failure\n");
        }
        else{
            xprintf("TWIC_TWIM_VECT failure\n");
        }
    }
}

