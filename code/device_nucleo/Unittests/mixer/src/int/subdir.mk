################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mixer/src/int/int_uart.c 

C_DEPS += \
./mixer/src/int/int_uart.d 

OBJS += \
./mixer/src/int/int_uart.o 


# Each subdirectory must supply rules for building sources it contributes
mixer/src/int/%.o: ../mixer/src/int/%.c mixer/src/int/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/mixer/inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Core/Inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/CMSIS/Include" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-mixer-2f-src-2f-int

clean-mixer-2f-src-2f-int:
	-$(RM) ./mixer/src/int/int_uart.d ./mixer/src/int/int_uart.o

.PHONY: clean-mixer-2f-src-2f-int

