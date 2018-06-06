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

// nRF24L0 instruction definitions
#define nRF24_CMD_R_REGISTER       (uint8_t)0x00 // Register read
#define nRF24_CMD_W_REGISTER       (uint8_t)0x20 // Register write
#define nRF24_CMD_R_RX_PAYLOAD     (uint8_t)0x61 // Read RX payload
#define nRF24_CMD_W_TX_PAYLOAD     (uint8_t)0xA0 // Write TX payload
#define nRF24_CMD_FLUSH_TX         (uint8_t)0xE1 // Flush TX FIFO
#define nRF24_CMD_FLUSH_RX         (uint8_t)0xE2 // Flush RX FIFO
#define nRF24_CMD_REUSE_TX_PL      (uint8_t)0xE3 // Reuse TX payload
#define nRF24_CMD_LOCK_UNLOCK      (uint8_t)0x50 // Lock/unlock exclusive features
#define nRF24_CMD_NOP              (uint8_t)0xFF // No operation (used for reading status register)

// nRF24L0 register definitions
#define nRF24_REG_CONFIG           (uint8_t)0x00 // Configuration register
#define nRF24_REG_EN_AA            (uint8_t)0x01 // Enable "Auto acknowledgment"
#define nRF24_REG_EN_RXADDR        (uint8_t)0x02 // Enable RX addresses
#define nRF24_REG_SETUP_AW         (uint8_t)0x03 // Setup of address widths
#define nRF24_REG_SETUP_RETR       (uint8_t)0x04 // Setup of automatic retransmit
#define nRF24_REG_RF_CH            (uint8_t)0x05 // RF channel
#define nRF24_REG_RF_SETUP         (uint8_t)0x06 // RF setup register
#define nRF24_REG_STATUS           (uint8_t)0x07 // Status register
#define nRF24_REG_OBSERVE_TX       (uint8_t)0x08 // Transmit observe register
#define nRF24_REG_RPD              (uint8_t)0x09 // Received power detector
#define nRF24_REG_RX_ADDR_P0       (uint8_t)0x0A // Receive address data pipe 0
#define nRF24_REG_RX_ADDR_P1       (uint8_t)0x0B // Receive address data pipe 1
#define nRF24_REG_RX_ADDR_P2       (uint8_t)0x0C // Receive address data pipe 2
#define nRF24_REG_RX_ADDR_P3       (uint8_t)0x0D // Receive address data pipe 3
#define nRF24_REG_RX_ADDR_P4       (uint8_t)0x0E // Receive address data pipe 4
#define nRF24_REG_RX_ADDR_P5       (uint8_t)0x0F // Receive address data pipe 5
#define nRF24_REG_TX_ADDR          (uint8_t)0x10 // Transmit address
#define nRF24_REG_RX_PW_P0         (uint8_t)0x11 // Number of bytes in RX payload in data pipe 0
#define nRF24_REG_RX_PW_P1         (uint8_t)0x12 // Number of bytes in RX payload in data pipe 1
#define nRF24_REG_RX_PW_P2         (uint8_t)0x13 // Number of bytes in RX payload in data pipe 2
#define nRF24_REG_RX_PW_P3         (uint8_t)0x14 // Number of bytes in RX payload in data pipe 3
#define nRF24_REG_RX_PW_P4         (uint8_t)0x15 // Number of bytes in RX payload in data pipe 4
#define nRF24_REG_RX_PW_P5         (uint8_t)0x16 // Number of bytes in RX payload in data pipe 5
#define nRF24_REG_FIFO_STATUS      (uint8_t)0x17 // FIFO status register
#define nRF24_REG_DYNPD            (uint8_t)0x1C // Enable dynamic payload length
#define nRF24_REG_FEATURE          (uint8_t)0x1D // Feature register

