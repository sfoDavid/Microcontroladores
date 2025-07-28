/*
 * i2c.h
 *
 *  Created on: Jul 23, 2025
 *      Author: david
 */

#ifndef I2C_H_
#define I2C_H_


#include "stm32f1xx.h"

// Macros para manipulação do endereço I2C
#define I2C_WRITE_ADDR(addr)  ((addr) << 1)       // Endereço de escrita (bit RW = 0)
#define I2C_READ_ADDR(addr)   (((addr) << 1) | 1) // Endereço de leitura (bit RW = 1)

// Inicializa o periférico I2C1 e os pinos PB6 (SCL) e PB7 (SDA)
void I2C_Init(void);

// Gera condição de START no barramento I2C
int I2C_Start(void);

// Gera condição de STOP no barramento I2C
int I2C_Stop(void);

// Envia dados no barramento I2C (modo master transmitter)
void I2C_Write(uint8_t addr, uint8_t *data, uint16_t size);

// Lê dados do barramento I2C (modo master receiver)
void I2C_Read(uint8_t addr, uint8_t *data, uint16_t size);

// Verifica se o dispositivo com o endereço está respondendo (ACK)
uint8_t I2C_IsDeviceReady(uint8_t addr, uint32_t trials);



#endif /* I2C_H_ */
