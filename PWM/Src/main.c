  #include <stdint.h>
#include "stm32f1xx.h"

void TIM2_IRQHandler(void);
void configGPIO();
void timer2Config();
void configTimer3();
void updateColorSequence();

volatile uint16_t red = 0, green = 0, blue = 0;
volatile uint8_t colorIndex = 0;
volatile uint8_t increasing = 1;

int main(void){

	configGPIO();
	timer2Config();
	configTimer3();

	while(1)
	{


	}

}


void TIM2_IRQHandler(void) {
	if(TIM2->SR & TIM_SR_UIF){
		TIM2->SR &= ~TIM_SR_UIF; // Limpa a flag de interrupção
		updateColorSequence();

	}

}
void configGPIO(){
	RCC->APB2ENR |= (1 << 2);
	RCC->APB2ENR |= (1 << 3);

	GPIOA->CRL &= 0x00FFFFFF;
	GPIOA->CRL |= 0xBB000000;

	GPIOB->CRL &= 0xFFFFFFF0;
	GPIOB->CRL |= 0x0000000B;
}

void timer2Config() {
    // Habilita o clock para o TIM3
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configura o prescaler e o valor de auto-reload
    TIM2->PSC = 8000-1;
    TIM2->ARR = 20-1; //20ms

    // Habilita a interrupção de atualização do TIM3
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);

    // Ativa o timer
    TIM2->CR1 |= TIM_CR1_CEN;
}

void configTimer3(){
	//Clock AFIO
	RCC->APB2ENR |= (1 << 0);
	//CH1 - A6, CH2 - A7, CH3 - B0
	AFIO->MAPR &= (0x00 << 10);

	//clock TIMER3
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->PSC = 0;
	TIM3->ARR = 100;

	TIM3->CCMR1 = 0x6060;
	TIM3->CCMR2 = 0x0060;
	TIM3->CCER = 0x0111;
	TIM3->CR1  |= (1 << 0);
}

void updateColorSequence() {
    // Controla o ciclo de aumento e diminuição de PWM
    static uint16_t brightness = 0;
    static uint8_t step = 1;

    if (increasing) {
        brightness += step;
        if (brightness >= 100) increasing = 0;
    } else {
        brightness -= step;
        if (brightness == 0) {
            increasing = 1;
            colorIndex = (colorIndex + 1) % 7; // Avança para a próxima cor
        }
    }

    // Define a cor com base no colorIndex
    switch (colorIndex) {
        case 0: // Vermelho
            red = brightness;
            green = 0;
            blue = 0;
            break;
        case 1: // Verde
            red = 0;
            green = brightness;
            blue = 0;
            break;
        case 2: // Azul
            red = 0;
            green = 0;
            blue = brightness;
            break;
        case 3: // Amarelo
            red = brightness;
            green = brightness;
            blue = 0;
            break;
        case 4: // Ciano
            red = 0;
            green = brightness;
            blue = brightness;
            break;
        case 5: // Roxo
            red = brightness;
            green = 0;
            blue = brightness;
            break;
        case 6: // Branco
            red = brightness;
            green = brightness;
            blue = brightness;
            break;
    }

    // Atualiza os valores de PWM
    TIM3->CCR1 = red;   // Canal 1 - Vermelho
    TIM3->CCR2 = green; // Canal 2 - Verde
    TIM3->CCR3 = blue;  // Canal 3 - Azul
}
