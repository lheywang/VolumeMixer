# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Define your project. Specify C language as no C++ support is needed.
project(Mixer_support_lib C)

set(LIB_SOURCES
    src/mixer.c
    src/parser/parser.c
    src/screen/screen.c
)

# Adding headers files
include_directories(inc/)
include_directories(src/private/)


option(BUILD_TESTS "Build the unit test executable instead of the main application." OFF)

if (BUILD_TESTS)

    message(STATUS "Configuring for Unit Test build (Host: x86_64)")    
    set(CMAKE_C_COMPILER "/usr/bin/gcc")
    set(CMAKE_CXX_COMPILER "g++")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++20 -Wall -Wextra ")

    include_directories(tools/cpputest/include/)
    include_directories(tools/cpputest/include/CppUTest/)

    add_subdirectory(cpputest)

    add_library(mixer STATIC ${LIB_SOURCES})

    set(TEST_SOURCE_FILES
    )

    add_executable(UnitsTests tests/tester.cpp ${TEST_SOURCE_FILES})

    target_link_libraries(UnitsTests
        PRIVATE
        CppUTest
        CppUTestExt
        mixer
    )

else()
    message(STATUS "Configuring for Main Application build (Target: AARCH64)")
    set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -O3 -MMD -Wall -Wextra ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a53 -O3 -std=c++20 -MMD -Wall -Wextra ")

    add_library(mixer STATIC ${LIB_SOURCES})
    
endif()
