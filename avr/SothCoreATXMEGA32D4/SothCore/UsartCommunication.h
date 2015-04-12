/*
 * UsartCommunication.h
 *
 *  Author: Hideyuki Takei <hide@soth.io>
 */ 


#ifndef USART_COMMUNICATION_H_
#define USART_COMMUNICATION_H_


void initUsartComm();
void sendCharacterToComm(const char);
void sendStringToComm(const char *);


#endif /* USART_COMMUNICATION_H_ */