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
 * \file common/main.c
 * Startup file (having reset and main routines)
 * This file provides functions necessary to start all the example tasks
 * based on the configuration.
 */
#include <stdio.h>
#include "lpc43xx.h"
#include "lpc43xx_cgu.h"
#include "bsp.h"
#include "debug_frmwrk.h"
#include "ipc_msg.h"

#ifdef OS_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifdef OS_UCOS_III
#include "os.h"
#endif

extern unsigned char LR0[];

#ifdef OS_FREE_RTOS
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName )
{
	printf ("Stack over flow in %s\r\n", pcTaskName);
	while(1);
}

void vApplicationMallocFailedHook(void)
{
	printf("Malloc failure\r\n");
	while(1);
}
#endif

#ifdef CORE_M4
/* Reset Handler for M4 startup */
extern void fpuInit(void);
extern void fpuEnable(void);
extern void __main(void);

#if defined (OS_UCOS_III)
void FP_context_save_disable()
{
	uint32_t *FPCCR = (uint32_t *)0xE000EF34;
	*FPCCR = 0x0;
}
#endif

void Reset_Handler(void)
{
#if defined (OS_UCOS_III)
	FP_context_save_disable();
#endif

	fpuInit();
	fpuEnable();
	SystemInit();
	bsp_init(); /* We need this for scatter loading on SDRAM */
	__main();

	/* Control should never come here */
	while(1);
}
#endif

/* Currently 64 items queue is used in both M0
 * and M4. If the queue size for M0 and M4 has
 * to be different use the below declarations
 * inside the CORE_M0 and CORE_M4 blocks below
 **/
#ifndef IPCQUEUE_SZ
#define IPCQUEUE_SZ      64
#endif

extern void IPCEX_Init(void);
extern void USBHOST_Init(void);
extern void USBDEV_Init(void);
extern void LWIP_Init(void);
extern void EMWIN_Init(void);
extern void BLINKY_Init(void);

static void prvSetupHardware(void)
{
	extern uint32_t CGU_ClockSourceFrequency[];
	
#ifdef CORE_M0
	extern void prvSetupTimerInterrupt( void );
	extern uint32_t getPC(void);
	CGU_ClockSourceFrequency[CGU_CLKSRC_XTAL_OSC] = 12000000;

	bsp_misc_init();
	/**
	 * FIXME: Keil M0 startup code bug!
	 * This is not required for other compilers.
	 **/
	getPC();

	/* Update Clock Frequency */
	CGU_UpdateClock();

	#ifdef OS_FREE_RTOS
	prvSetupTimerInterrupt();
	#endif
#elif defined(CORE_M4)
	const unsigned char * const m0_image_addr = LR0;
	
	/* Re-initialize CGU for proper operation */
	CGU_Init();
	bsp_misc_init();

	/* Hardware Initialization */
	debug_frmwrk_init();
	LEDs_Init();

	/* Create a stdio stream for the serial port for stdin and stdout */
	Serial_CreateStream(NULL);

	/* Time to Start M0 */
	IPC_bootM0((uint32_t)m0_image_addr);

	/* Wait for M0 to finish his initialization */
	do {
		volatile int tout = 0x2FFFF;
		while(tout --);
	}while(0);
#endif

	/* Initialize the IPC Queue */
  IPCEX_Init();

	#ifdef EXAMPLE_USB_HOST
	USBHOST_Init();
	#endif
	#ifdef EXAMPLE_USB_DEVICE
	USBDEV_Init();
	#endif
	#ifdef EXAMPLE_LWIP
	LWIP_Init();
	#endif
	#ifdef EXAMPLE_EMWIN
	EMWIN_Init();
	#endif
	#ifdef EXAMPLE_BLINKY
	BLINKY_Init();
	#endif
}

extern void ipcex_tasks(void);
extern void blinky_tasks(void);
extern void usb_host_tasks(void);
extern void usb_device_tasks(void);
extern void lwip_tasks(void);
extern void emwin_tasks(void);

static void main_tasks(void)
{
	#if (defined(OS_FREE_RTOS) || defined(OS_UCOS_III))
	const int loop = 0;
	#else
	const int loop = 1;
	#endif

	#ifdef OS_UCOS_III
	//extern void OS_CSP_TickInit(void);
	OS_ERR ret;
	OSInit(&ret);
	if (ret != OS_ERR_NONE) {
		printf ("Unable init UCOS-III OS!\r\n");
		while(1);
	}
	#endif

	do {
		ipcex_tasks();
		#ifdef EXAMPLE_BLINKY
		blinky_tasks();
		#endif
		#ifdef EXAMPLE_USB_HOST
		usb_host_tasks();
		#endif
		#ifdef EXAMPLE_USB_DEVICE
		usb_device_tasks();
		#endif
		#ifdef EXAMPLE_LWIP
		lwip_tasks();
		#endif
		#ifdef EXAMPLE_EMWIN
		emwin_tasks();
		#endif
	}while (loop);

	#ifdef OS_FREE_RTOS
	/* Start the scheduler */
	vTaskStartScheduler();
	#endif

	#ifdef OS_UCOS_III
	//OS_CSP_TickInit();
	OSStart(&ret);
	if (ret != OS_ERR_NONE) {
		printf ("Unable start UCOS-III OS!\r\n");
		while(1);
	}
	#endif

	/* Control should never come here */
	printf("Schedule Failure\r\n");
	while(1);
}

int main(void)
{
	prvSetupHardware();
	printf("Init hw done\r\n");
	main_tasks();
	return 0;
}
