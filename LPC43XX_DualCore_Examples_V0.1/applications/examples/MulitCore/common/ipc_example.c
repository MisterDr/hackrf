#include "lpc43xx.h"
#include "ipc_msg.h"
#include "ipc_example.h"

#ifdef OS_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#elif defined(OS_UCOS_III)
#define UCOS_MIN_STACK_SZ         128
#define UCOS_IPCEX_TASK_PRIORITY  12
#include "os.h"
#endif

static void (*ipcex_callback_lookup[IPCEX_MAX_IDS])(uint32_t);

static ipcex_msg_t ipcex_queue[IPCEX_QUEUE_SZ];

int ipcex_register_callback(uint32_t id, void (*func)(uint32_t))
{
	if (id >= IPCEX_MAX_IDS)
		return 0;
	
	ipcex_callback_lookup[id] = func;
	return 1;
}

static void ipcex_dispatch_task(void *loop)
{
	int ret;
	ipcex_msg_t msg;
	do {
		if (loop)
			ret = IPC_popMsg(&msg);
		else
			ret = IPC_tryPopMsg(&msg);
		if (ret == QUEUE_VALID && msg.id < IPCEX_MAX_IDS) {
			if (ipcex_callback_lookup[msg.id])
				ipcex_callback_lookup[msg.id](msg.data);
		}
	}while(loop);
}

void ipcex_tasks(void)
{
#ifdef OS_FREE_RTOS
	/* Start Blinky event Task */
	xTaskCreate( ipcex_dispatch_task, ( signed char * ) "IPC Ex",
		configMINIMAL_STACK_SIZE, (void *)1, (tskIDLE_PRIORITY + 4UL),
		( xTaskHandle * ) NULL );
#elif defined(OS_UCOS_III)
	OS_ERR ret;
	static OS_TCB    mem_tcb;
	static CPU_STK   mem_stack[UCOS_MIN_STACK_SZ];

	OSTaskCreate   (
		&mem_tcb,
		"IPC Ex",
		ipcex_dispatch_task,
		(void *) 1,
		UCOS_IPCEX_TASK_PRIORITY,
		mem_stack,
		32,
		UCOS_MIN_STACK_SZ,
		0,
		0,
		(void *)0,
		(OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
		(OS_ERR *)&ret);
	if (ret != OS_ERR_NONE) {
		printf("Unable to create IPC ex task : ret = %x\r\n",ret);
		while (1);
	}
#else
	ipcex_dispatch_task(0);
#endif
}

void IPCEX_Init(void)
{
	IPC_initMsgQueue(ipcex_queue, sizeof(ipcex_msg_t), IPCEX_QUEUE_SZ);
}

int ipcex_msgPush(uint32_t id, uint32_t data)
{
	ipcex_msg_t msg;
	msg.id = id;
	msg.data = data;
#if (defined(OS_FREE_RTOS) || defined(OS_UCOS_III))
	return IPC_pushMsg(&msg);
#else
	return IPC_tryPushMsg(&msg);
#endif
}
