#include <stdio.h>
#include <string.h>

#include <stm32f10x.h>
#include <stm32f10x_it.h>
#include <stm32f10x_conf.h>

#include <FreeRTOS.h>
#include <task.h>

#define Blink_Led_Priority                          ( tskIDLE_PRIORITY )

GPIO_InitTypeDef GPIO_InitStructure;

static void prvSetupHardware(void);
static void prvBlinkLed(void *pvParameters);


int main(){
	prvSetupHardware();

	xTaskCreate(prvBlinkLed, "Blink", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	vTaskStartScheduler();

	for(;;);

	return 0;
}

static void prvSetupHardware(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/* Configure PC.4 as Output push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Set C13 to Low level ("0")
}

static void prvBlinkLed(void *pvParameters)
{
	const TickType_t xDelay = 500;
	for(;;){
		GPIOC->ODR ^= GPIO_Pin_13;
		vTaskDelay(xDelay / portTICK_RATE_MS);
	}

}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* This function will get called if a task overflows its stack.   If the
		parameters are corrupt then inspect pxCurrentTCB to find which was the
		offending task. */
    ( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}

void assert_failed( unsigned char *pucFile, unsigned long ulLine )
{
	( void ) pucFile;
	( void ) ulLine;

	for( ;; );
}
