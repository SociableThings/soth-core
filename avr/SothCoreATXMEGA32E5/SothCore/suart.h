#ifndef SUART
#define SUART

void xmit(uint8_t);
void xmitstr(const int *);
void xmitval(int16_t, int8_t, int8_t);
void xmitf(const int *, ...);
uint8_t rcvr();
void rcvrstr(char *, uint8_t);
uint8_t pickval(char **, uint16_t *, uint8_t);

#endif	/* SUART */
