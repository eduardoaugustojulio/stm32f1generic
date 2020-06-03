cmake_minimum_required(VERSION 2.8)
include (CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

CMAKE_FORCE_C_COMPILER("arm-none-eabi-gcc" GNU)
CMAKE_FORCE_CXX_COMPILER("arm-none-eabi-g++" GNU)

set(MCU_FLAGS "-mthumb -mcpu=cortex-m3 -msoft-float")
set(COMPILER_FLAGS "-fdata-sections -ffunction-sections -fno-builtin")
set(COMMON_FLAGS "${MCU_FLAGS} ${COMPILER_FLAGS}")

set(CMAKE_C_FLAGS "-g -O2 -std=c11 -Wextra ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "-g -O2 -std=c++14 -Wextra ${COMMON_FLAGS}")

set(LD_FLAGS "-nostartfiles -specs=nano.specs -specs=nosys.specs")
set(LINKER_SCRIPT_DIR "${CMAKE_SOURCE_DIR}/libs/libopencm3/lib/stm32/f1/stm32f103x8.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LD_FLAGS} ${MCU_FLAGS} -T ${LINKER_SCRIPT_DIR}")

set(CMAKE_AR		"arm-none-eabi-ar")
set(CMAKE_LINKER	"arm-none-eabi-ld")
set(CMAKE_NM		"arm-none-eabi-nm")
set(CMAKE_OBJCOPY   "arm-none-eabi-objcopy")
set(CMAKE_OBJDUMP   "arm-none-eabi-objdump")
set(CMAKE_STRIP 	"arm-none-eabi-strip")
set(CMAKE_RANLIB	"arm-none-eabi-ranlib")
set(CMAKE_SIZE      "arm-none-eabi-size")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM	NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY	ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE	ONLY)
