#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

static void blink_led(void *arg __attribute((unused))) {

    for (;;) {
        const TickType_t xDelay = 500;
        gpio_toggle(GPIOC, GPIO13);
        vTaskDelay(xDelay / portTICK_RATE_MS);
    }
}

int main(void) {

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    xTaskCreate(blink_led, "blink", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);
    vTaskStartScheduler();
    }
