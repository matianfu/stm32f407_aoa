################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.o \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.o \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.o \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.d \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.d \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.d \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


