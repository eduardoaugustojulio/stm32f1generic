#include "nRF24.h"

GPIO_InitTypeDef GPIOConfNrf24;
SPI_InitTypeDef SpiConfNrf24;

void initNrf24(void)
{
	initSPIx();
}


void initSPIx(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIOConfNrf24.GPIO_Pin = CE_NRF24 | CSN_NRF24 |
			SCK_NRF24 | MOSI_NRF24 | MISO_NRF24;

	GPIOConfNrf24.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOConfNrf24.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIOConfNrf24);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SpiConfNrf24.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SpiConfNrf24.SPI_Mode = SPI_Mode_Master;
	SpiConfNrf24.SPI_DataSize = SPI_DataSize_8b;
	SpiConfNrf24.SPI_CPOL = SPI_CPOL_Low;
	SpiConfNrf24.SPI_CPHA = SPI_CPHA_1Edge;
	SpiConfNrf24.SPI_NSS = SPI_NSS_Soft;
	SpiConfNrf24.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SpiConfNrf24.SPI_FirstBit = SPI_FirstBit_MSB;
	SpiConfNrf24.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SpiConfNrf24);
	SPI_Cmd(SPI1, ENABLE);
}


uint8_t Nrf24WriteRead(uint8_t data)
{
	return SPIxWriteRead(SPI1, data);
}

uint8_t SPIxWriteRead(SPI_TypeDef *SPI, uint8_t data){
	while (!(SPI->SR & SPI_SR_TXE)); // Wait while receive buffer is not empty
	SPI->DR = data; // Send byte to SPI
	while (!(SPI->SR & SPI_SR_RXNE)); // Wait while receive buffer is empty
	return SPI->DR; // Received byte
}

static uint8_t ReadRegNrf24(uint8_t reg)
{
	uint8_t value;
	CSN_NRF24_LOW();
	value = Nrf24WriteRead(reg & nRF24_MASK_RX_DR); // TODO CHANGE TO CORRECT FLAG
	value = Nrf24WriteRead(0x00);
	CSN_NRF24_HIGH();
	return value;
}


static void WriteRegNrf24(uint8_t reg, uint8_t value)
{

}
