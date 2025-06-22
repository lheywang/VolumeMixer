################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../cpputest/src/CppUTestExt/CodeMemoryReportFormatter.cpp \
../cpputest/src/CppUTestExt/GTest.cpp \
../cpputest/src/CppUTestExt/IEEE754ExceptionsPlugin.cpp \
../cpputest/src/CppUTestExt/MemoryReportAllocator.cpp \
../cpputest/src/CppUTestExt/MemoryReportFormatter.cpp \
../cpputest/src/CppUTestExt/MemoryReporterPlugin.cpp \
../cpputest/src/CppUTestExt/MockActualCall.cpp \
../cpputest/src/CppUTestExt/MockExpectedCall.cpp \
../cpputest/src/CppUTestExt/MockExpectedCallsList.cpp \
../cpputest/src/CppUTestExt/MockFailure.cpp \
../cpputest/src/CppUTestExt/MockNamedValue.cpp \
../cpputest/src/CppUTestExt/MockSupport.cpp \
../cpputest/src/CppUTestExt/MockSupportPlugin.cpp \
../cpputest/src/CppUTestExt/MockSupport_c.cpp \
../cpputest/src/CppUTestExt/OrderedTest.cpp 

CPP_DEPS += \
./cpputest/src/CppUTestExt/CodeMemoryReportFormatter.d \
./cpputest/src/CppUTestExt/GTest.d \
./cpputest/src/CppUTestExt/IEEE754ExceptionsPlugin.d \
./cpputest/src/CppUTestExt/MemoryReportAllocator.d \
./cpputest/src/CppUTestExt/MemoryReportFormatter.d \
./cpputest/src/CppUTestExt/MemoryReporterPlugin.d \
./cpputest/src/CppUTestExt/MockActualCall.d \
./cpputest/src/CppUTestExt/MockExpectedCall.d \
./cpputest/src/CppUTestExt/MockExpectedCallsList.d \
./cpputest/src/CppUTestExt/MockFailure.d \
./cpputest/src/CppUTestExt/MockNamedValue.d \
./cpputest/src/CppUTestExt/MockSupport.d \
./cpputest/src/CppUTestExt/MockSupportPlugin.d \
./cpputest/src/CppUTestExt/MockSupport_c.d \
./cpputest/src/CppUTestExt/OrderedTest.d 

OBJS += \
./cpputest/src/CppUTestExt/CodeMemoryReportFormatter.o \
./cpputest/src/CppUTestExt/GTest.o \
./cpputest/src/CppUTestExt/IEEE754ExceptionsPlugin.o \
./cpputest/src/CppUTestExt/MemoryReportAllocator.o \
./cpputest/src/CppUTestExt/MemoryReportFormatter.o \
./cpputest/src/CppUTestExt/MemoryReporterPlugin.o \
./cpputest/src/CppUTestExt/MockActualCall.o \
./cpputest/src/CppUTestExt/MockExpectedCall.o \
./cpputest/src/CppUTestExt/MockExpectedCallsList.o \
./cpputest/src/CppUTestExt/MockFailure.o \
./cpputest/src/CppUTestExt/MockNamedValue.o \
./cpputest/src/CppUTestExt/MockSupport.o \
./cpputest/src/CppUTestExt/MockSupportPlugin.o \
./cpputest/src/CppUTestExt/MockSupport_c.o \
./cpputest/src/CppUTestExt/OrderedTest.o 


# Each subdirectory must supply rules for building sources it contributes
cpputest/src/CppUTestExt/%.o: ../cpputest/src/CppUTestExt/%.cpp cpputest/src/CppUTestExt/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/cpputest/include" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/mixer/inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Core/Inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/CMSIS/Include" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-cpputest-2f-src-2f-CppUTestExt

clean-cpputest-2f-src-2f-CppUTestExt:
	-$(RM) ./cpputest/src/CppUTestExt/CodeMemoryReportFormatter.d ./cpputest/src/CppUTestExt/CodeMemoryReportFormatter.o ./cpputest/src/CppUTestExt/GTest.d ./cpputest/src/CppUTestExt/GTest.o ./cpputest/src/CppUTestExt/IEEE754ExceptionsPlugin.d ./cpputest/src/CppUTestExt/IEEE754ExceptionsPlugin.o ./cpputest/src/CppUTestExt/MemoryReportAllocator.d ./cpputest/src/CppUTestExt/MemoryReportAllocator.o ./cpputest/src/CppUTestExt/MemoryReportFormatter.d ./cpputest/src/CppUTestExt/MemoryReportFormatter.o ./cpputest/src/CppUTestExt/MemoryReporterPlugin.d ./cpputest/src/CppUTestExt/MemoryReporterPlugin.o ./cpputest/src/CppUTestExt/MockActualCall.d ./cpputest/src/CppUTestExt/MockActualCall.o ./cpputest/src/CppUTestExt/MockExpectedCall.d ./cpputest/src/CppUTestExt/MockExpectedCall.o ./cpputest/src/CppUTestExt/MockExpectedCallsList.d ./cpputest/src/CppUTestExt/MockExpectedCallsList.o ./cpputest/src/CppUTestExt/MockFailure.d ./cpputest/src/CppUTestExt/MockFailure.o ./cpputest/src/CppUTestExt/MockNamedValue.d ./cpputest/src/CppUTestExt/MockNamedValue.o ./cpputest/src/CppUTestExt/MockSupport.d ./cpputest/src/CppUTestExt/MockSupport.o ./cpputest/src/CppUTestExt/MockSupportPlugin.d ./cpputest/src/CppUTestExt/MockSupportPlugin.o ./cpputest/src/CppUTestExt/MockSupport_c.d ./cpputest/src/CppUTestExt/MockSupport_c.o ./cpputest/src/CppUTestExt/OrderedTest.d ./cpputest/src/CppUTestExt/OrderedTest.o

.PHONY: clean-cpputest-2f-src-2f-CppUTestExt

