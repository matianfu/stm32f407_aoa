################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c 

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
Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -DSEMIHOSTING -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


