#include "nRF24.h"

GPIO_InitTypeDef GPIOConfNrf24;
SPI_InitTypeDef SpiConfNrf24;

static void initSPIx(void)
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

static uint8_t SPIxWriteRead(SPI_TypeDef *SPI, uint8_t data){
    while (!(SPI->SR & SPI_SR_TXE)); // Wait while receive buffer is not empty
    SPI->DR = data; // Send byte to SPI
    while (!(SPI->SR & SPI_SR_RXNE)); // Wait while receive buffer is empty
    return SPI->DR; // Received byte
}

void initNrf24(void)
{
    initSPIx();
}

uint8_t Nrf24WriteRead(uint8_t data)
{
    return SPIxWriteRead(SPI1, data);
}

static uint8_t ReadRegNrf24(uint8_t reg)
{
    uint8_t value;

    CSN_NRF24_LOW();
    value = Nrf24WriteRead(reg & nRF24_MASK_REG_MAP); // TODO CHANGE TO CORRECT FLAG
    value = Nrf24WriteRead(0x00);
    CSN_NRF24_HIGH();

    return value;
}

static void WriteRegNrf24(uint8_t reg, uint8_t value)
{
    CSN_NRF24_LOW();
    if(reg < nRF24_CMD_W_REGISTER){
        Nrf24WriteRead(nRF24_CMD_W_REGISTER | (reg & nRF24_MASK_REG_MAP));
        Nrf24WriteRead(value);
    } else {
        Nrf24WriteRead(reg);
        if ((reg != nRF24_CMD_FLUSH_TX) && (reg != nRF24_CMD_FLUSH_RX) && \
                (reg != nRF24_CMD_REUSE_TX_PL) && (reg != nRF24_CMD_NOP)) {
            Nrf24WriteRead(value);
        }
    }
    CSN_NRF24_HIGH();
}

static void ReadMultipleRegNrf24(uint8_t reg, uint8_t *pBuf, uint8_t size)
{
    CSN_NRF24_LOW();
    Nrf24WriteRead(reg);
    while(size--){
        *pBuf++ = Nrf24WriteRead(nRF24_CMD_NOP);
    }
    CSN_NRF24_HIGH();
}

static void WriteMultipleRegNrf24(uint8_t reg, uint8_t *pBuf, uint8_t size) {
    CSN_NRF24_LOW();
    Nrf24WriteRead(reg);
    while (size--) {
        Nrf24WriteRead(*pBuf++);
    }
    CSN_NRF24_HIGH();
}

// Check if the nRF24L01 present
// return:
//   1 - nRF24L01 is online and responding
//   0 - received sequence differs from original
uint8_t checkNrf24(void) {
    uint8_t i;
    uint8_t rxbuf[5];
    uint8_t *ptr = (uint8_t *)nRF24_TEST_ADDR;

    // Write test TX address and read TX_ADDR register
    WriteMultipleRegNrf24(nRF24_CMD_W_REGISTER | nRF24_REG_TX_ADDR, ptr, 5);
    ReadMultipleRegNrf24(nRF24_CMD_R_REGISTER | nRF24_REG_TX_ADDR, rxbuf, 5);

    // Compare buffers, return error on first mismatch
    for (i = 0; i < 5; i++) {
        if (rxbuf[i] != *ptr++) return 0;
    }

    return 1;
}

// Control transceiver power mode
// input:
//   mode - new state of power mode, one of nRF24_PWR_xx values
void setPowerModeNrf24(uint8_t mode){
    uint8_t reg;
    reg = ReadRegNrf24(nRF24_REG_CONFIG);
    if(mode == PWRUp){
        reg |= nRF24_CONFIG_PWR_UP;
    } else {
        reg &= ~nRF24_CONFIG_PWR_UP;
    }
    WriteRegNrf24(nRF24_REG_CONFIG, reg);
}

// Set transceiver operational mode
// input:
//   mode - operational mode, one of nRF24_MODE_xx values
void setOperationalModeNrf24(uint8_t mode) {
    uint8_t reg;

    // Configure PRIM_RX bit of the CONFIG register
    reg  = ReadRegNrf24(nRF24_REG_CONFIG);
    reg &= ~nRF24_CONFIG_PRIM_RX;
    reg |= (mode & nRF24_CONFIG_PRIM_RX);
    WriteRegNrf24(nRF24_REG_CONFIG, reg);
}

// Set transceiver CRC scheme
// input:
//   scheme - CRC scheme, one of nRF24_CRC_xx values
// note: transceiver will forcibly turn on the CRC in case if auto acknowledgment
//       enabled for at least one RX pipe
void setCRCSchemeNrf24(uint8_t scheme) {
    uint8_t reg;
    // Configure EN_CRC[3] and CRCO[2] bits of the CONFIG register
    reg  = ReadRegNrf24(nRF24_REG_CONFIG);
    reg &= ~nRF24_MASK_CRC;
    reg |= (scheme & nRF24_MASK_CRC);
    WriteRegNrf24(nRF24_REG_CONFIG, reg);
}

// Set Frequency Channel
// input:
//  channel - radio frequency, value from 0 to 127
// note: frequency will be (2400 + channel)MHz
// note: PLOS_CNT[7:4] bits of the OBSERVER_TX register will be reset
void setRFChannelNrf24(uint8_t channel) {
    WriteRegNrf24(nRF24_REG_RF_CH, channel);
}

// Set Automatic retransmition parameters
// input:
//  ard - Auto retransmition delay, one of AutoRetransmitDelayXXX value
//  arc - count of auto retransmits, value from 0 to 15
// note: zero arc value means that the automatic retransmission disabled
void setAutoRetransmitNrf24(uint8_t ard, uint8_t arc){
    // Set auto retransmit settings (SETUP_RETR register)
    WriteRegNrf24(nRF24_REG_SETUP_RETR, (uint8_t)((ard << 4) | (arc & nRF24_MASK_RETR_ARC)));
}

// Set of address widths
// input:
//  addr_width - RX/TX address field width, value from 3 to 5
// note: this setting is common for all pipes
void setAddrWidthNrf24(uint8_t addr_width){
    WriteRegNrf24(nRF24_REG_SETUP_AW, addr_width - 2);
}


void setAddrNrf24(uint8_t pipe, const uint8_t *addr){
    uint8_t addr_width;

    // RX_ADDR_Px register
    switch (pipe) {
    case TxPipe:
    case RxPipe0:
    case RxPipe1:
        // Get address width
        addr_width = ReadRegNrf24((nRF24_REG_SETUP_AW) + 1);
                // Write address in reverse order (LSByte first)
                addr += addr_width;

        CSN_NRF24_LOW();
        Nrf24WriteRead(nRF24_CMD_W_REGISTER | Nrf24AddrRegs[pipe]);
        do {
            Nrf24WriteRead(*addr--);
        } while (addr_width--);
        CSN_NRF24_HIGH();

        break;

    case RxPipe2:
    case RxPipe3:
    case RxPipe4:
    case RxPipe5:
        // Write address LSBbyte (only first byte from the addr buffer)
        WriteRegNrf24(Nrf24AddrRegs[pipe], *addr);
        break;
    default:
        // Incorrect pipe number -> do nothing
        break;
    }
}
