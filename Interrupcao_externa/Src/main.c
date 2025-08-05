#include <stdint.h>
#include "stm32f1xx.h"

// Handler do botão PB0 → LED PC13
void EXTI0_IRQHandler(void) {
    GPIOC->ODR ^= (1 << 13);        // Inverte LED PC13
    EXTI->PR |= (1 << 0);           // Limpa flag de interrupção
}

// Handler do botão PB1 → LED PC14
void EXTI1_IRQHandler(void) {
    GPIOC->ODR ^= (1 << 14);        // Inverte LED PC14
    EXTI->PR |= (1 << 1);           // Limpa flag de interrupção
}

// Handler do botão PB10 → LED PC15
void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1 << 10)) {
        GPIOC->ODR ^= (1 << 15);    // Inverte LED PC15
        EXTI->PR |= (1 << 10);      // Limpa flag de interrupção
    }
}

int main(void) {
    //Habilita clocks dos GPIOs A, B, C e do AFIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |
                    RCC_APB2ENR_IOPBEN |
                    RCC_APB2ENR_IOPCEN |
                    RCC_APB2ENR_AFIOEN;

    //Configura PB0 e PB1 como entrada com pull-up (pino low = pressionado)
    GPIOB->CRL &= ~((0xF << (0 * 4)) | (0xF << (1 * 4)));
    GPIOB->CRL |=  ((0x8 << (0 * 4)) | (0x8 << (1 * 4)));

    //Configura PB10 como entrada com pull-up
    GPIOB->CRH &= ~(0xF << (2 * 4));  // 10 = bit 2 em CRH
    GPIOB->CRH |=  (0x8 << (2 * 4));

    //Ativa pull-ups
    GPIOB->ODR |= (1 << 0) | (1 << 1) | (1 << 10);

    //Configura PC13, PC14, PC15 como saída push-pull 2MHz
    GPIOC->CRH &= ~(0xF << (5 * 4) | 0xF << (6 * 4) | 0xF << (7 * 4));
    GPIOC->CRH |=  (0x2 << (5 * 4) | 0x2 << (6 * 4) | 0x2 << (7 * 4));

    //Inicialmente apaga todos os LEDs
    GPIOC->ODR &= ~(1 << 13 | 1 << 14 | 1 << 15);

    //Mapeia EXTI0 e EXTI1 para PB0 e PB1
    AFIO->EXTICR[0] &= ~((0xF << 0) | (0xF << 4));
    AFIO->EXTICR[0] |=  (0x1 << 0) | (0x1 << 4);  // 0x1 = porta B

    //Mapeia EXTI10 para PB10
    AFIO->EXTICR[2] &= ~(0xF << 8);    // EXTI10 → posição 8 bits
    AFIO->EXTICR[2] |=  (0x1 << 8);

    //Configura disparo por borda de descida (botão pressionado)
    EXTI->FTSR |= (1 << 0) | (1 << 1) | (1 << 10);
    EXTI->IMR  |= (1 << 0) | (1 << 1) | (1 << 10);

    //Habilita as interrupções no NVIC com prioridade
    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_SetPriority(EXTI1_IRQn, 1);
    NVIC_SetPriority(EXTI15_10_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    //Ativa as interrupções globais
    __enable_irq();

    while (1) {
        // Loop principal vazio — tudo tratado nas interrupções
    }
}




