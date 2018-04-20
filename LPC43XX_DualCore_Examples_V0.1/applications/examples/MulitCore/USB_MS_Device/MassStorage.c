/*
* Copyright(C) NXP Semiconductors, 2012
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


/** \file
 *
 *  Main source file for the MassStorage demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "MassStorage.h"
#include "ipc_msg.h"
#include "ipc_example.h"

/** nxpUSBlib Mass Storage Class driver interface configuration and state information. This structure is
 *  passed to all Mass Storage Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_MS_Device_t Disk_MS_Interface =
	{
		.Config =
			{
				.InterfaceNumber           = 0,

				.DataINEndpointNumber      = MASS_STORAGE_IN_EPNUM,
				.DataINEndpointSize        = MASS_STORAGE_IO_EPSIZE,
				.DataINEndpointDoubleBank  = false,

				.DataOUTEndpointNumber     = MASS_STORAGE_OUT_EPNUM,
				.DataOUTEndpointSize       = MASS_STORAGE_IO_EPSIZE,
				.DataOUTEndpointDoubleBank = false,

				.TotalLUNs                 = TOTAL_LUNS,
			},
	};

#define USB_IRQ_Priority       8
void usb_msdev_task(void);
#ifdef OS_FREE_RTOS
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

static xSemaphoreHandle usb_dev_event;
void USBMSDev_task(void *arg)
{
	vSemaphoreCreateBinary(usb_dev_event);
	if (usb_dev_event == NULL){
		printf("Unable to create semaphore!\r\n");
		while(1);
	}
	while (1) {
		xSemaphoreTake(usb_dev_event, 100);
		usb_msdev_task();
	}
}

void device_xfer_event(int x)
{
	portBASE_TYPE wake = pdFALSE;
	if (usb_dev_event)
		xSemaphoreGiveFromISR(usb_dev_event, &wake);
	portEND_SWITCHING_ISR(wake);
}

void usb_device_tasks(void)
{
	/* Start Blinky event Task */
	xTaskCreate( USBMSDev_task, ( signed char * ) "USB Task",
		configMINIMAL_STACK_SIZE + 128, NULL, (tskIDLE_PRIORITY + 2UL),
		( xTaskHandle * ) NULL );
}
#elif defined(OS_UCOS_III)
#else
void usb_device_tasks(void)
{
	usb_msdev_task();
}
#endif
/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
void usb_msdev_task(void)
{
	#if !defined(USB_DEVICE_ROM_DRIVER)
	MS_Device_USBTask(&Disk_MS_Interface);
	USB_USBTask();
	#endif
	if(MassStorage_FlushTimeOut())
	{
		MassStorage_Cache_Flush(true);
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void USBDEV_Init(void)
{
#ifdef __USE_DOSFS
static void print_vol_info(void);
print_vol_info();
#endif

	NVIC_SetPriority(USB0_IRQn, USB_IRQ_Priority);

#if 1//defined(__LPC18XX__)
	/* Init Mass Storage Memory */
	MassStorage_Init();
#endif
	USB_Init();
#if defined(USB_DEVICE_ROM_DRIVER)
	UsbdMsc_Init();
	USBD_API->hw->ForceFullSpeed(UsbHandle,1);
#endif
	USB_Connect();

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	sei();
	#if defined(USB_DEVICE_ROM_DRIVER)
	NVIC_SetPriority(USB0_IRQn, 0x04);
	NVIC_SetPriorityGrouping(5);
	#endif
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= MS_Device_ConfigureEndpoints(&Disk_MS_Interface);

//	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	MS_Device_ProcessControlRequest(&Disk_MS_Interface);
}

/** Mass Storage class driver callback function the reception of SCSI commands from the host, which must be processed.
 *
 *  \param[in] MSInterfaceInfo  Pointer to the Mass Storage class interface configuration structure being referenced
 */
bool CALLBACK_MS_Device_SCSICommandReceived(USB_ClassInfo_MS_Device_t* const MSInterfaceInfo)
{
	bool CommandSuccess;

	LEDs_SetAllLEDs(LEDMASK_USB_BUSY);
	CommandSuccess = SCSI_DecodeSCSICommand(MSInterfaceInfo);
	LEDs_SetAllLEDs(LEDMASK_USB_READY);
	if (CommandSuccess) {
		/* Toggle LEDS_LED3 for every key press */
		static uint32_t blink = LEDS_LED3;

		if (ipcex_msgPush(IPCEX_ID_BLINKY, (LEDS_LED3 << 16) | blink) == QUEUE_INSERT)
			blink ^= LEDS_LED3;
	}
	return CommandSuccess;
}

#ifdef __USE_DOSFS
#include "dosfs.h"
static uint8_t tmp_buff[SECTOR_SIZE], rdbuff[SECTOR_SIZE];
static void print_vol_info(void)
{
	VOLINFO vi;
	FILEINFO fi;
	DIRINFO di;
	DIRENT  de;
	if (DFS_GetVolInfo(0, tmp_buff, 0, &vi)) {
		printf("Error getting volume information\r\n");
		return;
	}
	printf("Volume label '%-11.11s'\r\n", vi.label);
	printf("%d sector/s per cluster, %d reserved sector/s, volume total %d sectors.\r\n", vi.secperclus, vi.reservedsecs, vi.numsecs);
	printf("%d sectors per FAT, first FAT at sector #%d, root dir at #%d.\r\n",vi.secperfat,vi.fat1,vi.rootdir);
	printf("(For FAT32, the root dir is a CLUSTER number, FAT12/16 it is a SECTOR number)\r\n");
	printf("%d root dir entries, data area commences at sector #%d.\r\n",vi.rootentries,vi.dataarea);
	printf("%d clusters (%d bytes) in data area, filesystem IDd as ", vi.numclusters, vi.numclusters * vi.secperclus * SECTOR_SIZE);
	if (vi.filesystem == FAT12)
		printf("FAT12.\r\n");
	else if (vi.filesystem == FAT16)
		printf("FAT16.\r\n");
	else if (vi.filesystem == FAT32)
		printf("FAT32.\r\n");
	else
		printf("[unknown]\r\n");

	di.scratch = tmp_buff;
	if (DFS_OpenDir(&vi, "/", &di)) {
		printf("error opening subdirectory\n");
		return ;
	}
	while (!DFS_GetNext(&vi, &di, &de)) {
		if (de.name[0])
			printf("file: '%-11.11s'\n", de.name);
	}

	if (DFS_OpenFile(&vi, "README.TXT", DFS_READ, tmp_buff, &fi)) {
		printf("error opening file\n");
		return ;
	}
	do {
	uint32_t i;
	DFS_ReadFile(&fi, tmp_buff, rdbuff, &i, sizeof(rdbuff));
	printf("Read %d bytes (Buf Sz: %d Bytes)\r\n", i, sizeof(rdbuff));
	rdbuff[i] = 0;
	printf("Content:%s\r\n", rdbuff);
	}while(0);
}
#endif
