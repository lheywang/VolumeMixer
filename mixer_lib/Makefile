# --- Toolchain Configuration ---
# Path to your ARM GNU Embedded Toolchain.
# Leave empty if 'arm-none-eabi-gcc' is in your system's PATH.
TOOLCHAIN_PATH ?=

# Define compilers, archiver, and linker
CC = $(TOOLCHAIN_PATH)arm-none-eabi-gcc
CXX = $(TOOLCHAIN_PATH)arm-none-eabi-g++
AR = $(TOOLCHAIN_PATH)arm-none-eabi-ar # Archiver for static libraries

# --- Project Configuration ---
TARGET = mixer
BUILD_DIR = build
LIB_DIR = lib

# --- Source Files ---
# List all C source files for your library
C_SRCS = \
    src/mixer.c \
    src/parser/parser.c \
	src/screen/screen.c

# List all C++ source files for your library (if any)
CXX_SRCS =
# Example: CXX_SRCS = src/cpp_component.cpp

# Generate a list of object files, placed in the build directory
# For src/my_library.c -> build/src/my_library.o
# For src/internal/private.c -> build/src/internal/private.o
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRCS)) \
       $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CXX_SRCS))

# --- Compiler Flags ---
# Architecture specific flags for ARM Cortex-M (e.g., STM32F4)
# Adjust -mcpu, -mfpu, -mfloat-abi based on your specific STM32 target.
# For Cortex-M0/M0+, you might remove -mfpu and use -mfloat-abi=soft.
ARCH_FLAGS = \
    -mcpu=cortex-m4         \
    -mthumb                 \
    -mfpu=fpv4-sp-d16       \
    -mfloat-abi=hard
    # -mfloat-abi=softfp
    # -mfloat-abi=soft

# Common optimization, warning, and debugging flags
COMMON_FLAGS = \
    -Os                     \
    -Wall                   \
    -Wextra                 \
    -Wno-unused-parameter   \
    -fdata-sections         \
    -ffunction-sections     \
    -fno-common             \
    -g

# Include paths for public and private headers
INCLUDES = -Iinc -Isrc/internal

# C compiler flags
C_FLAGS = $(ARCH_FLAGS) $(COMMON_FLAGS) $(INCLUDES) -std=c11

# C++ compiler flags
# -fno-exceptions and -fno-rtti are common for embedded C++ to reduce size.
CXX_FLAGS = $(ARCH_FLAGS) $(COMMON_FLAGS) $(INCLUDES) -std=c++11 -fno-exceptions -fno-rtti

# Archiver flags for creating static libraries
ARFLAGS = rcs

# --- Build Targets ---
.PHONY: all clean dirs

all: dirs $(LIB_DIR)/lib$(TARGET).a

# Create necessary directories
dirs:
	@mkdir -p $(BUILD_DIR) $(LIB_DIR)
	@mkdir -p $(dir $(OBJS)) # Create subdirectories for object files like build/src/ and build/src/internal/

# Rule to compile C source files into object files
# Uses automatic variables: $< is the prerequisite (source file), $@ is the target (object file)
$(BUILD_DIR)/%.o: %.c
	@echo "Compiling C: $<"
	@$(CC) $(C_FLAGS) -c $< -o $@

# Rule to compile C++ source files into object files
$(BUILD_DIR)/%.o: %.cpp
	@echo "Compiling C++: $<"
	@$(CXX) $(CXX_FLAGS) -c $< -o $@

# Rule to create the static library (.a file)
$(LIB_DIR)/lib$(TARGET).a: $(OBJS)
	@echo "Archiving $(TARGET) static library..."
	@$(AR) $(ARFLAGS) $@ $(OBJS)
	@echo "Successfully built static library: $@"

# Clean target: remove build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR) $(LIB_DIR)
	@echo "Clean complete."

