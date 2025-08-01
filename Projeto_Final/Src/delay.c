/*
 * delay.c
 *
 *  Created on: Jul 19, 2025
 *      Author: david
 */


#include "stm32f1xx.h"

volatile uint32_t delay_counter = 0;

void delay_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Habilita clock para TIM2

    TIM2->PSC = 7200 - 1;     // Prescaler: 72 MHz / 7200 = 10 kHz
    TIM2->ARR = 10 - 1;       // 10 kHz / 10 = 1 kHz → 1 ms por interrupção

    TIM2->DIER |= TIM_DIER_UIE;    // Habilita interrupção de atualização
    TIM2->CR1 |= TIM_CR1_CEN;      // Ativa o timer
    NVIC_EnableIRQ(TIM2_IRQn);     // Habilita interrupção global
}
// Handler da interrupção do TIM2
void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF; // Limpa flag de interrupção

        if (delay_counter > 0)
            delay_counter--;
    }
}
void delay_ms(uint32_t ms)
{
    delay_counter = ms;
    while (delay_counter > 0); // Espera até o contador zerar
}


void delay_ms_polling(uint32_t ms)
{
    // Aproximadamente 1 ms = 72000 ciclos @72MHz
    // Ajuste se necessário (pode variar com otimizações do compilador)
    for (uint32_t i = 0; i < ms * 7200; i++)
    {
        __asm volatile("nop");
    }
}


