# STM32F1GENERIC  
  
FreeRTOS template repository to stm32 blue pill board.

### to build:

```
cd stm32f1generic/
mkdir build/ && cd build/
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..
make libopencm3
make all
```

### to flash:

`st-flash write stm32generic.bin 0x8000000`
