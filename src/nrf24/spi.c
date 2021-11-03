#include "spi.h"

void spi1_nss_set(uint8_t level){
    (level == 0) ? gpio_clear(GPIOA, GPIO4) : gpio_set(GPIOA, GPIO4);
}

void spi1_init(){
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_SPI1);

    /* Configure NSS */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO4);

    /* Configure SCK */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5);
    
    /* Configure MOSI */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO7);
    
    /* Configure MISO */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6); 
    
    spi_reset(SPI1);
 
    spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
        SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

    spi1_nss_set(true);

    spi_enable(SPI1);
}

uint8_t spi1_xfer_byte(uint8_t byte){
    return spi_xfer(SPI1, byte);
}

void spi1_write(uint8_t byte){
    spi_send(SPI1, byte);
}


