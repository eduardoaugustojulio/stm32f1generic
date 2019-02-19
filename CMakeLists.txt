cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake")
project(STM32 C ASM)

IF(NOT CMAKE_CROSSCOMPILING)
  message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=/PATH/TO/TOOLCHAIN_FILE .")
ENDIF(NOT CMAKE_CROSSCOMPILING)

set(PROGRAM_DIR ${CMAKE_SOURCE_DIR}/programs)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/libraries)

# STM LIBS
set(STM_STD_DIR ${LIB_DIR}/stm32f10x-stdperiph-lib/Libraries/STM32F10x_StdPeriph_Driver)
file(GLOB STM_STD_SOURCES
    "${STM_STD_DIR}/src/*.c")
set(STM_STD_HEADERS ${STM_STD_DIR}/inc)

# CMSIS
set(CMSIS_DIR ${LIB_DIR}/stm32f10x-stdperiph-lib/Libraries/CMSIS)
file(GLOB CMSIS_SOURCE
          "${CMSIS_DIR}/CM3/DeviceSupport/ST/STM32F10x/*.c"
          "${CMSIS_DIR}/CM3/DeviceSupport/ST/STM32F10x/startup/arm/startup_stm32f10x_md.s"
)
set(CMSIS_ST_HEADERS 
        "${CMSIS_DIR}/CM3/CoreSupport"
        "${CMSIS_DIR}/CM3/DeviceSupport/ST/STM32F10x"
        "${CMSIS_DIR}/CM3/DeviceSupport/ST/STM32F10x/startup/arm"
)

# FREERTOS
set(FREERTOS_DIR "${LIB_DIR}/FreeRTOS/src")
set(FREERTOS_PORTABLE "${FREERTOS_DIR}/portable")
file(GLOB FREERTOS_SOURCES
        "${FREERTOS_DIR}/*.c"
        "${FREERTOS_PORTABLE}/GCC/ARM_CM3/*.c"
        "${FREERTOS_PORTABLE}/MemMang/heap_1.c"
)
set(FREERTOS_HEADERS
    "${FREERTOS_DIR}"
    "${FREERTOS_DIR}/include"
    "${FREERTOS_PORTABLE}/GCC/ARM_CM3"
    "${FREERTOS_PORTABLE}/MemMang"
)

set(STM32_GENERIC ${CMSIS_SOURCE} ${FREERTOS_SOURCES}  ${STM_STD_SOURCES})
include_directories(${STM_STD_HEADERS} ${CMSIS_ST_HEADERS} ${FREERTOS_HEADERS})

# enable for compile helloWord
#set(LIBOPENCM3_DIR ${LIB_DIR}/libopencm3)
#add_custom_target(libopencm3 make WORKING_DIRECTORY ${LIBOPENCM3_DIR})
#include_directories(${LIBOPENCM3_DIR}/include)
#link_directories(${LIBOPENCM3_DIR}/lib)

function(add_bin_from_elf bin elf)
  add_custom_target(${bin} ALL ${CMAKE_OBJCOPY} -Obinary ${elf} ${bin} DEPENDS ${elf})
endfunction(add_bin_from_elf)

function(bin_size bin)
	add_custom_command(TARGET ${bin} POST_BUILD 
		COMMAND ${CMAKE_SIZE} --format=berkeley ${bin})
endfunction(bin_size)

#add_subdirectory(${PROGRAM_DIR}/helloWord)
add_subdirectory(${PROGRAM_DIR}/rtos)
