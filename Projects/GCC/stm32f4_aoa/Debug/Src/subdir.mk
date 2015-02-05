################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/cygwin/home/ma/github/stm32f4_aoa/Src/gpio.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Src/main.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Src/stm32f4xx_it.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Src/syscalls.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Src/usart.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Src/usb_host.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Src/usbh_conf.c 

OBJS += \
./Src/gpio.o \
./Src/main.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/usart.o \
./Src/usb_host.o \
./Src/usbh_conf.o 

C_DEPS += \
./Src/gpio.d \
./Src/main.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/usart.d \
./Src/usb_host.d \
./Src/usbh_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Src/gpio.o: C:/cygwin/home/ma/github/stm32f4_aoa/Src/gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/main.o: C:/cygwin/home/ma/github/stm32f4_aoa/Src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/stm32f4xx_it.o: C:/cygwin/home/ma/github/stm32f4_aoa/Src/stm32f4xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/syscalls.o: C:/cygwin/home/ma/github/stm32f4_aoa/Src/syscalls.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/usart.o: C:/cygwin/home/ma/github/stm32f4_aoa/Src/usart.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/usb_host.o: C:/cygwin/home/ma/github/stm32f4_aoa/Src/usb_host.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/usbh_conf.o: C:/cygwin/home/ma/github/stm32f4_aoa/Src/usbh_conf.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


