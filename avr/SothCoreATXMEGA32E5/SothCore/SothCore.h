/*
 * SothCore.h
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 


#ifndef SOTH_CORE_H_
#define SOTH_CORE_H_


// Universal macro
#define nop()           __asm__ __volatile__ ("nop")
#define _sbi(port, bit) (port) |= (1 << (bit))
#define _cbi(port, bit) (port) &= ~(1 << (bit))

// LED
#define LED_STATUS_PORT PORTC
#define LED_STATUS_PIN  7

// Bluetooth reset
#define BLUETOOTH_RESET_PORT PORTC
#define BLUETOOTH_RESET_PIN  2

// Communication USART
#define USART_COMMUNICATION_PORT    PORTD
#define USART_COMMUNICATION_RXD_PIN 2
#define USART_COMMUNICATION_TXD_PIN 3

// Servo USART
#define USART_SERVO_PIN 6

// Software USART
#define SUSART_PORT_OUT VPORT3_OUT
#define SUSART_PORT_IN  VPORT3_IN
#define SUSART_RXD_PIN  5
#define SUARTT_CTRL_PIN 4

// Drive LED macro
#define onLedStatus()  _sbi(LED_STATUS_PORT.OUT, LED_STATUS_PIN)
#define offLedStatus() _cbi(LED_STATUS_PORT.OUT, LED_STATUS_PIN)

// Drive Bluetooth  reset macro
#define enableResetBluetooth() _cbi(BLUETOOTH_RESET_PORT.OUT, BLUETOOTH_RESET_PIN)
#define disableResetBluetooth() _sbi(BLUETOOTH_RESET_PORT.OUT, BLUETOOTH_RESET_PIN)

#endif /* SOTH_CORE_H_ */