#include "bsp.h"
#include "ipc_msg.h"
#include "ipc_example.h"

void Key_Pressed_Proc (uint32_t key)
{
	/* Toggle LEDS_LED3 for every key press */
	static uint32_t blink = LEDS_LED3;

	if (key == '\n')
		printf("\r");

	printf("%c", key);

	if (ipcex_msgPush(IPCEX_ID_BLINKY, (LEDS_LED3 << 16) | blink) == QUEUE_INSERT)
		blink ^= LEDS_LED3;
}

void USBHOST_Init(void)
{
		ipcex_register_callback(IPCEX_ID_USBHOST, Key_Pressed_Proc);
}

void usb_host_tasks(void)
{

}
