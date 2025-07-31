#include "stm32f1xx.h"
#include "uart.h"
#include "i2c.h"
#include "aht20.h"
#include "nrf24.h"
#include "delay.h"
#include <string.h>
#include <stdio.h>
#include "adc_ldr.h"

float temperature, humidity;
uint16_t ldr;

void print_nrf_debug(void) {
    uart_print_int("CONFIG", nRF24_ReadReg(CONFIG));
    uart_print_int("EN_AA", nRF24_ReadReg(EN_AA));
    uart_print_int("RF_CH", nRF24_ReadReg(RF_CH));
    uart_print_int("RF_SETUP", nRF24_ReadReg(RF_SETUP));
    uart_print_int("STATUS", nRF24_ReadReg(STATUS));
    uart_print_int("FIFO_STATUS", nRF24_ReadReg(FIFO_STATUS));
}

int main(void) {
    uint8_t tx_address[5] = {0x01, 0xDD, 0xCC, 0xBB, 0xAA};
    uint8_t payload[8];  // 4 bytes para float temperatura + 4 bytes umidade

    UART_Init();
    delay_init();
    SPI1_Init();
    AHT20_Init();
    ADC_LDR_Init();


    print_uart("Inicializando sensor e rádio...\r\n");

    nRF24_Init();
    nRF24_TX_Mode(tx_address, 10); // canal 10 com Auto-ACK

    print_uart("[NRF24 DEBUG INICIAL]\r\n");
    print_nrf_debug();

    while (1) {

        AHT20_ReadTemperatureHumidity(&temperature, &humidity);

        ldr = ADC_LDR_Read();
        // Envia via UART para debug
        char buf[64];
        float voltage = (ldr / 4095.0f) * 3.3f;

                // Formata a string para UART
        snprintf(buf, sizeof(buf), "LDR Raw: %u | Tensão: %.2f V\r\n", ldr, voltage);
        uart_write((uint8_t*)buf);

        // Copia os floats diretamente para o payload (memória binária)
        memcpy(&payload[0], &temperature, sizeof(float));
        memcpy(&payload[4], &humidity, sizeof(float));

        print_uart("[NRF24] Enviando dados brutos...\r\n");
        nRF24_WritePayload(payload, 8);
        delay_ms(5);

        uint8_t status = nRF24_GetStatus();
        if (status & MASK_TX_DS) {
            print_uart("[NRF24] Transmissão OK\r\n");
        } else if (status & MASK_MAX_RT) {
            print_uart("[NRF24] Falha: MAX_RT\r\n");
            nRF24_WriteReg(NRF24_CMD_FLUSH_TX, 0x00);
        } else {
            print_uart("[NRF24] Status indefinido\r\n");
        }

        nRF24_ClearIRQFlags();
        delay_ms(1000);
    }
}
