# Projeto de Microcontroladores - STM32F103C8T6

## 📋 Descrição

Este projeto implementa um sistema de monitoramento ambiental usando o microcontrolador STM32F103C8T6 (Blue Pill). O sistema coleta dados de temperatura, umidade e luminosidade através de sensores, e transmite essas informações via comunicação sem fio usando o módulo NRF24L01+.

## Autores

David Silva Fernandes de Oliveira - 539402

Maria Eduarda Santana Marques - 538264

Gisleno Rodrigues de Alencar Silva Júnior -  511938


## 🎯 Funcionalidades

- **Sensores Integrados:**
  - Sensor AHT20 para temperatura e umidade
  - Sensor LDR (Light Dependent Resistor) para medição de luminosidade
  - Conversão ADC para leitura do sensor LDR

- **Comunicação:**
  - Transmissão de dados via NRF24L01+ (2.4GHz)
  - Comunicação UART para debug e monitoramento
  - Interface I2C para comunicação com o sensor AHT20

- **Características Técnicas:**
  - Frequência de amostragem: 1Hz
  - Payload de transmissão: 10 bytes (4 bytes temp + 4 bytes umidade + 2 bytes LDR)
  - Canal RF: 10 (configurável)
  - Endereço de transmissão: `0x01, 0xDD, 0xCC, 0xBB, 0xAA`

## 🛠️ Hardware Necessário

### Microcontrolador
- **STM32F103C8T6** (Blue Pill)
- Clock: 72MHz
- Flash: 64KB
- RAM: 20KB

### Sensores e Módulos
- **Sensor AHT20** (Temperatura e Umidade)
  - Interface: I2C
  - Endereço: 0x38
  - Precisão: ±0.3°C (temperatura), ±2% (umidade)

- **Sensor LDR** (Luminosidade)
  - Interface: ADC
  - Resolução: 12-bit
  - Tensão de referência: 3.3V

- **Módulo NRF24L01+** (Comunicação RF)
  - Frequência: 2.4GHz
  - Potência: 0dBm
  - Taxa de dados: 2Mbps
  - Alcance: ~100m (em campo aberto)


## 📁 Estrutura do Projeto

```
Projeto_Final/
├── Inc/                    # Arquivos de cabeçalho
│   ├── main.h             # Definições principais
│   ├── aht20.h            # Interface do sensor AHT20
│   ├── nrf24.h            # Interface do módulo NRF24L01+
│   ├── i2c.h              # Biblioteca I2C
│   ├── uart.h             # Biblioteca UART
│   ├── adc_ldr.h          # Interface ADC para LDR
│   └── delay.h            # Funções de delay
├── Src/                   # Arquivos fonte
│   ├── main.c             # Programa principal
│   ├── aht20.c            # Implementação do sensor AHT20
│   ├── nrf24.c            # Implementação do NRF24L01+
│   ├── i2c.c              # Implementação I2C
│   ├── uart.c             # Implementação UART
│   ├── adc_ldr.c          # Implementação ADC
│   └── delay.c            # Implementação delay
├── Startup/               # Arquivos de inicialização
│   └── startup_stm32f103c8tx.s
├── F1_Header/             # Headers STM32F1
└── Debug/                 # Arquivos de compilação
```
