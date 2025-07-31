#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include "stm32f103xb.h"
// Protótipos das funções
void gpio_init(void);
void delay_ms(uint32_t ms);
void blink_led(uint8_t count, uint16_t duration);

#endif
