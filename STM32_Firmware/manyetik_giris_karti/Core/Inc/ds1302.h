/*
 * ds1302.h
 *
 *  Created on: Dec 29, 2025
 *      Author: CERENSULTANÇETİN
 */

#ifndef INC_DS1302_H_
#define INC_DS1302_H_

#include "main.h"


// Saat yapisi
typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dayofweek;
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
} DS1302_Time;

// Fonksiyon Prototipleri
void DS1302_Init(void);
void DS1302_Write(uint8_t address, uint8_t data);
uint8_t DS1302_Read(uint8_t address);
void DS1302_SetTime(uint8_t hour, uint8_t min, uint8_t sec, uint8_t day, uint8_t month, uint8_t year, uint8_t dow);
void DS1302_GetTime(DS1302_Time *time);

#endif /* INC_DS1302_H_ */
