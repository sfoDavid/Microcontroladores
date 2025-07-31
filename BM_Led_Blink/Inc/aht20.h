#ifndef AHT20_H_
#define AHT20_H_

#include "stm32f1xx.h"
#include "i2c.h"       // Biblioteca de I2C em modo polling
#include "delay.h"     // delay_ms()
#include <stdint.h>
#include <stdbool.h>

#define AHT20_I2C_ADDRESS          0x38

// Comandos AHT20
#define AHT20CMD_SOFTRESET        0xBA
#define AHT20_CMD_INIT             0xBE
#define AHT20CMD_TRIGGER 		 0xAC

// Funções principais
void AHT20_Init(void);
void AHT20_ReadTemperatureHumidity(float *temperature, float *humidity);

#endif /* AHT20_H_ */
