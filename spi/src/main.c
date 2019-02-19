#include <stdio.h>
#include <string.h>

#include <stm32f10x.h>
#include <stm32f10x_it.h>
#include <stm32f10x_conf.h>

#include <spi.h>

static void SetupHardware(void);

int main(){
	SetupHardware();
	initSPIx();

	for(;;);

	return 0;
}

static void SetupHardware(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

}
