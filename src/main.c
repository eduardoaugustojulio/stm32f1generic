#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>

#include <usb/usbcdc.h>

#include "spi.h"

static void blink(void *arg __attribute__((unused))){
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO13);

    for (;;) {
        gpio_toggle(GPIOC,GPIO13);
        vTaskDelay(pdMS_TO_TICKS(400));
    }
}

static void usb_hello_word(void *arg __attribute__((unused))){
    usb_getc();
    for(;;) {
        usb_printf("hello-wolrd!\n");
    }
}


static void spi_task(){
    const char *str = "hello-word";

    spi1_init();

    for(;;){
        spi1_nss_set(false);
        
        for(int i = 0; i < strlen(str); i++){
            spi1_xfer_byte(str[i]);
        }
        
        spi1_nss_set(true);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void){
    rcc_clock_setup_in_hse_8mhz_out_72mhz();  
    
    xTaskCreate(blink, "blink", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(spi_task, "spi_task",1024, NULL, configMAX_PRIORITIES-1, NULL);
    
    vTaskStartScheduler();
    
    for (;;);

    return 0;    
}
