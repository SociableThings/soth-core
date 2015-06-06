/*
 * UsartGPS.c
 *
 * Author: Hideyuki Takei
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "UsartGPS.h"
#include "suart.h"
#include "UsartCommunication.h"
#include "xprintf.h"

// Prototypes
void getGPSLine(char*, const uint8_t);
void getGPRMCLine(char*, const uint8_t);
NMEA_GPRMC getGPRMCInfo();


void initUsartGPS()
{
	// nop
}

void getGPSLine(char* str, const uint8_t length)
{
	int i=1;
	char c;
	
	if(length==0) return;
	
	while(1){
		c = rcvr();
		if(c=='$'){
			str[0] = c;
			break;
		}
	}
	
	while(1){
		str[i] = rcvr();
		if(str[i]=='\n'){
			break;
		}
		
		i++;
		
		if(i>=length){
			break;
		}
	}
}

void getGPRMCLine(char* str, const uint8_t length)
{
	while(1){
		getGPSLine(str, length);
		if(str[1]=='G' && str[2]=='P' && str[3]=='R' && str[4]=='M' && str[5]=='C'){
			return;
		}
	}
}

NMEA_GPRMC getGPRMCInfo()
{
	NMEA_GPRMC info;
	uint8_t i, dataIndex, stringIndex=0;
	char str[SUART_BUFFER_LENGTH];
	char data[100];
	char c;
	char time[2];
	float latlon;
	int latlon_int;
	
	getGPRMCLine(str, SUART_BUFFER_LENGTH);
	
	for(i=0; i<GPRMC_LENGTH; i++){
		dataIndex = 0;
		
		while(1){
			c = str[stringIndex++];
			
			if(c==',' || c=='*'){
				data[dataIndex++] = '\0';
				break;
			} else {
				data[dataIndex++] = c;
			}
		}
		
		switch(i){
			case 0: // Sentence type, $GPRMC
				break;
				
			case 1: // Current time (in hhmmss format), 052953.000
				// hour
				time[0] = data[0];
				time[1] = data[1];
				info.hour = (uint8_t)atoi(time);
				
				// minute
				time[0] = data[2];
				time[1] = data[3];
				info.minute = (uint8_t)atoi(time);
				
				// second
				time[0] = data[4];
				time[1] = data[5];
				info.second = (uint8_t)atoi(time);
				break;
				
			case 2: // Data validity flag
				// A = valid current data
				// B = valid stored data
				// V = invalid current data
				// W = invalid stored data
				if(data[0]=='A' || data[0]=='B'){
					info.validity = 1;
				} else {
					info.validity = 0;
				}
				break;
				
			case 3: // Latitude, 3538.9921
				if(info.validity){
					latlon = atof(data) / 100;
					latlon_int = (int)latlon;
					latlon = latlon - latlon_int;
					info.latitude = (double)latlon_int + (double)latlon*100/60;
				} else {
					info.latitude = 0.0;
				}
				break;
				
			case 5: // Longitude, 13924.1102
				if(info.validity){
					latlon = atof(data) / 100;
					latlon_int = (int)latlon;
					latlon = latlon - latlon_int;
					info.longitude = (double)latlon_int + (double)latlon*100/60;
				} else {
					info.longitude = 0.0;
				}
				break;
				
			case 7: // Speed (in knots/h -> km/h)
				if(info.validity){
					info.speed = atof(data) * 1.852;
				} else {
					info.speed = 0.0;
				}
				break;
				
			case 8: // Heading
				if(info.validity){
					info.heading = atof(data);
				} else {
					info.heading = 0.0;
				}
				break;
				
			case 9: // Date
				// day
				time[0] = data[0];
				time[1] = data[1];
				info.day = (uint8_t)atoi(time);
				
				// month
				time[0] = data[2];
				time[1] = data[3];
				info.month = (uint8_t)atoi(time);
				
				// year
				time[0] = data[4];
				time[1] = data[5];
				info.year = (uint8_t)atoi(time) +2000;				
				break;
		}
	}
	
	return info;
}

void getGPRMCInfoAsJson(char* jsonString)
{
	NMEA_GPRMC info = getGPRMCInfo();
	char lat[10], lon[10], speed[10], heading[12];
	
	dtostrf(info.latitude, 7, 5, lat);
	dtostrf(info.longitude, 8, 5, lon);
	dtostrf(info.speed, 4, 2, speed);
	dtostrf(info.heading, 5, 2, heading);
	
	xsprintf(
		jsonString,
		"{\"year\":%d,\"month\":%d,\"day\":%d,\"hour\":%d,\"minute\":%d,\"second\":%d,"
		"\"validity\":%d,\"latitude\":%s,\"longitude\":%s,\"speed\":%s,\"heading\":%s}\n",
		info.year,
		info.month,
		info.day,
		info.hour,
		info.minute,
		info.second,
		info.validity,
		lat,
		lon,
		speed,
		heading
	);
}
