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
 * \file ipc_queue.c
 * Inter-Processor communication module
 * This file provides functions necessary to communicate between
 * M0 and M4 processors in LPC43XX. This module uses message queues
 * to get the communication.
 */
#include <string.h>
#include "lpc43xx.h"
#include "ipc_msg.h"

/**
 * M0 write queue
 * This is the queue which will be written by M0 and be read by M4
 * \note
 * This variable is put in SHMEMM0 section which will
 * be linked to shared memory region by linker script.
 */
static struct ipc_queue queue_m0 __attribute__((section("SHMEMM0"))) = {0};

/**
 * M4 write queue
 * This is the queue which will be written by M4 and be read by M0
 * \note
 * This variable is put in SHMEMM4 section which will
 * be linked to shared memory region by linker script.
 */
static struct ipc_queue queue_m4 __attribute__((section("SHMEMM4"))) = {0};

/**
 * IRQ priority of IPC interrupt
 * Currently this has the highest priority
 */
#define IPC_IRQ_Priority    7

#ifdef CORE_M4
/**
 * M4 read queue pointer
 * M4 reads from this message queue based on the
 * tail index.
 */
static struct ipc_queue *qrd = &queue_m0;
/**
 * M4 write queue pointer
 * M4 writes from this message queue based on the
 * head index.
 */
static struct ipc_queue *qwr = &queue_m4;
#define IPC_IRQHandler M0CORE_IRQHandler
#define TXEVENT        M0TXEVENT
#define IPC_IRQn       M0CORE_IRQn
#elif defined(CORE_M0)
static struct ipc_queue *qrd = &queue_m4;
static struct ipc_queue *qwr = &queue_m0;
#define IPC_IRQHandler M0_M4CORE_IRQHandler
#define TXEVENT        M4TXEVENT
#define IPC_IRQn       M0_M4CORE_IRQn
#else
#error "For LPC43XX, CORE_M0 or CORE_M4 must be defined!"
#endif

/* FreeRTOS functions */
#ifdef OS_FREE_RTOS
#include "FreeRTOS.h"
#include "semphr.h"

static xSemaphoreHandle event_tx, event_rx;
static void os_event_handler(void)
{
	portBASE_TYPE wake1 = pdFALSE, wake2 = pdFALSE;

	if (event_rx && !QUEUE_IS_EMPTY(qrd))
		xSemaphoreGiveFromISR(event_rx, &wake1);
	
	if (event_tx && !QUEUE_IS_FULL(qwr))
		xSemaphoreGiveFromISR(event_tx, &wake2);
	
	portEND_SWITCHING_ISR(wake1 || wake2);
}

static void ipc_misc_init(void)
{
	vSemaphoreCreateBinary (event_tx);
	vSemaphoreCreateBinary (event_rx);
	
	if (!event_tx || !event_rx)
		while(1); /* BUG: unable to create semaphores */
}

#define ipc_wait_event(evt, sem) while ((evt)) xSemaphoreTake(sem, 100)
#define ipc_wait_event_tout(evt,tout,sem) \
	do { \
		while((evt)) \
			if ((evt) && xSemaphoreTake(sem, tout) != pdTRUE) \
				{tout = 0; break;} \
	}while (0)


#elif defined(OS_UCOS_III)
/***************************************************************************
 *                          uCOS-III adapter functions                     *
 ***************************************************************************/
#include "os.h"

static OS_SEM event_tx, event_rx;
static void ipc_misc_init(void)
{
	OS_ERR ret;
	OSSemCreate (&event_tx, "TX Sema", 0, &ret);
	if (ret != OS_ERR_NONE) {
		while (1);
	}
	OSSemCreate (&event_rx, "RX Sema", 0, &ret);
	if (ret != OS_ERR_NONE) {
		while (1);
	}
}

static void os_event_handler(void)
{
	OS_ERR ret;
	if (!QUEUE_IS_EMPTY(qrd))
		OSSemPost(&event_rx, OS_OPT_POST_ALL, &ret);
	
	if (!QUEUE_IS_FULL(qwr))
		OSSemPost(&event_tx, OS_OPT_POST_ALL, &ret);
}

/* Wait for the event */
#define ipc_wait_event(evt,sem) \
	while ((evt)) { OS_ERR ret; \
		OSSemPend(&(sem), (OS_TICK) 100, OS_OPT_PEND_BLOCKING, (CPU_TS*)0, &ret); }
#define ipc_wait_event_tout(evt,tout,sem) \
		do { \
			while((evt)) { \
				OS_ERR ret; \
				OSSemPend(&(sem), (OS_TICK) tout, OS_OPT_PEND_BLOCKING, (CPU_TS*)0, &ret); \
				if ((evt) && ret == OS_ERR_TIMEOUT) {tout = 0; break;} \
			} \
		}while(0)
/************************** End of uCOS-III adapter functions **************/
#else
/***************************************************************************
 *                           OS less adapter functions                     *
 ***************************************************************************/
#define CPU_FREQ     96000000UL
static void os_event_handler(void)
{
	/* Nothing to do here */
}

/* Wait for the event */
#define ipc_wait_event(evt,sem) while((evt))

#define ipc_wait_event_tout(evt,tout,sem) \
	do {\
		uint32_t cnt = LPC_RITIMER->COUNTER + (tout * (CPU_FREQ / 1000)); \
		if (cnt + 5000 < cnt) cnt += 5000; \
		while ((evt) && LPC_RITIMER->COUNTER < cnt); \
		if (evt) tout = 0; \
	}while(0)

