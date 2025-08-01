#include "stm32f1xx.h"
#include "uart.h"
#include "i2c.h"
#include "aht20.h"
#include "nrf24.h"
#include "delay.h"
#include <stdio.h>
#include "adc_ldr.h"
#include <string.h>

float temperature, humidity;
uint16_t ldr;
uint32_t temp;
uint32_t hum;



void print_nrf_debug(void) {
    uart_print_int("CONFIG", nRF24_ReadReg(CONFIG));
    uart_print_int("EN_AA", nRF24_ReadReg(EN_AA));
    uart_print_int("RF_CH", nRF24_ReadReg(RF_CH));
    uart_print_int("RF_SETUP", nRF24_ReadReg(RF_SETUP));
    uart_print_int("STATUS", nRF24_ReadReg(STATUS));
    uart_print_int("FIFO_STATUS", nRF24_ReadReg(FIFO_STATUS));
}

int main(void) {
    // Endereço de 5 bytes do receptor (deve bater com RX_ADDR_P0 do receptor)
    uint8_t tx_address[5] = {0x01, 0xDD, 0xCC, 0xBB, 0xAA};
    uint8_t payload[32]; // 32 é o máx. permitido no NRF24

    UART_Init();
    delay_init();
    SPI1_Init();
    AHT20_Init();
    ADC_LDR_Init();


    print_uart("Inicializando sensor e rádio...\r\n");

    nRF24_Init();
    nRF24_TX_Mode(tx_address, 10); // Canal 10, com Auto-ACK

    print_uart("[NRF24 DEBUG INICIAL]\r\n");
    print_nrf_debug();

    while (1) {
        AHT20_ReadTemperatureHumidity(&temp, &hum);
        ldr = ADC_LDR_Read();

        uart_print_int("RAW_HUM", hum);
        uart_print_int("RAW_TEMP", temp);
        uart_print_int("LDR", ldr);

        // Monta payload com dados brutos
        memcpy(&payload[0],  &temp, 4);   // temp ocupa 4 bytes
        memcpy(&payload[4],  &hum, 4);    // hum ocupa 4 bytes
        memcpy(&payload[8],  &ldr, 2);    // ldr ocupa 2 bytes

        // Envia por UART (debug)
        print_uart("[NRF24] Enviando dados brutos...\r\n");

        // Envia por NRF24
        nRF24_WritePayload(payload, 10);  // apenas os 10 bytes úteis
        delay_ms(2);

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
