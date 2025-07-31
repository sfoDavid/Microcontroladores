// adc_ldr.h
#ifndef ADC_LDR_H
#define ADC_LDR_H

#include <stdint.h>

// Inicializa o ADC e configura o pino do LDR (PA0)
void ADC_LDR_Init(void);

// Faz uma leitura do ADC e retorna o valor (0 a 4095)
uint16_t ADC_LDR_Read(void);

#endif // ADC_LDR_H
