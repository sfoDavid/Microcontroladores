#include "i2c.h"
#include "uart.h"
#include "stm32f1xx.h"
#include <stdint.h>
#include <stdbool.h>
#include "delay.h"
#include "aht20.h"

void AHT20_Init(void) {
    uint8_t cmd[3];

    print_uart("Passo 1: I2C_Init finalizado\r\n");
    I2C_Init();                  // já foi feito antes no main, pode pular

    print_uart("Passo 2: Enviando soft reset...\r\n");
    cmd[0] = AHT20CMD_SOFTRESET;
    I2C_Write(AHT20_I2C_ADDRESS, cmd, 1);
    delay_ms(20);
    print_uart("Passo 3: Soft reset enviado\r\n");

    print_uart("Passo 4: Enviando init command...\r\n");
    cmd[0] = 0xBE; cmd[1] = 0x08; cmd[2] = 0x00;
    I2C_Write(AHT20_I2C_ADDRESS, cmd, 3);
    delay_ms(10);
    print_uart("Passo 5: Init command enviado\r\n");

    print_uart("Passo 6: Init completo\r\n");
}




void AHT20_ReadTemperatureHumidity(uint32_t* temperature, uint32_t* humidity) {
    uint8_t cmd[3] = { AHT20CMD_TRIGGER, 0x33, 0x00 };
    uint8_t data[6];

    print_uart("Iniciando medição...\r\n");
    I2C_Write(AHT20_I2C_ADDRESS, cmd, 3);
    delay_ms(80);  // Espera mínima sugerida

    // Verificar se o sensor está pronto (bit 7 == 0)
    do {
        I2C_Read(AHT20_I2C_ADDRESS, data, 1);
    } while (data[0] & 0x80);  // Espera o bit de busy (bit 7) ficar zero

    I2C_Read(AHT20_I2C_ADDRESS, data, 6);  // Agora lê os 6 bytes

    uint32_t rawHumidity = ((uint32_t)data[1] << 16 | (uint32_t)data[2] << 8 | data[3]) >> 4;
    uint32_t rawTemp = (((uint32_t)(data[3] & 0x0F)) << 16) | ((uint32_t)data[4] << 8) | data[5];

    uart_print_int("RAW_HUM", rawHumidity);
    uart_print_int("RAW_TEMP", rawTemp);

    *humidity = rawHumidity;
    *temperature = rawTemp;

    print_uart("Medição finalizada\r\n");
}
