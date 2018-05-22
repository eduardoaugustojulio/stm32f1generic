#include <stdio.h>
#include <string.h>

#include <stm32f10x.h>

#include <FreeRTOS.h>
#include <task.h>

#define Blink_Led_Priority                          ( tskIDLE_PRIORITY + 1 )


static void prvSetupHardware(void);
static void prvBlinkLed(void *pvParameters);


int main(){
	prvSetupHardware();

	xTaskCreate(prvBlinkLed, "Blink", configMINIMAL_STACK_SIZE, NULL, Blink_Led_Priority, NULL);

	vTaskStartScheduler();

	for(;;);

	return 0;
}

static void prvSetupHardware(void)
{

}

static void prvBlinkLed(void *pvParameters)
{

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
