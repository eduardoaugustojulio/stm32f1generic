set(LIBOPENCM3_DIR "${CMAKE_SOURCE_DIR}/libs/libopencm3")

if (NOT EXISTS ${LIBOPENCM3_DIR}/Makefile)
    message(FATAL_ERROR "libopencm3 submodule not found. Initialize with 'git submodule update --init' in the source directory")
endif()

add_definitions(-DSTM32F1)
add_custom_target(libopencm3 make TARGETS=stm32/f1 WORKING_DIRECTORY ${LIBOPENCM3_DIR})
link_directories("${LIBOPENCM3_DIR}/lib")
include_directories("${LIBOPENCM3_DIR}/include")
