# STM32F1GENERIC  

Study , implementation and template repository to stm32f103c8t6 using open source tools:

FreeRTOS, CMake, GCC, CMSIS and ST32F1 STDPERIPH .

###*to build:*

git clone ../stm32f1generic.git
cd stm32f1generic

_set your correct USER_COMPILER_PATH "$ENV{HOME}/gcc-arm-none-eabi/bin" in toolchain.cmake_

mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..
make

###*to flash:*

st-flash write programs/rtos/rtos.bin 0x8000000

