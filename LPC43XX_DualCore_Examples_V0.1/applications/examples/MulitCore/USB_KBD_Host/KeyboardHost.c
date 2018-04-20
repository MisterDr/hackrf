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
 *  Main source file for the KeyboardHost demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "KeyboardHost.h"
#include "ipc_msg.h"
#include "ipc_example.h"

/** nxpUSBlib HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Host_t Keyboard_HID_Interface =
	{
		.Config =
			{
				.DataINPipeNumber       = 1,
				.DataINPipeDoubleBank   = false,

				.DataOUTPipeNumber      = 2,
				.DataOUTPipeDoubleBank  = false,

				.HIDInterfaceProtocol   = HID_CSCP_KeyboardBootProtocol,
			},
	};

void dbg_ch(char ch)
{
	static unsigned char *ptr = (unsigned char *)0x10080040;
	if (ptr < (unsigned char *) 0x10084000)
		*ptr++ = ch;
}

#ifdef OS_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static xSemaphoreHandle usb_event;
void USB_KBD_Task(void *arg)
{
	vSemaphoreCreateBinary(usb_event);
	if (usb_event == NULL)
		while(1);

	while(1)
	{
		if (USB_HostState == HOST_STATE_Unattached || 
			  USB_HostState == HOST_STATE_Configured)
				xSemaphoreTake(usb_event, 100);

		if (USB_HostState == HOST_STATE_Unattached)
			continue;
		if (USB_HostState == HOST_STATE_Configured) {
			KeyboardHost_Task();
			continue;
		}
		/* This is a dummy function */
		HID_Host_USBTask(&Keyboard_HID_Interface);
		
		/* USB State machine processing */
		USB_USBTask();
	}
}

void usb_host_tasks(void)
{
	/* Start Blinky event Task */
	xTaskCreate( USB_KBD_Task, ( signed char * ) "USB Task",
		configMINIMAL_STACK_SIZE + 128, NULL, (tskIDLE_PRIORITY + 1UL),
		( xTaskHandle * ) NULL );
}

void usb_release_sem(void)
{
	portBASE_TYPE wake = pdFALSE;
	if (usb_event)
		xSemaphoreGiveFromISR(usb_event, &wake);
	portEND_SWITCHING_ISR(wake);
}
#define EventAttach       usb_release_sem
#define EventDetach       usb_release_sem
#define EventKeyPress     usb_release_sem
#elif defined(OS_UCOS_III)
#else
/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
void usb_host_tasks(void)
{
		if (USB_HostState == HOST_STATE_Configured) {
			KeyboardHost_Task();
		}
		else
		{
			HID_Host_USBTask(&Keyboard_HID_Interface);
			USB_USBTask();
		}
}
void EventAttach(void){};
void EventDetach(void){};
void EventKeyPress(void){};
#endif

/** Task to manage an enumerated USB keyboard once connected, to display key state
 *  data as it is received.
 */
void KeyboardHost_Task(void)
{
	if (USB_HostState != HOST_STATE_Configured)
	  return;

	if (HID_Host_IsReportReceived(&Keyboard_HID_Interface))
	{
		USB_KeyboardReport_Data_t KeyboardReport;
		HID_Host_ReceiveReport(&Keyboard_HID_Interface, &KeyboardReport);

		LEDs_ChangeLEDs(LEDS_LED1, (KeyboardReport.Modifier) ? LEDS_LED1 : 0);

		uint8_t KeyCode = KeyboardReport.KeyCode[0];
		dbg_ch('K');

		if (KeyCode)
		{
			char PressedKey = 0;

			LEDs_ToggleLEDs(LEDS_LED2);

			/* Retrieve pressed key character if alphanumeric */
			if ((KeyCode >= HID_KEYBOARD_SC_A) && (KeyCode <= HID_KEYBOARD_SC_Z))
			{
				PressedKey = (KeyCode - HID_KEYBOARD_SC_A) + 'A';
			}
			else if ((KeyCode >= HID_KEYBOARD_SC_1_AND_EXCLAMATION) &
			         (KeyCode  < HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS))
			{
				PressedKey = (KeyCode - HID_KEYBOARD_SC_1_AND_EXCLAMATION) + '1';
			}
			else if (KeyCode == HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS)
			{
				PressedKey = '0';
			}
			else if (KeyCode == HID_KEYBOARD_SC_SPACE)
			{
				PressedKey = ' ';
			}
			else if (KeyCode == HID_KEYBOARD_SC_ENTER)
			{
				PressedKey = '\n';
			}

			if (PressedKey)
			  ipcex_msgPush(IPCEX_ID_USBHOST, PressedKey);
		}
	}
}

