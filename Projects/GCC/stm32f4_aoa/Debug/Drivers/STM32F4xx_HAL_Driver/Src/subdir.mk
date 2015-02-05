################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
/home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c 

OBJS += \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.o 

C_DEPS += \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.o: /home/ma/stm32/stm32f407_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


