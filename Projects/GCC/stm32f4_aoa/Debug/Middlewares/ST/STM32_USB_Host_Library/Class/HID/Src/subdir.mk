################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_keybd.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_mouse.c \
C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_parser.c 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.o \
./Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_keybd.o \
./Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_mouse.o \
./Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_parser.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.d \
./Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_keybd.d \
./Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_mouse.d \
./Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_parser.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_keybd.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_keybd.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_mouse.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_mouse.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_parser.o: C:/cygwin/home/ma/github/stm32f4_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_parser.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -DUSE_HAL_DRIVER -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Core\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\HID\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Middlewares\ST\STM32_USB_Host_Library\Class\MSC\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Inc" -I"C:/cygwin/home/ma/github/stm32f4_aoa/Projects/GCC/stm32f4_aoa\..\..\..\Drivers\STM32F4xx_HAL_Driver\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


