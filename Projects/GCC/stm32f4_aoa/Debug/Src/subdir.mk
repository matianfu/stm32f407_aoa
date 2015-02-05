################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/ma/stm32/stm32f407_aoa/Src/gpio.c \
/home/ma/stm32/stm32f407_aoa/Src/main.c \
/home/ma/stm32/stm32f407_aoa/Src/stm32f4xx_it.c \
/home/ma/stm32/stm32f407_aoa/Src/syscalls.c \
/home/ma/stm32/stm32f407_aoa/Src/usart.c \
/home/ma/stm32/stm32f407_aoa/Src/usb_host.c \
/home/ma/stm32/stm32f407_aoa/Src/usbh_conf.c 

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
Src/gpio.o: /home/ma/stm32/stm32f407_aoa/Src/gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/main.o: /home/ma/stm32/stm32f407_aoa/Src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/stm32f4xx_it.o: /home/ma/stm32/stm32f407_aoa/Src/stm32f4xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/syscalls.o: /home/ma/stm32/stm32f407_aoa/Src/syscalls.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/usart.o: /home/ma/stm32/stm32f407_aoa/Src/usart.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/usb_host.o: /home/ma/stm32/stm32f407_aoa/Src/usb_host.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/usbh_conf.o: /home/ma/stm32/stm32f407_aoa/Src/usbh_conf.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


