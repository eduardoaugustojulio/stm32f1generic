cmake_minimum_required(VERSION 2.8)
include (CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME	  Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(USER_COMPILER_PATH "$ENV{HOME}/gcc-arm-none-eabi-7/bin")
CMAKE_FORCE_C_COMPILER("${USER_COMPILER_PATH}/arm-none-eabi-gcc"   GNU)
CMAKE_FORCE_CXX_COMPILER("${USER_COMPILER_PATH}/arm-none-eabi-g++" GNU)

set(PROCESSOR_FLAGS "-mthumb -mcpu=cortex-m3")
set(COMMON_FLAGS " -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVE ${PROCESSOR_FLAGS} -fdata-sections -ffunction-sections -fno-builtin")
set(LD_FLAGS "-nostartfiles -specs=rdimon.specs -specs=nano.specs -specs=nosys.specs")
set(CMAKE_C_FLAGS "-g -O2 -Wall --std=c11 ${COMMON_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-g -O2 --std=c++14 -fno-rtti -fno-exceptions ${COMMON_FLAGS} ${CMAKE_CXX_FLAGS}")

set(LINKER_SCRIPT_DIR "${CMAKE_SOURCE_DIR}/utils/stm32_flash.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PROCESSOR_FLAGS} -T ${LINKER_SCRIPT_DIR} ${LD_FLAGS}")

set(CMAKE_AR		"${USER_COMPILER_PATH}/arm-none-eabi-ar")
set(CMAKE_LINKER	"${USER_COMPILER_PATH}/arm-none-eabi-ld")
set(CMAKE_NM		"${USER_COMPILER_PATH}/arm-none-eabi-nm")
set(CMAKE_OBJCOPY    	"${USER_COMPILER_PATH}/arm-none-eabi-objcopy")
set(CMAKE_OBJDUMP    	"${USER_COMPILER_PATH}/arm-none-eabi-objdump")
set(CMAKE_STRIP 	"${USER_COMPILER_PATH}/arm-none-eabi-strip")
set(CMAKE_RANLIB	"${USER_COMPILER_PATH}/arm-none-eabi-ranlib")
set(CMAKE_SIZE          "${USER_COMPILER_PATH}/arm-none-eabi-size")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM	NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY	ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE	ONLY)