#include <stdio.h>
#include <string.h>

#include <stm32f10x.h>
#include <stm32f10x_it.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>


#include <FreeRTOS.h>
#include <task.h>

#define Blink_Led_Priority                          ( tskIDLE_PRIORITY + 1 )

GPIO_InitTypeDef GPIO_InitStructure;

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
	/* RCC system reset(for debug purpose). */
	RCC_DeInit();

	/* Enable HSE. */
	RCC_HSEConfig( RCC_HSE_ON );

	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

	/* HCLK = SYSCLK. */
	RCC_HCLKConfig( RCC_SYSCLK_Div1 );

	/* PCLK2  = HCLK. */
	RCC_PCLK2Config( RCC_HCLK_Div1 );

	/* PCLK1  = HCLK/2. */
	RCC_PCLK1Config( RCC_HCLK_Div2 );

	/* Flash 2 wait state. */
	*( volatile unsigned long  * )0x40022000 = 0x01;

	/* PLLCLK = 8MHz * 9 = 72 MHz */
	RCC_PLLConfig( RCC_PLLSource_HSE_Div1, RCC_PLLMul_9 );

	/* Enable PLL. */
	RCC_PLLCmd( ENABLE );

	/* Wait till PLL is ready. */
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

	/* Select PLL as system clock source. */
	RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK);

	/* Wait till PLL is used as system clock source. */
	while (RCC_GetSYSCLKSource() != 0x08);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/* Configure PC.4 as Output push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

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
