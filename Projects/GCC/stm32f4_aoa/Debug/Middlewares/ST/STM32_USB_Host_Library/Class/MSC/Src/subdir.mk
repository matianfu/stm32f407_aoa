################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.o \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.o \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.d \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.d \
./Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


