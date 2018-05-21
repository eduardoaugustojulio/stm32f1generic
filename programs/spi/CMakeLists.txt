cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME spi)

set(SRCS_PATH 	"${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(${PROJECT_NAME}.elf ${SRCS_PATH}/main.c ${SRCS})
target_link_libraries(${PROJECT_NAME}.elf opencm3_stm32f1)

add_bin_from_elf(${PROJECT_NAME}.bin ${PROJECT_NAME}.elf)
bin_size(${PROJECT_NAME}.elf)
