################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.c \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_keybd.c \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_mouse.c \
/home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_parser.c 

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
Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_keybd.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_keybd.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_mouse.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_mouse.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_parser.o: /home/ma/stm32/stm32f407_aoa/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_parser.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