// Register bits definitions
#define nRF24_CONFIG_PRIM_RX       (uint8_t)0x01 // PRIM_RX bit in CONFIG register
#define nRF24_CONFIG_PWR_UP        (uint8_t)0x02 // PWR_UP bit in CONFIG register
#define nRF24_FLAG_RX_DR           (uint8_t)0x40 // RX_DR bit (data ready RX FIFO interrupt)
#define nRF24_FLAG_TX_DS           (uint8_t)0x20 // TX_DS bit (data sent TX FIFO interrupt)
#define nRF24_FLAG_MAX_RT          (uint8_t)0x10 // MAX_RT bit (maximum number of TX retransmits interrupt)

// Register masks definitions
#define nRF24_MASK_REG_MAP         (uint8_t)0x1F // Mask bits[4:0] for CMD_RREG and CMD_WREG commands
#define nRF24_MASK_CRC             (uint8_t)0x0C // Mask for CRC bits [3:2] in CONFIG register
#define nRF24_MASK_STATUS_IRQ      (uint8_t)0x70 // Mask for all IRQ bits in STATUS register
#define nRF24_MASK_RF_PWR          (uint8_t)0x06 // Mask RF_PWR[2:1] bits in RF_SETUP register
#define nRF24_MASK_RX_P_NO         (uint8_t)0x0E // Mask RX_P_NO[3:1] bits in STATUS register
#define nRF24_MASK_DATARATE        (uint8_t)0x28 // Mask RD_DR_[5,3] bits in RF_SETUP register
#define nRF24_MASK_EN_RX           (uint8_t)0x3F // Mask ERX_P[5:0] bits in EN_RXADDR register
#define nRF24_MASK_RX_PW           (uint8_t)0x3F // Mask [5:0] bits in RX_PW_Px register
#define nRF24_MASK_RETR_ARD        (uint8_t)0xF0 // Mask for ARD[7:4] bits in SETUP_RETR register
#define nRF24_MASK_RETR_ARC        (uint8_t)0x0F // Mask for ARC[3:0] bits in SETUP_RETR register
#define nRF24_MASK_RXFIFO          (uint8_t)0x03 // Mask for RX FIFO status bits [1:0] in FIFO_STATUS register
#define nRF24_MASK_TXFIFO          (uint8_t)0x30 // Mask for TX FIFO status bits [5:4] in FIFO_STATUS register
#define nRF24_MASK_PLOS_CNT        (uint8_t)0xF0 // Mask for PLOS_CNT[7:4] bits in OBSERVE_TX register
#define nRF24_MASK_ARC_CNT         (uint8_t)0x0F // Mask for ARC_CNT[3:0] bits in OBSERVE_TX register

// Fake address to test transceiver presence (5 bytes long)
#define nRF24_TEST_ADDR "nRF24"

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

// Auto Retransmit Delay
enum{
    AutoRetransmitDelayNone   = (uint8_t)0x00,
    AutoRetransmitDelay250us  = (uint8_t)0x00,
    AutoRetransmitDelay500us  = (uint8_t)0x01,
    AutoRetransmitDelay750us  = (uint8_t)0x02,
    AutoRetransmitDelay1000us = (uint8_t)0x03,
    AutoRetransmitDelay1250us = (uint8_t)0x04,
    AutoRetransmitDelay1500us = (uint8_t)0x05,
    AutoRetransmitDelay1750us = (uint8_t)0x06,
    AutoRetransmitDelay2000us = (uint8_t)0x07,
    AutoRetransmitDelay2250us = (uint8_t)0x08,
    AutoRetransmitDelay2500us = (uint8_t)0x09,
    AutoRetransmitDelay2750us = (uint8_t)0x0A,
    AutoRetransmitDelay3000us = (uint8_t)0x0B,
    AutoRetransmitDelay3250us = (uint8_t)0x0C,
    AutoRetransmitDelay3500us = (uint8_t)0x0D,
    AutoRetransmitDelay3750us = (uint8_t)0x0E,
    AutoRetransmitDelay4000us = (uint8_t)0x0F
};

