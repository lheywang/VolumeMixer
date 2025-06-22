################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../cpputest/src/Platforms/Borland/UtestPlatform.cpp 

CPP_DEPS += \
./cpputest/src/Platforms/Borland/UtestPlatform.d 

OBJS += \
./cpputest/src/Platforms/Borland/UtestPlatform.o 


# Each subdirectory must supply rules for building sources it contributes
cpputest/src/Platforms/Borland/%.o: ../cpputest/src/Platforms/Borland/%.cpp cpputest/src/Platforms/Borland/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/cpputest/include" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/mixer/inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Core/Inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/CMSIS/Include" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-cpputest-2f-src-2f-Platforms-2f-Borland

clean-cpputest-2f-src-2f-Platforms-2f-Borland:
	-$(RM) ./cpputest/src/Platforms/Borland/UtestPlatform.d ./cpputest/src/Platforms/Borland/UtestPlatform.o

.PHONY: clean-cpputest-2f-src-2f-Platforms-2f-Borland

