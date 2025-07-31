################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc_ldr.c \
../Src/aht20.c \
../Src/delay.c \
../Src/i2c.c \
../Src/main.c \
../Src/nrf24.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/uart.c 

OBJS += \
./Src/adc_ldr.o \
./Src/aht20.o \
./Src/delay.o \
./Src/i2c.o \
./Src/main.o \
./Src/nrf24.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/uart.o 

C_DEPS += \
./Src/adc_ldr.d \
./Src/aht20.d \
./Src/delay.d \
./Src/i2c.d \
./Src/main.d \
./Src/nrf24.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DSTM32F103xB=STM32F103xB -c -I"/BM_Led_Blink/Inc" -I"/home/eduarda/STM32CubeIDE/workspace_1.18.1/BM_Led_Blink/F1_Header/Include" -I"/home/eduarda/STM32CubeIDE/workspace_1.18.1/BM_Led_Blink/F1_Header/Device/ST/STM32F1xx/Include" -I/home/eduarda/STM32CubeIDE/workspace_1.18.1/BM_Led_Blink/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/adc_ldr.cyclo ./Src/adc_ldr.d ./Src/adc_ldr.o ./Src/adc_ldr.su ./Src/aht20.cyclo ./Src/aht20.d ./Src/aht20.o ./Src/aht20.su ./Src/delay.cyclo ./Src/delay.d ./Src/delay.o ./Src/delay.su ./Src/i2c.cyclo ./Src/i2c.d ./Src/i2c.o ./Src/i2c.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/nrf24.cyclo ./Src/nrf24.d ./Src/nrf24.o ./Src/nrf24.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

