/*
 * SothCore.h
 *
 *  Author: Hideyuki Takei <hide@soth.io>
 */ 


#ifndef SOTH_CORE_H_
#define SOTH_CORE_H_


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
#define USART_COMMUNICATION_PORT    PORTC
#define USART_COMMUNICATION_RXD_PIN 2
#define USART_COMMUNICATION_TXD_PIN 3

// Software USART
#define SUSART_PORT_OUT VPORT2_OUT
#define SUSART_PORT_IN  VPORT2_IN
#define SUSART_RXD_PIN  6
#define SUSART_TXD_PIN  7

// Drive LED macro
#define onLedPower()   _sbi(LED_POWER_PORT.OUT, LED_POWER_PIN)
#define offLedPower()  _cbi(LED_POWER_PORT.OUT, LED_POWER_PIN)
#define onLedStatus()  _sbi(LED_STATUS_PORT.OUT, LED_STATUS_PIN)
#define offLedStatus() _cbi(LED_STATUS_PORT.OUT, LED_STATUS_PIN)


#endif /* SOTH_CORE_H_ */