################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c 

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
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.o: C:/cygwin/home/ma/github/stm32f4_aoa/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


