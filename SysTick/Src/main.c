#include <stdint.h>
#include "stm32f1xx.h"

// Define um tipo enum para as cores do LED
typedef enum {
    APAGADO,
    VERMELHO,
    VERDE,
    AZUL,
    AMARELO,
    CIANO,
    ROXO,
    BRANCO
} Cor;

// Variável para armazenar o estado atual das cores
Cor SysTick_TASK = APAGADO;

// Macros para controle de pinos
#define LED_VERMELHO   (1 << 0)   // PC13
#define LED_VERDE      (1 << 1)   // PC14
#define LED_AZUL       (1 << 10)  // PC15

// Função para configurar a interrupção do SysTick
void SysTick_Handler(void)
{
    SysTick_TASK++;  // Incrementa a cor
    if (SysTick_TASK > BRANCO) {
        SysTick_TASK = APAGADO;  // Reseta a contagem de cores
    }
}

// Função para acionar as cores do LED RGB
void Set_LED_Color(Cor color)
{
    // Apaga todos os LEDs antes de configurar a nova cor
    GPIOB->ODR &= ~(LED_VERMELHO | LED_VERDE | LED_AZUL);

    // Atribui as cores de acordo com o valor de 'color'
    switch(color)
    {
        case VERMELHO:
            GPIOB->ODR |= LED_VERMELHO; // Acende o LED Vermelho
            break;
        case VERDE:
            GPIOB->ODR |= LED_VERDE;    // Acende o LED Verde
            break;
        case AZUL:
            GPIOB->ODR |= LED_AZUL;     // Acende o LED Azul
            break;
        case AMARELO:
            GPIOB->ODR |= (LED_VERMELHO | LED_VERDE); // Acende Vermelho e Verde (Amarelo)
            break;
        case CIANO:
            GPIOB->ODR |= (LED_VERDE | LED_AZUL);    // Acende Verde e Azul (Ciano)
            break;
        case ROXO:
            GPIOB->ODR |= (LED_VERMELHO | LED_AZUL); // Acende Vermelho e Azul (Roxo)
            break;
        case BRANCO:
            GPIOB->ODR |= (LED_VERMELHO | LED_VERDE | LED_AZUL); // Acende todos os LEDs (Branco)
            break;
        case APAGADO:
        default:
            // Não acende nenhum LED
            break;
    }
}

int main(void)
{
    // Habilita o clock para o GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // Configura os pinos PB0, PB1 e PB10 como saída
    GPIOB->CRL &= 0xFFFFFF00;
    GPIOB->CRH &= 0xFFFFF0FF;
    GPIOB->CRL |= 0x00000022;
    GPIOB->CRH |= 0x00000200;

    // Inicialmente apaga todos os LEDs
    Set_LED_Color(APAGADO);

    // Configuração do SysTick para gerar interrupções a cada 1 segundo
    uint32_t ticks = 1000000;  // 1 segundo (aproximadamente)
    if ((ticks - 1UL) <= 0xFFFFFFUL)
    {
        SysTick->LOAD  = (uint32_t)(ticks - 1UL);
        SysTick->VAL   = 0UL;  // Carrega o valor do contador SysTick
        SysTick->CTRL  = (0 << 2) |  // SysTick_Clock = AHB/8
                         (1 << 1) |  // Habilita a IRQ e o Timer SysTick
                         (1 << 0);   // Habilita o SysTick
    }

    while (1)
    {
        // O loop principal fica vazio, pois a troca de cores é feita na interrupção
        // O SysTick_Handler alterna as cores do LED a cada segundo
        switch (SysTick_TASK) {
            case APAGADO:
                Set_LED_Color(APAGADO);
                break;
            case VERMELHO:
                Set_LED_Color(VERMELHO);
                break;
            case VERDE:
                Set_LED_Color(VERDE);
                break;
            case AZUL:
                Set_LED_Color(AZUL);
                break;
            case AMARELO:
                Set_LED_Color(AMARELO);
                break;
            case CIANO:
                Set_LED_Color(CIANO);
                break;
            case ROXO:
                Set_LED_Color(ROXO);
                break;
            case BRANCO:
                Set_LED_Color(BRANCO);
                break;
            default:
                SysTick_TASK = APAGADO;  // Reseta para APAGADO se houver um erro
                break;
        }
    }
}
