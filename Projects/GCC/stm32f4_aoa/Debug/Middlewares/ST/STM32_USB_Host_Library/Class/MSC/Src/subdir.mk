################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.o \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.o \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.d \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.d \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


