#include "adc_ldr.h"
#include "stm32f1xx.h"

void ADC_LDR_Init(void) {
    // Enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Configure PA0 as analog input
    GPIOA->CRL &= ~GPIO_CRL_CNF0;
    GPIOA->CRL &= ~GPIO_CRL_MODE0;

    // Enable ADC1 clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Set software trigger
    ADC1->CR2 |= (7UL << ADC_CR2_EXTSEL_Pos);

    // Power up the ADC
    ADC1->CR2 |= ADC_CR2_ADON;

    // Short delay to stabilize
    for (volatile int i = 0; i < 10000; i++);
}

uint16_t ADC_LDR_Read(void) {
    // Relaunch the ADC
    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // Wait for conversion to complete
    while (!(ADC1->SR & ADC_SR_EOC));

    return ADC1->DR;
}
