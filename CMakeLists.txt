cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchain.cmake")

project(stm32generic C ASM)

function(add_bin_from_elf bin elf)
  add_custom_target(${bin} ALL ${CMAKE_OBJCOPY} -Obinary ${elf} ${bin} DEPENDS ${elf})
endfunction(add_bin_from_elf)

function(calc_bin_size bin)
    add_custom_command(TARGET ${bin} POST_BUILD 
        COMMAND ${CMAKE_SIZE} --format=berkeley ${bin})
endfunction(calc_bin_size)

include(FreeRTOS)
include(libopencm3)

set(MCU_FLAGS "-mthumb -mcpu=cortex-m3 -msoft-float")
set(COMPILER_FLAGS "-fdata-sections -ffunction-sections -fno-builtin -finline-functions")
set(COMMON_FLAGS "${MCU_FLAGS} ${COMPILER_FLAGS}")

set(CMAKE_C_FLAGS "-Os -std=c11 -Wextra ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "-Os -std=c++14 -Wextra ${COMMON_FLAGS}")

set(LD_FLAGS "-nostartfiles -specs=nano.specs -specs=nosys.specs")
set(LINKER_SCRIPT_DIR "${CMAKE_SOURCE_DIR}/stm32f103x8.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LD_FLAGS} ${MCU_FLAGS} -T ${LINKER_SCRIPT_DIR}")

include_directories("src" "src/nrf24" "src/usb")
file(GLOB SOURCES "src/*.c" "src/nrf24/*.c" "src/usb/*.c")

add_executable(${PROJECT_NAME}.elf ${SOURCES})
target_link_libraries(${PROJECT_NAME}.elf opencm3_stm32f1 freertos)
add_bin_from_elf("${PROJECT_NAME}.bin" "${PROJECT_NAME}.elf")
calc_bin_size("${PROJECT_NAME}.elf")
