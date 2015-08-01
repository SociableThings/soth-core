/*
 * TransitionEasing.c
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <avr/io.h>
#include <math.h>

#define PI 3.1415

uint16_t cubicEaseIn(float t, float b, float c, float d)
{
    return (uint16_t)(c*(t/=d)*t*t + b);
}
uint16_t cubicEaseOut(float t, float b, float c, float d)
{
    return (uint16_t)(c*((t=t/d-1)*t*t + 1) + b);
}

uint16_t cubicEaseInOut(float t, float b, float c, float d)
{
    if ((t/=d/2) < 1) return c/2*t*t*t + b;
    return (uint16_t)(c/2*((t-=2)*t*t + 2) + b);
}
