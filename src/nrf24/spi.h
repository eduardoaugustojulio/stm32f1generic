#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

void sp1_nss_set(uint8_t level);
uint8_t spi1_xfer_byte(uint8_t byte);
void spi1_write(uint8_t byte);
void spi1_init();
