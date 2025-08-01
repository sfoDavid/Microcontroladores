#ifndef NRF24_H
#define NRF24_H

#include "stm32f1xx.h"

// SPI commands
#define NRF24_CMD_W_REGISTER     0x20
#define NRF24_CMD_R_REGISTER     0x00
#define NRF24_CMD_W_TX_PAYLOAD   0xA0
#define NRF24_CMD_R_RX_PAYLOAD   0x61
#define NRF24_CMD_FLUSH_TX       0xE1
#define NRF24_CMD_FLUSH_RX       0xE2
#define NRF24_CMD_NOP            0xFF

// Registers
#define CONFIG         0x00
#define EN_AA          0x01
#define SETUP_RETR     0x04
#define RF_CH          0x05
#define RF_SETUP       0x06
#define STATUS         0x07
#define RX_ADDR_P0     0x0A
#define TX_ADDR        0x10
#define RX_PW_P0       0x11
#define FIFO_STATUS    0x17
#define EN_RXADDR      0x02


// Bits
#define MASK_RX_DR     (1 << 6)
#define MASK_TX_DS     (1 << 5)
#define MASK_MAX_RT    (1 << 4)

// Interface pública
void nRF24_Init(void);
uint8_t nRF24_Check(void);
void nRF24_TX_Mode(uint8_t *address, uint8_t channel);
void nRF24_RX_Mode(uint8_t *address, uint8_t channel);
void nRF24_WritePayload(uint8_t *pBuf, uint8_t length);
uint8_t nRF24_ReadPayload(uint8_t *pBuf, uint8_t length);
uint8_t nRF24_Receive(uint8_t *pBuf);
uint8_t nRF24_GetStatus(void);
void nRF24_ClearIRQFlags(void);
void nRF24_SetupRetransmissions(uint8_t retries, uint16_t delay_us);

// Nível baixo
void nRF24_HW_Init(void);
void SPI1_Init(void);
uint8_t nRF24_LL_RW(uint8_t data);
void nRF24_CE_H(void);
void nRF24_CE_L(void);
void nRF24_CSN_H(void);
void nRF24_CSN_L(void);
void nRF24_WriteReg(uint8_t reg, uint8_t value);
uint8_t nRF24_ReadReg(uint8_t reg);
void nRF24_WriteBuf(uint8_t reg, uint8_t *pBuf, uint8_t len);
void nRF24_ReadBuf(uint8_t reg, uint8_t *pBuf, uint8_t len);

#endif
