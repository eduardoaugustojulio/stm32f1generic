#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>

#include <usbcdc.h>

static void
flasher(void *arg __attribute__((unused))) {

    for (;;) {
            gpio_toggle(GPIOC,GPIO13);
            vTaskDelay(pdMS_TO_TICKS(400));
    }

}

void
usb_hello_word(void) {

    usb_getc();
    for(;;){
        usb_printf("hello-word!\n");
    }
}

int
main (void) {

    rcc_clock_setup_in_hse_8mhz_out_72mhz();    // Use this for "blue pill"
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO13);
    
    usb_start(1,1);
   
    xTaskCreate(usb_hello_word, "hello-word", 300, NULL, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(flasher, "flash", 100, NULL, configMAX_PRIORITIES-1, NULL);

    vTaskStartScheduler();
    for (;;);
    return 0;    
}
