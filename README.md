# STM32F1GENERIC  
  
Template repository to stm32f103c8t6:

### to build:

```
cd stm32f1generic/
mkdir build/ && cd build/
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..
make
```

### to flash:

`st-flash write programs/rtos/rtos.bin 0x8000000`
