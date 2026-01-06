/*
 * ds1302.c
 *
 *  Created on: Dec 29, 2025
 *      Author: CERENSULTANÇETİN
 */

#include "ds1302.h"

// Pin Tanimlamalari (main.h icinden gelir)
// DS1302_RST_Pin, DS1302_CLK_Pin, DS1302_DAT_Pin

// Yardimci Fonksiyonlar
void write_byte(uint8_t byte) {
	for (int i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(DS1302_DAT_GPIO_Port, DS1302_DAT_Pin, (byte & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DS1302_CLK_GPIO_Port, DS1302_CLK_Pin, GPIO_PIN_SET); // Clock High
		HAL_GPIO_WritePin(DS1302_CLK_GPIO_Port, DS1302_CLK_Pin, GPIO_PIN_RESET); // Clock Low
		byte >>= 1;
	}
}

uint8_t read_byte(void) {
	uint8_t byte = 0;
	for (int i = 0; i < 8; i++) {
		byte >>= 1;
		if (HAL_GPIO_ReadPin(DS1302_DAT_GPIO_Port, DS1302_DAT_Pin) == GPIO_PIN_SET) {
			byte |= 0x80;
		}
		HAL_GPIO_WritePin(DS1302_CLK_GPIO_Port, DS1302_CLK_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DS1302_CLK_GPIO_Port, DS1302_CLK_Pin, GPIO_PIN_RESET);
	}
	return byte;
}

// Baslatma
void DS1302_Init(void) {
	HAL_GPIO_WritePin(DS1302_RST_GPIO_Port, DS1302_RST_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DS1302_CLK_GPIO_Port, DS1302_CLK_Pin, GPIO_PIN_RESET);
}

// Tek Bayt Yazma
void DS1302_Write(uint8_t address, uint8_t data) {
	// Pin Yonunu Output Yap
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS1302_DAT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DS1302_DAT_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(DS1302_RST_GPIO_Port, DS1302_RST_Pin, GPIO_PIN_SET);
	write_byte(address);
	write_byte(data);
	HAL_GPIO_WritePin(DS1302_RST_GPIO_Port, DS1302_RST_Pin, GPIO_PIN_RESET);
}

// Tek Bayt Okuma
uint8_t DS1302_Read(uint8_t address) {
	uint8_t data;

	// Pin Yonunu Output Yap (Adres yazmak icin)
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS1302_DAT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DS1302_DAT_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(DS1302_RST_GPIO_Port, DS1302_RST_Pin, GPIO_PIN_SET);
	write_byte(address);

	// Pin Yonunu Input Yap (Veri okumak icin)
	GPIO_InitStruct.Pin = DS1302_DAT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DS1302_DAT_GPIO_Port, &GPIO_InitStruct);

	data = read_byte();
	HAL_GPIO_WritePin(DS1302_RST_GPIO_Port, DS1302_RST_Pin, GPIO_PIN_RESET);

	return data;
}

// Zamani Ayarla
void DS1302_SetTime(uint8_t hour, uint8_t min, uint8_t sec, uint8_t day, uint8_t month, uint8_t year, uint8_t dow) {
	DS1302_Write(0x8E, 0x00); // Yazma korumasini kaldir
	DS1302_Write(0x80, ((sec / 10) << 4) | (sec % 10));
	DS1302_Write(0x82, ((min / 10) << 4) | (min % 10));
	DS1302_Write(0x84, ((hour / 10) << 4) | (hour % 10));
	DS1302_Write(0x86, ((day / 10) << 4) | (day % 10));
	DS1302_Write(0x88, ((month / 10) << 4) | (month % 10));
	DS1302_Write(0x8A, dow);
	DS1302_Write(0x8C, ((year / 10) << 4) | (year % 10));
	DS1302_Write(0x8E, 0x80); // Yazma korumasini ac
}

// Zamani Oku
void DS1302_GetTime(DS1302_Time *time) {
	uint8_t temp;

	temp = DS1302_Read(0x81);
	time->seconds = ((temp >> 4) * 10) + (temp & 0x0F);

	temp = DS1302_Read(0x83);
	time->minutes = ((temp >> 4) * 10) + (temp & 0x0F);

	temp = DS1302_Read(0x85);
	time->hour = ((temp >> 4) * 10) + (temp & 0x0F);

	temp = DS1302_Read(0x87);
	time->dayofmonth = ((temp >> 4) * 10) + (temp & 0x0F);

	temp = DS1302_Read(0x89);
	time->month = ((temp >> 4) * 10) + (temp & 0x0F);

	time->dayofweek = DS1302_Read(0x8B);

	temp = DS1302_Read(0x8D);
	time->year = ((temp >> 4) * 10) + (temp & 0x0F);
}
