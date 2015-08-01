/*
 * TransitionEasing.h
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 


#ifndef TRANSITIONEASING_H_
#define TRANSITIONEASING_H_


uint16_t cubicEaseIn(float t, float b, float c, float d);
uint16_t cubicEaseOut(float t, float b, float c, float d);
uint16_t cubicEaseInOut(float t, float b, float c, float d);

uint16_t backEaseIn(float t,float b, float c, float d);
uint16_t backEaseOut(float t,float b, float c, float d);
uint16_t backEaseInOut(float t,float b, float c, float d);

#endif /* TRANSITIONEASING_H_ */