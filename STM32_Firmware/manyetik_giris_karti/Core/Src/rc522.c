/*
 * rc552.c
 *
 *  Created on: Dec 29, 2025
 *      Author: CERENSULTANÇETİN
 */


#include "rc522.h"


extern SPI_HandleTypeDef hspi1; // main.c icindeki hspi1'i kullan

uint8_t MFRC522_str[16];

void MFRC522_WriteRegister(uint8_t addr, uint8_t val) {
	uint8_t addr_val[2];
	addr_val[0] = (addr << 1) & 0x7E;
	addr_val[1] = val;

	HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_RESET); // CS Low
	HAL_SPI_Transmit(&hspi1, addr_val, 2, 100);
	HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_SET);   // CS High
}

uint8_t MFRC522_ReadRegister(uint8_t addr) {
	uint8_t val;
	uint8_t addr_val = ((addr << 1) & 0x7E) | 0x80;

	HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_RESET); // CS Low
	HAL_SPI_Transmit(&hspi1, &addr_val, 1, 100);
	HAL_SPI_Receive(&hspi1, &val, 1, 100);
	HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_SET);   // CS High
	return val;
}

void MFRC522_SetBitMask(uint8_t reg, uint8_t mask) {
	MFRC522_WriteRegister(reg, MFRC522_ReadRegister(reg) | mask);
}

void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask) {
	MFRC522_WriteRegister(reg, MFRC522_ReadRegister(reg) & (~mask));
}

void MFRC522_AntennaOn(void) {
	uint8_t temp;
	temp = MFRC522_ReadRegister(MFRC522_REG_TX_CONTROL);
	if (!(temp & 0x03)) MFRC522_SetBitMask(MFRC522_REG_TX_CONTROL, 0x03);
}

void MFRC522_AntennaOff(void) {
	MFRC522_ClearBitMask(MFRC522_REG_TX_CONTROL, 0x03);
}

void MFRC522_Reset(void) {
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, MFRC522_CMD_SOFT_RESET);
}

void MFRC522_Init(void) {
	HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RC522_RST_GPIO_Port, RC522_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(RC522_RST_GPIO_Port, RC522_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	MFRC522_Reset();
	MFRC522_WriteRegister(MFRC522_REG_T_MODE, 0x8D);
	MFRC522_WriteRegister(MFRC522_REG_T_PRESCALER, 0x3E);
	MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_L, 30);
	MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_H, 0);
	MFRC522_WriteRegister(MFRC522_REG_TX_ASK, 0x40);
	MFRC522_WriteRegister(MFRC522_REG_MODE, 0x3D);
	MFRC522_AntennaOn();
}

uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint16_t *backLen) {
	uint8_t status = MI_ERR;
	uint8_t irqEn = 0x00;
	uint8_t waitIRq = 0x00;
	uint8_t lastBits;
	uint8_t n;
	uint16_t i;

	switch (command) {
		case MFRC522_CMD_MF_AUTHENT:
			irqEn = 0x12; waitIRq = 0x10; break;
		case MFRC522_CMD_TRANSCEIVE:
			irqEn = 0x77; waitIRq = 0x30; break;
		default:
			break;
	}

	MFRC522_WriteRegister(MFRC522_REG_COMM_IEN, irqEn | 0x80);
	MFRC522_ClearBitMask(MFRC522_REG_COMM_IRQ, 0x80);
	MFRC522_SetBitMask(MFRC522_REG_FIFO_LEVEL, 0x80);
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, MFRC522_CMD_IDLE);

	for (i = 0; i < sendLen; i++) MFRC522_WriteRegister(MFRC522_REG_FIFO_DATA, sendData[i]);
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, command);
	if (command == MFRC522_CMD_TRANSCEIVE) MFRC522_SetBitMask(MFRC522_REG_BIT_FRAMING, 0x80);

	i = 2000;
	do {
		n = MFRC522_ReadRegister(MFRC522_REG_COMM_IRQ);
		i--;
	} while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

	MFRC522_ClearBitMask(MFRC522_REG_BIT_FRAMING, 0x80);

	if (i != 0) {
		if (!(MFRC522_ReadRegister(MFRC522_REG_ERROR) & 0x1B)) {
			status = MI_OK;
			if (n & irqEn & 0x01) status = MI_NOTAGERR;
			if (command == MFRC522_CMD_TRANSCEIVE) {
				n = MFRC522_ReadRegister(MFRC522_REG_FIFO_LEVEL);
				lastBits = MFRC522_ReadRegister(MFRC522_REG_CONTROL) & 0x07;
				if (lastBits) *backLen = (n - 1) * 8 + lastBits; else *backLen = n * 8;
				if (n == 0) n = 1;
				if (n > 16) n = 16;
				for (i = 0; i < n; i++) backData[i] = MFRC522_ReadRegister(MFRC522_REG_FIFO_DATA);
			}
		} else status = MI_ERR;
	}
	return status;
}

uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType) {
	uint8_t status;
	uint16_t backBits;
	MFRC522_WriteRegister(MFRC522_REG_BIT_FRAMING, 0x07);
	TagType[0] = reqMode;
	status = MFRC522_ToCard(MFRC522_CMD_TRANSCEIVE, TagType, 1, TagType, &backBits);
	if ((status != MI_OK) || (backBits != 0x10)) status = MI_ERR;
	return status;
}

uint8_t MFRC522_Anticoll(uint8_t *serNum) {
	uint8_t status;
	uint8_t i;
	uint8_t serNumCheck = 0;
	uint16_t unLen;

	MFRC522_WriteRegister(MFRC522_REG_BIT_FRAMING, 0x00);
	serNum[0] = 0x93;
	serNum[1] = 0x20;
	status = MFRC522_ToCard(MFRC522_CMD_TRANSCEIVE, serNum, 2, serNum, &unLen);

	if (status == MI_OK) {
		for (i = 0; i < 4; i++) serNumCheck ^= serNum[i];
		if (serNumCheck != serNum[4]) status = MI_ERR;
	}
	return status;
}

uint8_t MFRC522_Check(uint8_t *id) {
	uint8_t status;
	status = MFRC522_Request(0x26, id); // 0x26 = PICC_REQIDL
	if (status == MI_OK) status = MFRC522_Anticoll(id);
	MFRC522_Halt();
	return status;
}

void MFRC522_Halt(void) {
	uint16_t unLen;
	uint8_t buff[4];
	buff[0] = 0x50;
	buff[1] = 0x00;
	MFRC522_CalculateCRC(buff, 2, &buff[2]);
	MFRC522_ToCard(MFRC522_CMD_TRANSCEIVE, buff, 4, buff, &unLen);
}

void MFRC522_CalculateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData) {
	uint8_t i, n;
	MFRC522_ClearBitMask(MFRC522_REG_DIV_IRQ, 0x04);
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, MFRC522_CMD_IDLE);
	MFRC522_WriteRegister(MFRC522_REG_FIFO_LEVEL, 0x80);
	for (i = 0; i < len; i++) MFRC522_WriteRegister(MFRC522_REG_FIFO_DATA, *(pIndata + i));
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, MFRC522_CMD_CALC_CRC);
	i = 0xFF;
	do {
		n = MFRC522_ReadRegister(MFRC522_REG_DIV_IRQ);
		i--;
	} while ((i != 0) && !(n & 0x04));
	pOutData[0] = MFRC522_ReadRegister(MFRC522_REG_CRC_RESULT_L);
	pOutData[1] = MFRC522_ReadRegister(MFRC522_REG_CRC_RESULT_M);
}
