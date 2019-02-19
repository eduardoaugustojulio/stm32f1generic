#include <stdio.h>
#include <string.h>

#include <stm32f10x.h>
#include <stm32f10x_it.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>

#define	CE   GPIO_Pin_3
#define	CSN  GPIO_Pin_4
#define SCK  GPIO_Pin_5
#define MISO GPIO_Pin_6
#define MOSI GPIO_Pin_7
#define PORT GPIOA

GPIO_InitTypeDef GPIO_conf;
SPI_InitTypeDef SPI_conf;

void initSPIx(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_conf.GPIO_Pin = CE | CSN | SCK | MOSI | MISO;

    GPIO_conf.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_conf.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(PORT, &GPIO_conf);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    SPI_conf.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_conf.SPI_Mode = SPI_Mode_Master;
    SPI_conf.SPI_DataSize = SPI_DataSize_8b;
    SPI_conf.SPI_CPOL = SPI_CPOL_Low;
    SPI_conf.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_conf.SPI_NSS = SPI_NSS_Soft;
    SPI_conf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_conf.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_conf.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_conf);
    SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPIxWriteRead(SPI_TypeDef *SPI, uint8_t data){
    while (!(SPI->SR & SPI_SR_TXE)); // Wait while receive buffer is not empty
    SPI->DR = data; // Send byte to SPI
    while (!(SPI->SR & SPI_SR_RXNE)); // Wait while receive buffer is empty
    return SPI->DR; // Received byte
}

