set(FREERTOS_DIR "${CMAKE_SOURCE_DIR}/libs/FreeRTOS/src")

set(FREERTOS_HEADERS 
        "${FREERTOS_DIR}/include" 
        "${FREERTOS_DIR}/portable/GCC/ARM_CM3" 
        "${FREERTOS_DIR}/portable/MemMang"
)

file(GLOB FREERTOS_SOURCES 
        "${FREERTOS_DIR}/*.c" 
        "${FREERTOS_DIR}/portable/MemMang/heap_1.c"
        "${FREERTOS_DIR}/portable/GCC/ARM_CM3/*.c"
)

include_directories(${FREERTOS_HEADERS})
add_library(freertos STATIC ${FREERTOS_SOURCES})
