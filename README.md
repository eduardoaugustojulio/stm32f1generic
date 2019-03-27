# STM32F1GENERIC  
  
FreeRTOS template repository to stm32 blue pill board.

### to build:

```
cd stm32f1generic/
mkdir build/ && cd build/
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..
make
```

### to flash:

`st-flash write programs/rtos/rtos.bin 0x8000000`
