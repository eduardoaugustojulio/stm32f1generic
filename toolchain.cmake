cmake_minimum_required(VERSION 2.8)
include (CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

CMAKE_FORCE_C_COMPILER("arm-none-eabi-gcc" GNU)
CMAKE_FORCE_CXX_COMPILER("arm-none-eabi-g++" GNU)

set(PROCESSOR_FLAGS "-mthumb -mcpu=cortex-m3")
set(LIBRARY_FLAGS "-DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER")
set(COMMON_FLAGS "${LIBRARY_FLAGS}  ${PROCESSOR_FLAGS} -fdata-sections -ffunction-sections -fno-builtin")
set(LD_FLAGS "-specs=rdimon.specs -specs=nano.specs -specs=nosys.specs")
set(CMAKE_C_FLAGS "-g -O2 -Wall --std=c11 ${COMMON_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-g -O2 --std=c++14 -fno-rtti -fno-exceptions ${COMMON_FLAGS} ${CMAKE_CXX_FLAGS}")

set(LINKER_SCRIPT_DIR "${CMAKE_SOURCE_DIR}/libraries/stm32f10x-stdperiph-lib/stm32_flash.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PROCESSOR_FLAGS} -T ${LINKER_SCRIPT_DIR} ${LD_FLAGS}")

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
