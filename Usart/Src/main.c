#include "stm32f1xx.h"

#define PWM_FREQ_HZ 1000
#define STEP        1
#define MAX_DUTY    100

d\n
int8_t  dir = 1;
uint8_t color_index = 0;
uint8_t blink_enabled = 0;     // Controle de piscar ligado/desligado
uint32_t blink_delay = 50000;  // Delay para piscar (ajuste conforme clock)
uint32_t blink_counter = 0;
uint8_t led_on = 0;

const uint8_t colors[7][3] = {
    {1, 0, 0},  // Vermelho
    {0, 1, 0},  // Verde
    {0, 0, 1},  // Azul
    {1, 1, 0},  // Amarelo
    {0, 1, 1},  // Ciano
    {1, 0, 1},  // Roxo
    {1, 1, 1}   // Branco
};

void pwm_init(void);
void pwm_timer3_init(void);
void enable_usart(void);
void uart1_write_char(char c);
void uart1_write_string(const char *s);
char uart1_read_char(void);
void uart1_read_string(char *buf, uint16_t max_len);
void timer4_init(void);

int main(void)
{
    pwm_init();
    pwm_timer3_init();
    enable_usart();
    timer4_init();

    char buffer[32];

    uart1_write_string("Digite um comando (rXX, gXX, bXX, d):\r\n");

    while (1) {

        uart1_read_string(buffer, sizeof(buffer));

        if (buffer[0] != '\0') {
            if (buffer[0] == 'd' || buffer[0] == 'D') {
                blink_enabled = !blink_enabled;
                if (!blink_enabled) {
                    TIM2->CCR1 = 0;
                    uart1_write_string("toogle desativado\r\n");
                } else {
                    uart1_write_string("toogle ativado\r\n");
                }
            }
            else if (buffer[0] != '\0' && buffer[1] != '\0' && buffer[2] != '\0') {
                int8_t level = (buffer[1] - '0') * 10 + (buffer[2] - '0');

                switch (buffer[0]) {
                    case 'r':
                    case 'R':
                        TIM2->CCR1 = (TIM2->ARR * level) / 100;
                        blink_enabled = 0;  // Para piscar se estiver ativo
                        break;

                    case 'g':
                    case 'G':
                        TIM3->CCR1 = (TIM3->ARR * level) / 100;
                        blink_enabled = 0;
                        break;

                    case 'b':
                    case 'B':
                        TIM2->CCR2 = (TIM2->ARR * level) / 100;
                        blink_enabled = 0;
                        break;

                    default:
                        uart1_write_string("Comando inválido\r\n");
                        break;
                }
            } else {
                uart1_write_string("Comando errado\r\n");
            }
        }

        uart1_write_string("\r\n");
    }
}


void pwm_init(void)
{
    // TIM2 → PA0, PA1 (CH1, CH2)
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // PA0 e PA1 → AF Push-Pull
    GPIOA->CRL &= ~(0xFF << 0);         // limpa PA0 e PA1
    GPIOA->CRL |=  (0xBB << 0);         // MODE=11, CNF=10

    // Timer base
    uint32_t clk = 8000000;
    uint32_t arr = 1000;
    uint32_t psc = (clk / (PWM_FREQ_HZ * arr)) - 1;
    arr = (clk / (PWM_FREQ_HZ * (psc + 1))) - 1;

    TIM2->PSC = psc;
    TIM2->ARR = arr;

    // PWM CH1 e CH2
    TIM2->CCMR1 = (6 << 4) | TIM_CCMR1_OC1PE | (6 << 12) | TIM_CCMR1_OC2PE;
    TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM2->CCR1 = 0;
    TIM2->CCR2 = 0;

    TIM2->CR1 |= TIM_CR1_ARPE;
    TIM2->EGR |= TIM_EGR_UG;
    TIM2->CR1 |= TIM_CR1_CEN;
}

void pwm_timer3_init(void)
{
    // TIM3 → PA6 (CH1)
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // PA6 como AF Push-Pull
    GPIOA->CRL &= ~(0xF << 24);
    GPIOA->CRL |=  (0xB << 24);

    // Timer base
    uint32_t clk = 8000000;
    uint32_t arr = 1000;
    uint32_t psc = (clk / (PWM_FREQ_HZ * arr)) - 1;
    arr = (clk / (PWM_FREQ_HZ * (psc + 1))) - 1;

    TIM3->PSC = psc;
    TIM3->ARR = arr;

    // PWM CH1
    TIM3->CCMR1 = (6 << 4) | TIM_CCMR1_OC1PE;
    TIM3->CCER |= TIM_CCER_CC1E;
    TIM3->CCR1 = 0;

    TIM3->CR1 |= TIM_CR1_ARPE;
    TIM3->EGR |= TIM_EGR_UG;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void enable_usart(){

	RCC->APB2ENR |= (1 << 14); //Enable usart1

	GPIOA->CRH &= ~(0xF << 4);     // Limpa bits de configuração de PA9
	GPIOA->CRH |= (0xB << 4);      // 1011 = AF output push-pull, 50 MHz

	// PA10 (RX) como input floating
	GPIOA->CRH &= ~(0xF << 8);     // Limpa bits de configuração de PA10
	GPIOA->CRH |= (0x4 << 8);      // 0100 = input floating

	// Baud Rate: 9600 com HSI (8MHz) → USARTDIV ≈ 52.08 → 52.1 → 0x0201
	USART1->BRR = (52 << 4) | 1;
	USART1->CR1 = 0;
	USART1->CR1 |= (0 << 12);    // M = 0 (8 bits)
	USART1->CR2 &= ~(3 << 12);   // STOP = 00 (1 stop bit)
	USART1->CR1 |= (1 << 3);     // TE = 1 (habilita transmissor)
	USART1->CR1 |= (1 << 2);
	USART1->CR1 |= (1 << 13);    // UE = 1 (USART enable)
}

void uart1_write_char(char c) {
    while (!(USART1->SR & (1 << 7))); // Espera TXE (transmit buffer empty)
    USART1->DR = c;
    while (!(USART1->SR & (1 << 6))); // Espera TC (transmission complete)
}

void uart1_write_string(const char *s) {
    while (*s) {
        uart1_write_char(*s++);
    }
}

char uart1_read_char(void) {
    while (!(USART1->SR & USART_SR_RXNE)); // Espera até que dado esteja disponível
    return USART1->DR;                     // Lê o dado recebido
}

void uart1_read_string(char *buf, uint16_t max_len) {
    uint16_t i = 0;
    char c;

    while (i < max_len - 1) {
        c = uart1_read_char();      // Lê um caractere
        if (c == '\n' || c == '\r') // Se for fim de linha, finaliza
            break;

        buf[i++] = c;
        uart1_write_char(c);        // Ecoa o caractere (opcional)
    }

    buf[i] = '\0'; // Finaliza a string
}

void timer4_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    TIM4->PSC = 7999;  // 8 MHz / (7999 + 1) = 1 kHz
    TIM4->ARR = 24;    // 1 kHz / (99 + 1) = 10 Hz → 100 ms

    TIM4->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM4_IRQn);
    TIM4->CR1 |= TIM_CR1_CEN;
}

void TIM4_IRQHandler(void) {
    if (TIM4->SR & TIM_SR_UIF) {
        TIM4->SR &= ~TIM_SR_UIF;  // Limpa flag

        if (blink_enabled) {
            if (led_on) {
                TIM2->CCR1 = 0;
                led_on = 0;
            } else {
                TIM2->CCR1 = TIM2->ARR; // 100% duty
                led_on = 1;
            }
        }
    }
}
