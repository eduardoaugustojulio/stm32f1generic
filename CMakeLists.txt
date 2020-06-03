cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchain.cmake")
project(rtos C ASM)

function(add_bin_from_elf bin elf)
  add_custom_target(${bin} ALL ${CMAKE_OBJCOPY} -Obinary ${elf} ${bin} DEPENDS ${elf})
endfunction(add_bin_from_elf)

function(elf_size bin)
    add_custom_command(TARGET ${bin} POST_BUILD 
        COMMAND ${CMAKE_SIZE} --format=berkeley ${bin})
endfunction(elf_size)

add_definitions(-DSTM32F1)
set(LIB_DIR "${CMAKE_SOURCE_DIR}/libs")
set(SRCS_PATH "${CMAKE_SOURCE_DIR}/src")
file(GLOB SRCS "${SRCS_PATH}/*.c")

# LIBOPENCM3
set(LIBOPENCM3_DIR "${LIB_DIR}/libopencm3")
add_custom_target(libopencm3 make WORKING_DIRECTORY ${LIBOPENCM3_DIR})
set(LIBOPENCM3_HEADERS "${LIBOPENCM3_DIR}/include")
set(LIBOPENCM3_BINARIES "${LIBOPENCM3_DIR}/lib")
link_directories(${LIBOPENCM3_BINARIES})

# FREERTOS
set(FREERTOS_DIR "${LIB_DIR}/FreeRTOS/src")
set(FREERTOS_PORTABLE "${FREERTOS_DIR}/portable")
set(FREERTOS_HEADERS "${FREERTOS_DIR}/include" "${FREERTOS_PORTABLE}/GCC/ARM_CM3" "${FREERTOS_PORTABLE}/MemMang")
file(GLOB FREERTOS_SOURCES "${FREERTOS_DIR}/*.c" "${FREERTOS_PORTABLE}/GCC/ARM_CM3/*.c" "${FREERTOS_PORTABLE}/MemMang/heap_1.c")

include_directories(${SRCS_PATH} ${FREERTOS_HEADERS} ${LIBOPENCM3_HEADERS})
add_executable("${PROJECT_NAME}.elf" ${SRCS} ${FREERTOS_SOURCES})
target_link_libraries("${PROJECT_NAME}.elf" "opencm3_stm32f1")

add_bin_from_elf("${PROJECT_NAME}.bin" "${PROJECT_NAME}.elf")
elf_size("${PROJECT_NAME}.elf")
