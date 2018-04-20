#ifndef _IPC_EXAMPLE_H
#define _IPC_EXAMPLE_H

/* Must always be power of 2 */
#define IPCEX_QUEUE_SZ        64

#define IPCEX_MAX_IDS         20

#define IPCEX_ID_BLINKY        1
#define IPCEX_ID_USBHOST       2
#define IPCEX_ID_USBDEVICE     3
#define IPCEX_ID_LWIP          4
#define IPCEX_ID_EMWIN         5
#define IPCEX_ID_USER1         10
#define IPCEX_ID_USER2         11

typedef struct __ipcex_msg
{
	uint32_t id;
	uint32_t data;
}ipcex_msg_t;

#ifdef __cplusplus
extern "C" {
#endif

extern int ipcex_register_callback(uint32_t id, void (*func)(uint32_t));
extern void ipcex_tasks(void);
extern void IPCEX_Init(void);
extern int ipcex_msgPush(uint32_t id, uint32_t data);

#ifdef __cplusplus
}
#endif

#endif
