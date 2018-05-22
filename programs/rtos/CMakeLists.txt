cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME rtos)

set(SRCS_PATH 	"${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB SRCS
    "${SRCS_PATH}/*.c"
    "${SRCS_PATH}/*.h")
include_directories(${SRCS_PATH})

add_executable(${PROJECT_NAME}.elf ${SRCS_PATH}/main.c ${SRCS}
        ${CMSIS_SOURCE} ${FREERTOS_SOURCES}  ${STM_STD_SOURCES})

add_bin_from_elf(${PROJECT_NAME}.bin ${PROJECT_NAME}.elf)
bin_size(${PROJECT_NAME}.elf)