/** Event handler for the USB_DeviceAttached event. This indicates that a device has been attached to the host, and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Host_DeviceAttached(void)
{
	puts_P(PSTR("Device Attached.\r\n"));
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
	EventAttach();
}

/** Event handler for the USB_DeviceUnattached event. This indicates that a device has been removed from the host, and
 *  stops the library USB task management process.
 */
void EVENT_USB_Host_DeviceUnattached(void)
{
	puts_P(PSTR("\r\nDevice Unattached.\r\n"));
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	EventDetach();
}

/** Event handler for the USB_DeviceEnumerationComplete event. This indicates that a device has been successfully
 *  enumerated by the host and is now ready to be used by the application.
 */
void EVENT_USB_Host_DeviceEnumerationComplete(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);

	uint16_t ConfigDescriptorSize;
	uint8_t  ConfigDescriptorData[512];

	if (USB_Host_GetDeviceConfigDescriptor(1, &ConfigDescriptorSize, ConfigDescriptorData,
	                                       sizeof(ConfigDescriptorData)) != HOST_GETCONFIG_Successful)
	{
		puts_P(PSTR("Error Retrieving Configuration Descriptor.\r\n"));
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	if (HID_Host_ConfigurePipes(&Keyboard_HID_Interface,
	                            ConfigDescriptorSize, ConfigDescriptorData) != HID_ENUMERROR_NoError)
	{
		puts_P(PSTR("Attached Device Not a Valid Keyboard.\r\n"));
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	if (USB_Host_SetDeviceConfiguration(1) != HOST_SENDCONTROL_Successful)
	{
		puts_P(PSTR("Error Setting Device Configuration.\r\n"));
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	if (HID_Host_SetBootProtocol(&Keyboard_HID_Interface) != 0)
	{
		puts_P(PSTR("Could not Set Boot Protocol Mode.\r\n"));
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		USB_Host_SetDeviceConfiguration(0);
		return;
	}

	puts_P(PSTR("Keyboard Enumerated.\r\n"));
	LEDs_SetAllLEDs(LEDMASK_USB_READY);
}

/** Event handler for the USB_HostError event. This indicates that a hardware error occurred while in host mode. */
void EVENT_USB_Host_HostError(const uint8_t ErrorCode)
{
	USB_Disable();

	printf_P(PSTR(ESC_FG_RED "Host Mode Error\r\n"
	                         " -- Error Code %d\r\n" ESC_FG_WHITE), ErrorCode);

	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	for(;;);
}

/** Event handler for the USB_DeviceEnumerationFailed event. This indicates that a problem occurred while
 *  enumerating an attached USB device.
 */
void EVENT_USB_Host_DeviceEnumerationFailed(const uint8_t ErrorCode,
                                            const uint8_t SubErrorCode)
{
	printf_P(PSTR(ESC_FG_RED "Dev Enum Error\r\n"
	                         " -- Error Code %d\r\n"
	                         " -- Sub Error Code %d\r\n"
	                         " -- In State %d\r\n" ESC_FG_WHITE), ErrorCode, SubErrorCode, USB_HostState);

	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
}

/** Dummy callback function for HID Parser
 */
bool CALLBACK_HIDParser_FilterHIDReportItem(HID_ReportItem_t* const CurrentItem){
	return false;
}

void host_xfer_event(void)
{
	if (USB_HostState != HOST_STATE_Configured)
	  return;
	EventKeyPress();
}

void USBHOST_Init(void)
{
	USB_Init();
}

