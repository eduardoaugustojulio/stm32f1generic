#ifndef __NRF24_H
#define __NRF24_H

#include <stdio.h>
#include <string.h>

#include <stm32f10x.h>
#include <stm32f10x_it.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>

#include <FreeRTOS.h>
#include <task.h>


#define PORT_NRF24 GPIOA

#define	CE_NRF24   GPIO_Pin_3
#define CE_NRF24_LOW()	GPIO_ResetBits(PORT_NRF24, CE_NRF24);
#define CE_NRF24_HIGH() GPIO_SetBits(PORT_NRF24, CE_NRF24);

#define	CSN_NRF24  GPIO_Pin_4
#define CSN_NRF24_LOW()	GPIO_ResetBits(PORT_NRF24, CSN_NRF24);
#define CSN_NRF24_HIGH() GPIO_SetBits(PORT_NRF24, CSN_NRF24);

#define SCK_NRF24  GPIO_Pin_5
#define MISO_NRF24 GPIO_Pin_6
#define MOSI_NRF24 GPIO_Pin_7

typedef enum {
	DataRate250kbps = (uint8_t)0x20,
	DataRate1Mbps = (uint8_t)0x00,
	DataRate2Mbps = (uint8_t)0x08
}Nrf24DataRateTypeDef;

typedef enum {
	TxPower18dBm = (uint8_t)0x00,
	TxPower12dBm = (uint8_t)0x02,
	TxPower6dBm = (uint8_t)0x04,
	TxPower0dBm = (uint8_t)0x06
}Nrf24TxPowerTypeDef;

typedef enum{
	CRCOff = (uint8_t)0x00,
	CRC1Byte = (uint8_t)0x08,
	CRC2Byte = (uint8_t)0x0c
}Nrf24CRCTypeDef;

typedef enum{
	PWRUp = (uint8_t)0x02,
	PWRDown = (uint8_t)0x00
}Nrf24PWRTypeDef;

typedef enum{
	Rx = (uint8_t)0x01,
	Tx = (uint8_t)0x00
}Nrf24PrimTypeDef;

typedef enum{
	RxPipe0 = (uint8_t)0x00,
	RxPipe1 = (uint8_t)0x01,
	RxPipe2 = (uint8_t)0x02,
	RxPipe3 = (uint8_t)0x03,
	RxPipe4 = (uint8_t)0x04,
	RxPipe5 = (uint8_t)0x05
}Nrf24RxPipeTypeDef;

typedef enum{
	AutoACKOff = (uint8_t)0x00,
	AutoACKPipe0 = (uint8_t)0x01,
	AutoACKPipe1 = (uint8_t)0x02,
	AutoACKPipe2 = (uint8_t)0x04,
	AutoACKPipe3 = (uint8_t)0x08,
	AutoACKPipe4 = (uint8_t)0x10,
	AutoACKPipe5 = (uint8_t)0x20
}Nrf24EnableAutoACKPipeTypeDef;

typedef enum{
	RxPacketPipe0 = (uint8_t)0x00,
	RxPacketPipe1 = (uint8_t)0x01,
	RxPacketPipe2 = (uint8_t)0x02,
	RxPacketPipe3 = (uint8_t)0x03,
	RxPacketPipe4 = (uint8_t)0x04,
	RxPacketPipe5 = (uint8_t)0x05,
	RxPacketEmpty = (uint8_t)0xfe,
	RxPacketError = (uint8_t)0xff
}Nrf24RxPacketTypedef;

typedef enum {
	TxPacketSuccess,
	TxPacketTimeout,
	TxPacketMaxrt,
	TxPacketError
}NrfT4RxPacketTypedef;

