#ifndef __SPI_H
#define __SPI_H

void initSPIx(void);
uint8_t SPIxWriteRead(SPI_TypeDef*, uint8_t);

#endif // __NRF24_H
