#include <lpc43xx.h>
#include "FreeRTOS.h"
#include "task.h"

void vApplicationIdleHook(void) __attribute__((weak));
void vApplicationIdleHook(void)
{
	__WFI();
}

void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName ) __attribute__((weak));
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName )
{
	while(1);
}

void vApplicationMallocFailedHook(void) __attribute__((weak));
void vApplicationMallocFailedHook(void)
{
	while(1);
}