static void ipc_misc_init(void)
{
}
/************** End of OS less adapter functions *********************/
#endif

/**
 * Initiate interrupt on other processor
 * Upon calling this function generates and interrupt on the other
 * core. Ex. if called from M0 core it generates interrupt on M4 core
 * and vice versa.
 */
static void ipc_send_signal(void)
{
	__DSB();
	__sev();
}

/**
 * Sends notification interrupt
 *
 * When called from M0 sends notification interrupt to M4 and vice-versa.
 */
void IPC_msgNotify(void)
{
	ipc_send_signal();
}

/**
 * \brief Function to initialize the IPC message queue
 *
 * This function intializes the interprocessor communication message queue
 * \param data Pointer to the array of 32bit unsigned integers to be used as queue
 * \param size size of the \a data element
 * \param count Maximum number of items in Queue
 */
void IPC_initMsgQueue(void *data, int size, int count)
{
	/* Sanity Check */
	if (!size || !count || !data)
		while(1);

	/* Check if size is a power of 2 */
	if (count & (count - 1))
		while(1); /* BUG: Size must always be power of 2 */

	ipc_misc_init();
	qwr->count = count;
	qwr->size = size;
	qwr->data = data;
	NVIC_SetPriority(IPC_IRQn, IPC_IRQ_Priority);
	NVIC_EnableIRQ(IPC_IRQn);
}

/**
 * \brief Function to push message into IPC queue
 *
 * This function will push an 32-bit unsigned data as a message
 * to the other processor. i.e., if called from M0 it pushes the
 * message to M4 and vice-versa.
 * \param data 32-bit unsigned integer data to be pushed
 * \param tout Timeout in milliseconds
 * \return QUEUE_INSERT on success, QUEUE_ERROR or QUEUE_FULL on failure
 *         QUEUE_TIMEOUT when there is a timeout
 */
int IPC_pushMsgTout(const void *data, int tout)
{
	/* Check if write queue is initialized */
	if (!qwr->size)
		return QUEUE_ERROR;

	if (tout == 0) {
		/* Check if queue is full */
		if (QUEUE_IS_FULL(qwr))
			return QUEUE_FULL;
	} else if (tout < 0) {
		/* Wait for read queue to have some data */
		ipc_wait_event (QUEUE_IS_FULL(qwr), event_tx);
	} else {
		/* Wait for read queue to have some data */
		ipc_wait_event_tout (QUEUE_IS_FULL(qwr), tout, event_tx);
		if (tout == 0)
			return QUEUE_TIMEOUT;
	}

/*	qwr->data[qwr->head & (qwr->size - 1)] = data; */
	memcpy(qwr->data + ((qwr->head & (qwr->count - 1)) * qwr->size), data, qwr->size);
	qwr->head ++;
	ipc_send_signal();

	return QUEUE_INSERT;
}

int IPC_popMsgTout(void *data, int tout)
{
#ifdef EVENT_ON_RX
	int raise_event = QUEUE_IS_FULL(qrd);
#endif

	if (!qrd->size)
		return QUEUE_ERROR;

	if (tout == 0) {
		/* Check if read queue is empty */
		if (QUEUE_IS_EMPTY(qrd))
		return QUEUE_EMPTY;
	}	else if (tout < 0) {
		/* Wait for read queue to have some data */
		ipc_wait_event (QUEUE_IS_EMPTY(qrd), event_rx);
	} else {
		/* Wait for event or timeout */
		ipc_wait_event_tout(QUEUE_IS_EMPTY(qrd), tout, event_rx);
		if (tout == 0)
			return QUEUE_TIMEOUT;
	}

	/* Pop the queue Item */
/*	*data = qrd->data[qrd->tail & (qrd->size - 1)]; */
	memcpy(data, qrd->data + ((qrd->tail & (qrd->count - 1)) * qrd->size), qrd->size);
	qrd->tail ++;

#ifdef EVENT_ON_RX
	if (raise_event)
		ipc_send_signal();
#endif
	return QUEUE_VALID;
}

int IPC_msgPending(void)
{
	return QUEUE_DATA_COUNT(qrd);
}

/* IPC Event handler, can be overridden by user */
void ipc_event_handler(void) __attribute__((weak));
void ipc_event_handler(void)
{
}

/* Queue event handler */
void IPC_IRQHandler(void)
{
	/* Clear the interrupt */
	LPC_CREG->TXEVENT = 0x0;

	/* Invoke OS Specific handler */
	os_event_handler();

	/* Invoke handler */
	ipc_event_handler();
}

const char *IPC_strerror(int errnum)
{
	static const char *ipc_errstr[] = {
		"Queue Insert OK",
		"Queue Pop OK/Valid",
		"Queue is Full",
		"Queue is Empty",
		"Queue Error/Not initialized",
		"Queue operation timed out",
	};
	if (errnum < 0) errnum = 1 - errnum;
	return ipc_errstr[errnum];
}

#ifdef CORE_M4 /* Start M0 from M4 core */
int IPC_bootM0(uint32_t m0_image_addr)
{
	/* Make sure the alignment is OK */
	if (m0_image_addr & 0xFFF)
		return -1;

	/* Make sure the M0 core is being held in reset via the RGU */
	LPC_RGU->RESET_CTRL1 = (1 << 24);

	LPC_CCU1->CLK_M4_M0APP_CFG = 1;

	/* Keep in mind the M0 image must be aligned on a 4K boundary */
	LPC_CREG->M0APPMEMMAP = m0_image_addr;

	LPC_RGU->RESET_CTRL1 = 0;

	return 0;
}
#endif