// Data Rate
enum {
    DataRate250kbps = (uint8_t)0x20,
    DataRate1Mbps = (uint8_t)0x00,
    DataRate2Mbps = (uint8_t)0x08
};

// RF Power Output in TX mode
enum {
    TxPower18dBm = (uint8_t)0x00,
    TxPower12dBm = (uint8_t)0x02,
    TxPower6dBm = (uint8_t)0x04,
    TxPower0dBm = (uint8_t)0x06
};

// CRC Encoding scheme
enum{
    CRCOff = (uint8_t)0x00,
    CRC1Byte = (uint8_t)0x08,
    CRC2Byte = (uint8_t)0x0c
};

// Power Control
enum{
    PWRUp = (uint8_t)0x02,
    PWRDown = (uint8_t)0x00
};

// Transceiver mode
enum{
    RxMode = (uint8_t)0x01,
    TxMode = (uint8_t)0x00
};

// Enumeration of RX pipe address and TX address
enum{
    RxPipe0 = (uint8_t)0x00,
    RxPipe1 = (uint8_t)0x01,
    RxPipe2 = (uint8_t)0x02,
    RxPipe3 = (uint8_t)0x03,
    RxPipe4 = (uint8_t)0x04,
    RxPipe5 = (uint8_t)0x05,
    TxPipe = (uint8_t)0x06
};

// State of auto acknowledgment for specified pipe
enum{
    AutoACKOff = (uint8_t)0x00,
    AutoACKOn = (uint8_t)0x01
};

// Status of the RX fifo
enum{
    RxFifoData = (uint8_t)0x00,
    RxFifoEmpty = (uint8_t)0x01,
    RxFifoFull = (uint8_t)0x02,
    RxFifoError = (uint8_t)0x03
};

// Status of the TX fifo
enum{
    TxFifoData = (uint8_t)0x00,
    TxFifoEmpty = (uint8_t)0x01,
    TxFifoFull = (uint8_t)0x02,
    TxFifoError = (uint8_t)0x03
};

// Result of RX FIFO reading
typedef enum {
    Pipe0  = (uint8_t)0x00,
    Pipe1  = (uint8_t)0x01,
    Pipe2  = (uint8_t)0x02,
    Pipe3  = (uint8_t)0x03,
    Pipe4  = (uint8_t)0x04,
    Pipe6  = (uint8_t)0x05,
    Empty  = (uint8_t)0xff
}Nrf24RxResult;

// Addresses of the Rx PW_P# registers
static const uint8_t Nrf24RxPwPIPE[6] = {
    nRF24_REG_RX_PW_P0,
    nRF24_REG_RX_PW_P1,
    nRF24_REG_RX_PW_P2,
    nRF24_REG_RX_PW_P3,
    nRF24_REG_RX_PW_P4,
    nRF24_REG_RX_PW_P5
};

// Addresses of the address registers
static const uint8_t Nrf24AddrRegs[7] = {
    nRF24_REG_RX_ADDR_P0,
    nRF24_REG_RX_ADDR_P1,
    nRF24_REG_RX_ADDR_P2,
    nRF24_REG_RX_ADDR_P3,
    nRF24_REG_RX_ADDR_P4,
    nRF24_REG_RX_ADDR_P5,
    nRF24_REG_TX_ADDR
};


void initNrf24 (void);
uint8_t Nrf24WriteRead(uint8_t data);

uint8_t checkNrf24(void);

void setPowerModeNrf24(uint8_t mode);
void setOperationalModeNrf24(uint8_t mode);
void setCRCSchemeNrf24(uint8_t scheme);
void setRFChannelNrf24(uint8_t channel);
void setAutoRetransmitNrf24(uint8_t ard, uint8_t arc);
void setAddrWidthNrf24(uint8_t addr_width);
void setAddrNrf24(uint8_t pipe, const uint8_t *addr);

#endif // __NRF24_H
