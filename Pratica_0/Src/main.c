#include "stm32f1xx.h"

volatile uint8_t toggle = 0;

void clock_prescaler_config(void) {
    // Reduz o clock AHB para 1/8
    RCC->CFGR |= RCC_CFGR_HPRE_DIV8;
}

void SysTick_Init(void) {
    SysTick->LOAD = 100000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}

int main(void)
{
    clock_prescaler_config();
    SysTick_Init();

    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // Configura PC13 como saída push-pull
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |=  (0x2 << 20);

    // LED começa ligado
    GPIOC->ODR &= ~(1 << 13);

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk; // Garante modo SLEEP

    while (1)
    {
        if (toggle)
        {
            GPIOC->ODR ^= (1 << 13); // Inverte o estado do LED
            toggle = 0;
        }

        __WFI(); // Dorme até a próxima interrupção do SysTick
    }
}

// Interrupção do SysTick
void SysTick_Handler(void)
{
    toggle = 1;
}
