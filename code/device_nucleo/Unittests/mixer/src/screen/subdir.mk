################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mixer/src/screen/screen.c 

C_DEPS += \
./mixer/src/screen/screen.d 

OBJS += \
./mixer/src/screen/screen.o 


# Each subdirectory must supply rules for building sources it contributes
mixer/src/screen/%.o: ../mixer/src/screen/%.c mixer/src/screen/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/mixer/inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Core/Inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/CMSIS/Include" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-mixer-2f-src-2f-screen

clean-mixer-2f-src-2f-screen:
	-$(RM) ./mixer/src/screen/screen.d ./mixer/src/screen/screen.o

.PHONY: clean-mixer-2f-src-2f-screen

