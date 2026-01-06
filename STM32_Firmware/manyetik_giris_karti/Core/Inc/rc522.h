/*
 * rc552.h
 *
 *  Created on: Dec 29, 2025
 *      Author: CERENSULTANÇETİN
 */

#ifndef INC_RC522_H_
#define INC_RC522_H_

#include "main.h"

// MFRC522 Register Adresleri
#define MFRC522_REG_COMMAND             0x01
#define MFRC522_REG_COMM_IEN            0x02
#define MFRC522_REG_DIV_IEN             0x03
#define MFRC522_REG_COMM_IRQ            0x04
#define MFRC522_REG_DIV_IRQ             0x05
#define MFRC522_REG_ERROR               0x06
#define MFRC522_REG_STATUS1             0x07
#define MFRC522_REG_STATUS2             0x08
#define MFRC522_REG_FIFO_DATA           0x09
#define MFRC522_REG_FIFO_LEVEL          0x0A
#define MFRC522_REG_WATER_LEVEL         0x0B
#define MFRC522_REG_CONTROL             0x0C
#define MFRC522_REG_BIT_FRAMING         0x0D
#define MFRC522_REG_COLL                0x0E
#define MFRC522_REG_MODE                0x11
#define MFRC522_REG_TX_MODE             0x12
#define MFRC522_REG_RX_MODE             0x13
#define MFRC522_REG_TX_CONTROL          0x14
#define MFRC522_REG_TX_ASK              0x15
#define MFRC522_REG_TX_SEL              0x16
#define MFRC522_REG_RX_SEL              0x17
#define MFRC522_REG_RX_THRESHOLD        0x18
#define MFRC522_REG_DEMOD               0x19
#define MFRC522_REG_MF_TX               0x1C
#define MFRC522_REG_MF_RX               0x1D
#define MFRC522_REG_SERIAL_SPEED        0x1F
#define MFRC522_REG_CRC_RESULT_M        0x21
#define MFRC522_REG_CRC_RESULT_L        0x22
#define MFRC522_REG_MOD_WIDTH           0x24
#define MFRC522_REG_RF_CFG              0x26
#define MFRC522_REG_GS_N                0x27
#define MFRC522_REG_CW_GS_P             0x28
#define MFRC522_REG_MOD_GS_P            0x29
#define MFRC522_REG_T_MODE              0x2A
#define MFRC522_REG_T_PRESCALER         0x2B
#define MFRC522_REG_T_RELOAD_H          0x2C
#define MFRC522_REG_T_RELOAD_L          0x2D
#define MFRC522_REG_T_COUNTER_VALUE_H   0x2E
#define MFRC522_REG_T_COUNTER_VALUE_L   0x2F
#define MFRC522_REG_TEST_SEL1           0x31
#define MFRC522_REG_TEST_SEL2           0x32
#define MFRC522_REG_TEST_PIN_EN         0x33
#define MFRC522_REG_TEST_PIN_VALUE      0x34
#define MFRC522_REG_TEST_BUS            0x35
#define MFRC522_REG_AUTO_TEST           0x36
#define MFRC522_REG_VERSION             0x37
#define MFRC522_REG_ANALOG_TEST         0x38
#define MFRC522_REG_TEST_DAC1           0x39
#define MFRC522_REG_TEST_DAC2           0x3A
#define MFRC522_REG_TEST_ADC            0x3B

// Komutlar
#define MFRC522_CMD_IDLE                0x00
#define MFRC522_CMD_MEM                 0x01
#define MFRC522_CMD_GEN_RANDOM_ID       0x02
#define MFRC522_CMD_CALC_CRC            0x03
#define MFRC522_CMD_TRANSMIT            0x04
#define MFRC522_CMD_NO_CMD_CHANGE       0x07
#define MFRC522_CMD_RECEIVE             0x08
#define MFRC522_CMD_TRANSCEIVE          0x0C
#define MFRC522_CMD_MF_AUTHENT          0x0E
#define MFRC522_CMD_SOFT_RESET          0x0F

// Durumlar
#define MI_OK                           0
#define MI_NOTAGERR                     1
#define MI_ERR                          2

// Kart ID Dizisi (UID genelde 4 veya 7 byte olur)
extern uint8_t MFRC522_str[16];

// Fonksiyonlar
void MFRC522_Init(void);
uint8_t MFRC522_Check(uint8_t *id);
uint8_t MFRC522_Compare(uint8_t *CardID, uint8_t *CompareID);
void MFRC522_WriteRegister(uint8_t addr, uint8_t val);
uint8_t MFRC522_ReadRegister(uint8_t addr);
void MFRC522_SetBitMask(uint8_t reg, uint8_t mask);
void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask);
void MFRC522_AntennaOn(void);
void MFRC522_AntennaOff(void);
void MFRC522_Reset(void);
uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType);
uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint16_t *backLen);
uint8_t MFRC522_Anticoll(uint8_t *serNum);
uint8_t MFRC522_SelectTag(uint8_t *serNum);
uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum);
uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t *recvData);
uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t *writeData);
void MFRC522_Halt(void);
void MFRC522_CalculateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData);

#endif /* INC_RC522_H_ */
