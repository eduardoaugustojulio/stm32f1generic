set(STM_LIB_DIR "libs/stm32f10x-stdperiph-lib/Libraries")

add_definitions("-DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER")

#STD LIB
set(STM_STD_DIR "${STM_LIB_DIR}/STM32F10x_StdPeriph_Driver")
set(STM_STD_HEADERS "${STM_STD_DIR}/inc")
file(GLOB STM_STD_SOURCES "${STM_STD_DIR}/src/*.c")

#CMSIS
set(CMSIS_DIR "${STM_LIB_DIR}/CMSIS/CM3")
set(CMSIS_HEADERS 
    "${CMSIS_DIR}/CoreSupport"
    "${CMSIS_DIR}/DeviceSupport/ST/STM32F10x"
    "${CMSIS_DIR}/DeviceSupport/ST/STM32F10x/startup/arm"
)
file(GLOB CMSIS_SOURCE
    "${CMSIS_DIR}/DeviceSupport/ST/STM32F10x/*.c"
    "${CMSIS_DIR}/DeviceSupport/ST/STM32F10x/startup/arm/startup_stm32f10x_md.s"
)

include_directories(${STM_STD_HEADERS} ${CMSIS_HEADERS})
add_library(stm32_stdperiph STATIC ${STM_STD_SOURCES} ${CMSIS_SOURCE})
