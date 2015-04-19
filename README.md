# soth-core

---

## About
Hardware core module for Sociable Things Agent.

## Architecture

Agent

![image](https://dl.dropboxusercontent.com/u/180053/soth/soth-bonsai-concept.002.jpg)

Core

![Architecture](https://dl.dropboxusercontent.com/u/180053/soth/soth-bonsai-concept.003.jpg)

## Data format
### GPS
```
{
    "year": 2015,
    "month": 4,
    "day": 12,
    "hour": 14,
    "minute": 0,
    "second": 48,
    "validity": 1,
    "latitude": 35.69020,
    "longitude": 139.69001,
    "speed": 0.74,
    "heading": 102.72
}
```

## Progress

Now prototyping

![image](https://dl.dropboxusercontent.com/u/180053/soth/soth-core/circuit.jpg)

![image](https://dl.dropboxusercontent.com/u/180053/soth/soth-core/browser.jpg)


## GPS bore data

valid GPRMC

```
$GPGGA,150754.000,3540.6036,N,13940.9576,E,2,5,1.85,67.2,M,39.4,M,0000,0000*6A
$GPGLL,3540.6036,N,13940.9576,E,150754.000,A,D*5D
$GPGSA,A,3,28,03,01,06,17,,,,,,,,2.08,1.85,0.95*00
$GPGSV,4,1,13,03,71,081,15,28,56,235,35,17,49,320,16,01,42,051,15*77
$GPGSV,4,2,13,11,32,077,,32,27,046,,06,20,269,35,04,17,067,*76
$GPGSV,4,3,13,23,17,136,,09,05,164,,10,05,213,,44,04,260,*74
$GPGSV,4,4,13,19,03,118,*48
$GPRMC,150754.000,A,3540.6036,N,13940.9576,E,0.06,198.48,110415,,,D*60
$GPVTG,198.48,T,,M,0.06,N,0.12,K,D*31
```

invalid GPRMC

```
$GPRMC,142307.497,V,,,,,0.00,0.00,110415,,,N*44
```