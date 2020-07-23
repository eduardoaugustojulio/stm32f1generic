#include <stdlib.h>
#include <stdio.h>

void spi_init();
void spi_css(uint8_t level);
uint8_t spi_xfer_byte(uint8_t byte);
