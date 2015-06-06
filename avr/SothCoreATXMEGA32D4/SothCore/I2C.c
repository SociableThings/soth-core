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

uint8_t i = 0;

void initI2C()
{
    TWIC.CTRL = 0x00;
    TWIC.MASTER.BAUD = 0; // F_CPU / (2*I2C_FREQ) - 5 = 0
    TWIC.MASTER.CTRLB = 0x00;
    TWIC.MASTER.CTRLC = 0x00;
    TWIC.MASTER.CTRLA = TWI_MASTER_INTLVL1_bm | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;

    TWIC.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}

void read(uint8_t address)
{
    TWIC.MASTER.ADDR = address | TW_READ;
}

void write(uint8_t address)
{
    TWIC.MASTER.ADDR = address | TW_WRITE;
    //TWIC.MASTER.DATA = 0x0F;
}

ISR(TWIC_TWIM_vect)
{
    if(bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_ARBLOST_bp) || bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_BUSERR_bp)){
        xprintf("TWIC_TWIM_VECT ERROR\n");

        TWIC.MASTER.STATUS |= TWI_MASTER_ARBLOST_bm | TWI_MASTER_BUSERR_bm;
    }
    else{
        if(bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_WIF_bp)){
            xprintf("TWIC_TWIM_VECT WIF\n");

            TWIC.MASTER.STATUS |= TWI_MASTER_WIF_bm;

            if(i==1){
                TWIC.MASTER.ADDR = 0b10111000 | TW_READ;
            }
            else{
                TWIC.MASTER.DATA = 0x0F;
                i = 1;
            }

        }
        else if(bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_RIF_bp)){
            xprintf("TWIC_TWIM_VECT RIF: %X\n", TWIC.MASTER.DATA);

            TWIC.MASTER.STATUS |= TWI_MASTER_RIF_bm;
        }
        else if(TWIC.MASTER.STATUS & TWI_MASTER_BUSSTATE_IDLE_gc){
            xprintf("TWIC_TWIM_VECT Transaction unknown failure\n");
        }
        else{
            xprintf("TWIC_TWIM_VECT failure\n");
        }
    }
}

