# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

project(Mixer_support_lib)

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

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++20 -Wall -Wextra ")

    include_directories(tools/cpputest/include/)
    include_directories(tools/cpputest/include/CppUTest/)

    add_subdirectory(cpputest)

    set(TEST_SOURCE_FILES
    )

    add_library(mixer STATIC ${LIB_SOURCES})

    add_executable(UnitsTests tests/tester.cpp ${TEST_SOURCE_FILES})

    target_link_libraries(UnitsTests
        PRIVATE
        CppUTest
        CppUTestExt
        mixer
    )

else()
    message(STATUS "Configuring for Main Application build (Target: AARCH64)")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -O3 -MMD -Wall -Wextra ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a53 -O3 -std=c++20 -MMD -Wall -Wextra ")

    add_library(mixer STATIC ${LIB_SOURCES})
    
endif()
