/*
 * SothCore.h
 *
 * Created: 2015/04/11 10:50:19
 *  Author: Hideyuki Takei <hide@soth.io>
 */ 


#ifndef SOTHCORE_H_
#define SOTHCORE_H_


// Universal macro
#define nop()           __asm__ __volatile__ ("nop")
#define _sbi(port, bit) (port) |= (1 << (bit))
#define _cbi(port, bit) (port) &= ~(1 << (bit))

// LED
#define LED_POWER_PORT  PORTC
#define LED_POWER_PIN   4
#define LED_STATUS_PORT PORTC
#define LED_STATUS_PIN  5

// USART
#define USART_DEBUG_PORT    PORTC
#define USART_DEBUG_RXD_PIN 2
#define USART_DEBUG_TXD_PIN 3

// Drive LED macro
#define onLedPower()   _sbi(LED_POWER_PORT.OUT, LED_POWER_PIN)
#define offLedPower()  _cbi(LED_POWER_PORT.OUT, LED_POWER_PIN)
#define onLedStatus()  _sbi(LED_STATUS_PORT.OUT, LED_STATUS_PIN)
#define offLedStatus() _cbi(LED_STATUS_PORT.OUT, LED_STATUS_PIN)


#endif /* SOTHCORE_H_ */