/*
 * delay.h
 *
 *  Created on: Jul 19, 2025
 *      Author: david
 */

#ifndef DELAY_H_
#define DELAY_H_


#include "stm32f1xx.h"

void delay_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void delay_ms_polling(uint32_t ms);



#endif /* DELAY_H_ */
