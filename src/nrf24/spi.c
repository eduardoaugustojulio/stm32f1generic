#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

void spi_init()
{
    rcc_periph_clock_enable(RCC_SPI1);
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Configure NSS */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO_SPI1_NSS);
    /* Configure SCK */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_SPI1_SCK);
    /* Configure MOSI */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_SPI1_MOSI);
    /* Configure MISO */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_SPI1_MISO);    
    
   
    spi_set_bidirectional_mode(SPI1);        
    spi_set_dff_8bit(SPI1);
    spi_set_full_duplex_mode(SPI1);
    spi_send_msb_first(SPI1);

    spi_enable_software_slave_management(SPI1);
    spi_set_nss_high(SPI1);

    spi_set_baudrate_prescaler(SPI1, SPI_CR1_BR_FPCLK_DIV_8);

    spi_set_master_mode(SPI1);
    spi_set_clock_polarity_0(SPI1);

    spi_set_clock_phase_0(SPI1);
    spi_enable_ss_output(SPI1);

    spi_enable(SPI1);
}

void spi_css(uint8_t level) 
{
    (level == 0) ? gpio_clear(GPIOA, GPIO_SPI1_NSS) : gpio_set(GPIOA, GPIO_SPI1_NSS);
}

uint8_t spi_xfer_byte(uint8_t byte)
{
    return spi_xfer(SPI1, byte);
}
