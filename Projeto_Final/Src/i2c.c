/*
 * i2c.c
 *
 *  Created on: Jul 23, 2025
 *      Author: david
 */

#include "stm32f1xx.h"
#include "uart.h"
#include "i2c.h"

#define debugON


void I2C_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
    GPIOB->CRL |= (GPIO_CRL_MODE6_1 | GPIO_CRL_CNF6 | GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7);

    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    I2C1->CR2 = 8;
    I2C1->CCR = 40;
    I2C1->TRISE = 9;
    I2C1->CR1 |= I2C_CR1_PE;

#ifdef debugON
    print_uart("I2C_Init OK\r\n");
#endif
}

int I2C_Start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    for (uint32_t t = 10000; t; t--) {
        if (I2C1->SR1 & I2C_SR1_SB) {
#ifdef debugON
            print_uart("I2C_Start OK\r\n");
#endif
            return 0;
        }
    }
#ifdef debugON
    print_uart("Erro: Timeout em I2C_Start\r\n");
#endif
    return -1;
}

int I2C_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
    for (uint32_t t = 10000; t; t--) {
        if (!(I2C1->CR1 & I2C_CR1_STOP)) {
#ifdef debugON
            print_uart("I2C_Stop OK\r\n");
#endif
            return 0;
        }
    }
#ifdef debugON
    print_uart("Erro: Timeout em I2C_Stop\r\n");
#endif
    return -1;
}

void I2C_Write(uint8_t addr, uint8_t *data, uint16_t size) {
    if (I2C_Start() != 0) return;

    I2C1->DR = I2C_WRITE_ADDR(addr);
    print_uart("Esperando ACK do endereço...\r\n");
    for (uint32_t t = 10000; t; t--) {
        if (I2C1->SR1 & I2C_SR1_ADDR) break;
        if (I2C1->SR1 & I2C_SR1_AF) {
#ifdef debugON
            print_uart("Erro: Endereço NACK em I2C_Write\r\n");
#endif
            I2C1->SR1 &= ~I2C_SR1_AF;
            I2C_Stop();
            return;
        }
    }
    (void)I2C1->SR1;
    (void)I2C1->SR2;

    while (size--) {
        I2C1->DR = *data++;
        for (uint32_t t = 10000; t; t--) {
            if (I2C1->SR1 & I2C_SR1_BTF) break;
        }
    }

    I2C_Stop();
#ifdef debugON
    print_uart("I2C_Write OK\r\n");
#endif
}

void I2C_Read(uint8_t addr, uint8_t *data, uint16_t size) {
    if (!size) return;

    if (I2C_Start() != 0) return;
    I2C1->DR = I2C_READ_ADDR(addr);

    for (uint32_t t = 10000; t; t--) {
        if (I2C1->SR1 & I2C_SR1_ADDR) break;
        if (I2C1->SR1 & I2C_SR1_AF) {
#ifdef debugON
            print_uart("Erro: Endereço NACK em I2C_Read\r\n");
#endif
            I2C1->SR1 &= ~I2C_SR1_AF;
            I2C_Stop();
            return;
        }
    }

    (void)I2C1->SR1;
    (void)I2C1->SR2;

    if (size == 1) {
        I2C1->CR1 &= ~I2C_CR1_ACK;
        I2C_Stop();
    } else {
        I2C1->CR1 |= I2C_CR1_ACK;
    }

    while (size--) {
        for (uint32_t t = 10000; t; t--) {
            if (I2C1->SR1 & I2C_SR1_RXNE) break;
        }

        *data++ = I2C1->DR;

        if (size == 1) {
            I2C1->CR1 &= ~I2C_CR1_ACK;
            I2C_Stop();
        }
    }
#ifdef debugON
    print_uart("I2C_Read OK\r\n");
#endif
}

uint8_t I2C_IsDeviceReady(uint8_t addr, uint32_t trials) {
    while (trials--) {
        if (I2C_Start() != 0) continue;

        I2C1->DR = I2C_WRITE_ADDR(addr);
        for (uint32_t t = 10000; t; t--) {
            if (I2C1->SR1 & I2C_SR1_ADDR) {
                (void)I2C1->SR1; (void)I2C1->SR2;
                I2C_Stop();
#ifdef debugON
                print_uart("Dispositivo pronto (ACK)\r\n");
#endif
                return 1;
            }

            if (I2C1->SR1 & I2C_SR1_AF) {
                I2C1->SR1 &= ~I2C_SR1_AF;
                break;
            }
        }

        I2C_Stop();
    }

#ifdef debugON
    print_uart("Dispositivo NÃO pronto após tentativas\r\n");
#endif
    return 0;
}