// nRF24L0 commands
#define nRF24_CMD_RREG             0x00  // R_REGISTER -> Read command and status registers
#define nRF24_CMD_WREG             0x20  // W_REGISTER -> Write command and status registers
#define nRF24_CMD_R_RX_PAYLOAD     0x61  // R_RX_PAYLOAD -> Read RX payload
#define nRF24_CMD_W_TX_PAYLOAD     0xA0  // W_TX_PAYLOAD -> Write TX payload
#define nRF24_CMD_FLUSH_TX         0xE1  // FLUSH_TX -> Flush TX FIFO
#define nRF24_CMD_FLUSH_RX         0xE2  // FLUSH_RX -> Flush RX FIFO
#define nRF24_CMD_REUSE_TX_PL      0xE3  // REUSE_TX_PL -> Reuse last transmitted payload
#define nRF24_CMD_NOP              0xFF  // No operation (to read status register)

// nRF24L0 registers
#define nRF24_REG_CONFIG           0x00  // Configuration register
#define nRF24_REG_EN_AA            0x01  // Enable "Auto acknowledgment"
#define nRF24_REG_EN_RXADDR        0x02  // Enable RX addresses
#define nRF24_REG_SETUP_AW         0x03  // Setup of address widths
#define nRF24_REG_SETUP_RETR       0x04  // Setup of automatic retransmit
#define nRF24_REG_RF_CH            0x05  // RF channel
#define nRF24_REG_RF_SETUP         0x06  // RF setup register
#define nRF24_REG_STATUS           0x07  // Status register
#define nRF24_REG_OBSERVE_TX       0x08  // Transmit observe register
#define nRF24_REG_CD               0x09  // Carrier detect
#define nRF24_REG_RX_ADDR_P0       0x0A  // Receive address data pipe 0
#define nRF24_REG_RX_ADDR_P1       0x0B  // Receive address data pipe 1
#define nRF24_REG_RX_ADDR_P2       0x0C  // Receive address data pipe 2
#define nRF24_REG_RX_ADDR_P3       0x0D  // Receive address data pipe 3
#define nRF24_REG_RX_ADDR_P4       0x0E  // Receive address data pipe 4
#define nRF24_REG_RX_ADDR_P5       0x0F  // Receive address data pipe 5
#define nRF24_REG_TX_ADDR          0x10  // Transmit address
#define nRF24_REG_RX_PW_P0         0x11  // Number of bytes in RX payload id data pipe 0
#define nRF24_REG_RX_PW_P1         0x12  // Number of bytes in RX payload id data pipe 1
#define nRF24_REG_RX_PW_P2         0x13  // Number of bytes in RX payload id data pipe 2
#define nRF24_REG_RX_PW_P3         0x14  // Number of bytes in RX payload id data pipe 3
#define nRF24_REG_RX_PW_P4         0x15  // Number of bytes in RX payload id data pipe 4
#define nRF24_REG_RX_PW_P5         0x16  // Number of bytes in RX payload id data pipe 5
#define nRF24_REG_FIFO_STATUS      0x17  // FIFO status register
#define nRF24_REG_DYNPD            0x1C  // Enable dynamic payload length
#define nRF24_REG_FEATURE          0x1D  // Feature register

// nRF24L0 bits
#define nRF24_MASK_RX_DR           0x40  // Mask interrupt caused by RX_DR
#define nRF24_MASK_TX_DS           0x20  // Mask interrupt caused by TX_DS
#define nRF24_MASK_MAX_RT          0x10  // Mask interrupt caused by MAX_RT
#define nRF24_FIFO_RX_EMPTY        0x01  // RX FIFO empty flag
#define nRF24_FIFO_RX_FULL         0x02  // RX FIFO full flag

#define nRF24_TEST_ADDR         "nRF24"  // Fake address to test nRF24 presence

#define nRF24_WAIT_TIMEOUT   0x000FFFFF  // Timeout counter

static void WriteRegNrf24(uint8_t reg, uint8_t value);

void initNrf24 (void);
void initSPIx(void);

uint8_t Nrf24WriteRead(uint8_t data);
uint8_t SPIxWriteRead(SPI_TypeDef *SPI, uint8_t data);



#endif // __NRF24_H
