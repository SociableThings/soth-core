/*
 * UsartGPS.h
 *
 *  Author: Hideyuki Takei
 */ 


#ifndef USART_GPS_H_
#define USART_GPS_H_


#define SUART_BUFFER_LENGTH 500
#define GPRMC_LENGTH 13

typedef struct
{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;       // current time GMT
	uint8_t minute;     // current time GMT
	uint8_t second;	    // current time GMT
	uint8_t validity;   // 0: invalid, 1: valid
	double latitude;
	double longitude;
	float speed;
	float heading;
} NMEA_GPRMC;

void initUsartGPS();
void powerOnGPS();
void powerOffGPS();
void getGPRMCInfoAsJson(char*);

#endif /* USART_GPS_H_ */