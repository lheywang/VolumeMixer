################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../cpputest/src/CppUTest/CommandLineArguments.cpp \
../cpputest/src/CppUTest/CommandLineTestRunner.cpp \
../cpputest/src/CppUTest/JUnitTestOutput.cpp \
../cpputest/src/CppUTest/MemoryLeakDetector.cpp \
../cpputest/src/CppUTest/MemoryLeakWarningPlugin.cpp \
../cpputest/src/CppUTest/SimpleMutex.cpp \
../cpputest/src/CppUTest/SimpleString.cpp \
../cpputest/src/CppUTest/SimpleStringInternalCache.cpp \
../cpputest/src/CppUTest/TeamCityTestOutput.cpp \
../cpputest/src/CppUTest/TestFailure.cpp \
../cpputest/src/CppUTest/TestFilter.cpp \
../cpputest/src/CppUTest/TestHarness_c.cpp \
../cpputest/src/CppUTest/TestMemoryAllocator.cpp \
../cpputest/src/CppUTest/TestOutput.cpp \
../cpputest/src/CppUTest/TestPlugin.cpp \
../cpputest/src/CppUTest/TestRegistry.cpp \
../cpputest/src/CppUTest/TestResult.cpp \
../cpputest/src/CppUTest/TestTestingFixture.cpp \
../cpputest/src/CppUTest/Utest.cpp 

CPP_DEPS += \
./cpputest/src/CppUTest/CommandLineArguments.d \
./cpputest/src/CppUTest/CommandLineTestRunner.d \
./cpputest/src/CppUTest/JUnitTestOutput.d \
./cpputest/src/CppUTest/MemoryLeakDetector.d \
./cpputest/src/CppUTest/MemoryLeakWarningPlugin.d \
./cpputest/src/CppUTest/SimpleMutex.d \
./cpputest/src/CppUTest/SimpleString.d \
./cpputest/src/CppUTest/SimpleStringInternalCache.d \
./cpputest/src/CppUTest/TeamCityTestOutput.d \
./cpputest/src/CppUTest/TestFailure.d \
./cpputest/src/CppUTest/TestFilter.d \
./cpputest/src/CppUTest/TestHarness_c.d \
./cpputest/src/CppUTest/TestMemoryAllocator.d \
./cpputest/src/CppUTest/TestOutput.d \
./cpputest/src/CppUTest/TestPlugin.d \
./cpputest/src/CppUTest/TestRegistry.d \
./cpputest/src/CppUTest/TestResult.d \
./cpputest/src/CppUTest/TestTestingFixture.d \
./cpputest/src/CppUTest/Utest.d 

OBJS += \
./cpputest/src/CppUTest/CommandLineArguments.o \
./cpputest/src/CppUTest/CommandLineTestRunner.o \
./cpputest/src/CppUTest/JUnitTestOutput.o \
./cpputest/src/CppUTest/MemoryLeakDetector.o \
./cpputest/src/CppUTest/MemoryLeakWarningPlugin.o \
./cpputest/src/CppUTest/SimpleMutex.o \
./cpputest/src/CppUTest/SimpleString.o \
./cpputest/src/CppUTest/SimpleStringInternalCache.o \
./cpputest/src/CppUTest/TeamCityTestOutput.o \
./cpputest/src/CppUTest/TestFailure.o \
./cpputest/src/CppUTest/TestFilter.o \
./cpputest/src/CppUTest/TestHarness_c.o \
./cpputest/src/CppUTest/TestMemoryAllocator.o \
./cpputest/src/CppUTest/TestOutput.o \
./cpputest/src/CppUTest/TestPlugin.o \
./cpputest/src/CppUTest/TestRegistry.o \
./cpputest/src/CppUTest/TestResult.o \
./cpputest/src/CppUTest/TestTestingFixture.o \
./cpputest/src/CppUTest/Utest.o 


# Each subdirectory must supply rules for building sources it contributes
cpputest/src/CppUTest/%.o: ../cpputest/src/CppUTest/%.cpp cpputest/src/CppUTest/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/cpputest/include" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/mixer/inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Core/Inc" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/CMSIS/Include" -I"/home/lheywang/Dev/VolumeMixer/code/device_nucleo/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-cpputest-2f-src-2f-CppUTest

clean-cpputest-2f-src-2f-CppUTest:
	-$(RM) ./cpputest/src/CppUTest/CommandLineArguments.d ./cpputest/src/CppUTest/CommandLineArguments.o ./cpputest/src/CppUTest/CommandLineTestRunner.d ./cpputest/src/CppUTest/CommandLineTestRunner.o ./cpputest/src/CppUTest/JUnitTestOutput.d ./cpputest/src/CppUTest/JUnitTestOutput.o ./cpputest/src/CppUTest/MemoryLeakDetector.d ./cpputest/src/CppUTest/MemoryLeakDetector.o ./cpputest/src/CppUTest/MemoryLeakWarningPlugin.d ./cpputest/src/CppUTest/MemoryLeakWarningPlugin.o ./cpputest/src/CppUTest/SimpleMutex.d ./cpputest/src/CppUTest/SimpleMutex.o ./cpputest/src/CppUTest/SimpleString.d ./cpputest/src/CppUTest/SimpleString.o ./cpputest/src/CppUTest/SimpleStringInternalCache.d ./cpputest/src/CppUTest/SimpleStringInternalCache.o ./cpputest/src/CppUTest/TeamCityTestOutput.d ./cpputest/src/CppUTest/TeamCityTestOutput.o ./cpputest/src/CppUTest/TestFailure.d ./cpputest/src/CppUTest/TestFailure.o ./cpputest/src/CppUTest/TestFilter.d ./cpputest/src/CppUTest/TestFilter.o ./cpputest/src/CppUTest/TestHarness_c.d ./cpputest/src/CppUTest/TestHarness_c.o ./cpputest/src/CppUTest/TestMemoryAllocator.d ./cpputest/src/CppUTest/TestMemoryAllocator.o ./cpputest/src/CppUTest/TestOutput.d ./cpputest/src/CppUTest/TestOutput.o ./cpputest/src/CppUTest/TestPlugin.d ./cpputest/src/CppUTest/TestPlugin.o ./cpputest/src/CppUTest/TestRegistry.d ./cpputest/src/CppUTest/TestRegistry.o ./cpputest/src/CppUTest/TestResult.d ./cpputest/src/CppUTest/TestResult.o ./cpputest/src/CppUTest/TestTestingFixture.d ./cpputest/src/CppUTest/TestTestingFixture.o ./cpputest/src/CppUTest/Utest.d ./cpputest/src/CppUTest/Utest.o

.PHONY: clean-cpputest-2f-src-2f-CppUTest

