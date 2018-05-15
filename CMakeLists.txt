cmake_minimum_required(VERSION 2.8)

IF(NOT CMAKE_CROSSCOMPILING)
  message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=/PATH/TO/TOOLCHAIN_FILE .")
ENDIF(NOT CMAKE_CROSSCOMPILING)

add_definitions(-DSTM32F1)
set(COMMON_FLAGS "-mthumb -mcpu=cortex-m3 -fdata-sections -ffunction-sections -fno-builtin")
set(CMAKE_C_FLAGS "-g -O2 -Wall --std=c11 ${COMMON_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-g -O2 --std=c++14 -fno-rtti -fno-exceptions ${COMMON_FLAGS} ${CMAKE_CXX_FLAGS}")

set(LIBOPENCM3_DIR ${CMAKE_SOURCE_DIR}/libopencm3)
add_custom_target(libopencm3 make WORKING_DIRECTORY ${LIBOPENCM3_DIR})
include_directories(${LIBOPENCM3_DIR}/include)
link_directories(${LIBOPENCM3_DIR}/lib)

set(LINKER_SCRIPT_DIR "${LIBOPENCM3_DIR}/lib/stm32/f1/stm32f100x4.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mcpu=cortex-m3 -mthumb -T ${LINKER_SCRIPT_DIR} -nostartfiles -specs=rdimon.specs -specs=nano.specs -specs=nosys.specs")

function(add_bin_from_elf bin elf)
  add_custom_target(${bin} ALL ${CMAKE_OBJCOPY} -Obinary ${elf} ${bin} DEPENDS ${elf})
endfunction(add_bin_from_elf)

function(bin_size bin)
    add_custom_command(TARGET ${bin} POST_BUILD
        COMMAND ${CMAKE_SIZE} --format=berkeley ${bin})
endfunction(bin_size)

add_subdirectory(helloWord)
add_subdirectory(spi)
