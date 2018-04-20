/*
* Copyright(C) NXP Semiconductors, 2011
* All rights reserved.
*
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* LPC products.  This software is supplied "AS IS" without any warranties of
* any kind, and NXP Semiconductors and its licensor disclaim any and
* all warranties, express or implied, including all implied warranties of
* merchantability, fitness for a particular purpose and non-infringement of
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights under any
* patent, copyright, mask work right, or any other intellectual property rights in
* or to any products. NXP Semiconductors reserves the right to make changes
* in the software without notification. NXP Semiconductors also makes no
* representation or warranty that such application will be suitable for the
* specified use without further testing or modification.
*
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors' and its
* licensor's relevant copyrights in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
*/

/**
 * \file Example_Blinky.c
 * Example program that blinks LEDs
 * This example blinks LED1 and LED2 at various rates, one of the LED
 * is turned on/off by M4 based on message received from M0. Meanwhile
 * M4 also sends blink message to M0 so that it blinks the other LED.
 */

#include "lpc43xx.h"
#include "ipc_msg.h"
#include "ipc_example.h"
#include "bsp.h"

#define CPU_FREQUENCY                (96000000UL)
#ifdef CORE_M4
#define BLINKY_DEFAULT_DELAY         1000
#define BLINK_LED                    LEDS_LED2
#elif defined(CORE_M0)
#define BLINK_LED                    LEDS_LED1
#define BLINKY_DEFAULT_DELAY         500
#else
#error "LPC43XX must have CORE_M0 or CORE_M4 defined!"
#endif

#ifdef OS_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#elif defined(OS_UCOS_III)
#define UCOS_MIN_STACK_SZ            128
#define UCOS_EVENT_TASK_PRIORITY     12
#include "os.h"
#else /* No OS configuration */
/* Nothing to do here */
#endif

/**
 * Delay to be used for blinking the LEDs
 */
static const uint32_t xDelay = BLINKY_DEFAULT_DELAY;
static void LED_Event_Task(void *loop);

#ifdef OS_FREE_RTOS
/**
 * Delay function used for blinking LED (FreeRTOS version)
 *
 * \brief Calling this function will cause a delay of \a xDelay
 * \return 0 when the time has lapsed else non zero
 */
static int blink_delay(void)
{
		vTaskDelay(xDelay);
		return 0;
}

/**
 * FreeRTOS task create for Blinky
 *
 * Calling this function creates the tasks in FreeRTOS
 * which will blink the LEDs
 */
void blinky_tasks(void)
{
	/* Start Blinky event Task */
	xTaskCreate( LED_Event_Task, ( signed char * ) "LED Event",
		configMINIMAL_STACK_SIZE, (void *)1, (tskIDLE_PRIORITY + 3UL),
		( xTaskHandle * ) NULL );
}
#elif defined(OS_UCOS_III)
/**
 * Delay function used for blinking LED (uCOS-III Version)
 *
 * \brief Calling this function will cause a delay of \a xDelay
 * \return 0 when the time has lapsed else non zero
 */
static int blink_delay(void)
{
		OS_ERR ret;
		OSTimeDlyHMSM(0,0,xDelay/1000,xDelay%1000,OS_OPT_TIME_HMSM_STRICT,&ret);
		return ret != OS_ERR_NONE;
}

void blinky_tasks(void)
{
	OS_ERR ret;
	static OS_TCB    mem_tcb;
	static CPU_STK   mem_stack[UCOS_MIN_STACK_SZ];

	OSTaskCreate   (
		&mem_tcb,
		"Event Tsk",
		LED_Event_Task,
		(void *) 1,
		UCOS_EVENT_TASK_PRIORITY,
		mem_stack,
		32,
		UCOS_MIN_STACK_SZ,
		0,
		0,
		(void *)0,
		(OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
		(OS_ERR *)&ret);
	if (ret != OS_ERR_NONE) {
		printf("Unable to create event task!\r\n");
		while (1);
	}
}
#else

/**
 * Delay function used for blinking LED (noOS version)
 *
 * \brief Calling this function will cause a delay of \a xDelay
 * \return 0 when the time has lapsed else non zero
 */
static int blink_delay(void)
{
	static int32_t final, init;
	if (!init) {
		int32_t curr = (int32_t) LPC_RITIMER->COUNTER;
		final = curr + CPU_FREQUENCY / (1000 / xDelay);
		init = 1 + (final < 0 && curr > 0);
	}

	if ((init == 2 && LPC_RITIMER->COUNTER >= (uint32_t) final) ||
		(init == 1 && (int32_t) LPC_RITIMER->COUNTER >= final)) {
			init = 0;
	}
	return init != 0;
}

/**
 * Standalone tasks for blinking
 *
 * Calling this function does the task of
 * blinking the LEDs and sending the Event to the
 * other processor.
 */
void blinky_tasks(void)
{
	LED_Event_Task((void *) 0);
}
#endif

/**
 * Blink LED based on event from M0/M4
 *
 * This function when called from M0 will blink the LED
 * based on message sent by M4 and vice-versa.
 */
static void LED_blinkProc(uint32_t val)
{
	LEDs_ChangeLEDs(val & 0xFFFF, (val >> 16) & 0xFFFF);
}

/**
 * Send blink event to M4/M0 from M0/M4
 *
 * This function if called from M4 will send the blink
 * event to M0 and vice-versa.
 */
static void LED_Event_Task(void *loop)
{
	static int blink = BLINK_LED;

#if defined(OS_UCOS_III)
	OS_CSP_TickInit();
#endif	
	
	do {
		if (!blink_delay()) {
			if (ipcex_msgPush(IPCEX_ID_BLINKY, (BLINK_LED << 16) | blink) == QUEUE_INSERT)
				blink ^= BLINK_LED;
		}
	}while(loop);
}

void BLINKY_Init(void)
{
	ipcex_register_callback(IPCEX_ID_BLINKY, LED_blinkProc);
}
