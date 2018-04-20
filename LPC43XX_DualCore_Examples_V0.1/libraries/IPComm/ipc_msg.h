#ifndef __IPCOMM__H
#define __IPCOMM__H

/* Do not modify these */
/** NOTE: Queue size should only be in powers
 *        of 2. ie., 1, 2, 4, 8, 16 etc..
 **/
#define QUEUE_SZ_M0           64
#define QUEUE_SZ_MASK_M0      (QUEUE_SZ - 1)

#define QUEUE_DATA_COUNT(q) ((uint32_t)((q)->head - (q)->tail))
#define QUEUE_IS_FULL(q)    (QUEUE_DATA_COUNT(q) >= (q)->size)
#define QUEUE_IS_EMPTY(q)   ((q)->head == (q)->tail)

/*!< \struct ipc_queue
 * Struct used for sync between M0 and M4
 * This structure provides head and tail indexes
 * to the uint32_t array of messages.
 */
struct ipc_queue
{
	int32_t size;              /*!< Size of a single item in queue */
	int32_t count;             /*!< Toal number of elements that can be stored in the queue */
	volatile uint32_t head;    /*!< Head index of the queue */
	volatile uint32_t tail;    /*!< Tail index of the queue */
	uint8_t *data;    /*!< Pointer to the data */
	uint32_t reserved[3];      /*!< Reserved entry to keep the structure aligned */
};

/**
 * Queue error values
 */
#define QUEUE_VALID      1
#define QUEUE_INSERT     0
#define QUEUE_FULL      -1
#define QUEUE_EMPTY     -2
#define QUEUE_ERROR     -3
#define QUEUE_TIMEOUT   -4

#ifdef __cplusplus
extern "C" {
#endif

/* Function like macro defines */
#define IPC_tryPushMsg(data) IPC_pushMsgTout(data, 0)
#define IPC_tryPopMsg(data)  IPC_popMsgTout(data, 0)
#define IPC_pushMsg(data)    IPC_pushMsgTout(data, -1)
#define IPC_popMsg(data)     IPC_popMsgTout(data, -1)

int IPC_msgPending(void);
int IPC_popMsgTout(void *data, int tout);
int IPC_pushMsgTout(const void *data, int tout);
void IPC_msgNotify(void);

/* IMPORTANT NOTE: MaxNoOfMsg must always be a power of 2 */
void IPC_initMsgQueue(void *data, int msgSize, int maxNoOfMsg);
int IPC_bootM0(uint32_t m0_image_addr);
const char *IPC_strerror(int errnum);

#ifdef __cplusplus
}
#endif

#endif /* ifndef __IPCOMM__H */
