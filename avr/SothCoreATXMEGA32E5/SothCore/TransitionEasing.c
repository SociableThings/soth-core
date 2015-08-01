/*
 * TransitionEasing.c
 *
 * Author: Hideyuki Takei <hide@soth.io>
 */ 

#include <avr/io.h>
#include <math.h>

// Cubic
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

// Back
uint16_t backEaseIn(float t,float b, float c, float d)
{
    float s = 1.70158f;
    float postFix = t/=d;
    return c*(postFix)*t*((s+1)*t - s) + b;
}
uint16_t backEaseOut(float t,float b, float c, float d)
{
    float s = 1.70158f;
    return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
}

uint16_t backEaseInOut(float t,float b, float c, float d)
{
    float s = 1.70158f;
    if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b;
    float postFix = t-=2;
    return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b;
}
