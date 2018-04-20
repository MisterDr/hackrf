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



#ifndef __BSP_MCB1800_4300_H__
#define __BSP_MCB1800_4300_H__

#if defined(__LPC43XX__)
#include "lpc43xx_uart.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_timer.h"
#include "lpc43xx_i2c.h"
#include "lpc43xx_gpdma.h"
#include "lpc43xx_i2s.h"
#include "lpc43xx_emc.h"
#include "lpc43xx_ssp.h"
#include "lpc_emac_config.h"
#include "debug_frmwrk.h"
#include "GUI.h"
#include "GUIDRV_Lin.h"
#elif defined(__LPC18XX__)
#include "lpc18xx_uart.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_cgu.h"
#include "lpc18xx_gpio.h"
#include "lpc18xx_timer.h"
#include "lpc18xx_i2c.h"
#include "lpc18xx_gpdma.h"
#include "lpc18xx_i2s.h"
#include "lpc18xx_emc.h"
#endif

#define BUTTONS_BUTTON1_GPIO_PORT_NUM	2
#define BUTTONS_BUTTON1_GPIO_BIT_NUM	0

/* LEDs on MCB4357 board */
#define LED1_GPIO_PORT_NUM			6
#define LED1_GPIO_BIT_NUM				(1 << 24)
#define LED2_GPIO_PORT_NUM			6
#define LED2_GPIO_BIT_NUM				(1 << 25)
#define LED3_GPIO_PORT_NUM			6
#define LED3_GPIO_BIT_NUM				(1 << 26)
#define LED4_GPIO_PORT_NUM			6
#define LED4_GPIO_BIT_NUM				(1 << 27)
#define LED5_GPIO_PORT_NUM			6
#define LED5_GPIO_BIT_NUM				(1 << 28)
#define LED6_GPIO_PORT_NUM			4
#define LED6_GPIO_BIT_NUM				(1 << 12)
#define LED7_GPIO_PORT_NUM			4
#define LED7_GPIO_BIT_NUM				(1 << 13)
#define LED8_GPIO_PORT_NUM			4
#define LED8_GPIO_BIT_NUM				(1 << 14)

#define JOYSTICK_UP_GPIO_PORT_NUM				6
#define JOYSTICK_UP_GPIO_BIT_NUM				10
#define JOYSTICK_DOWN_GPIO_PORT_NUM				6
#define JOYSTICK_DOWN_GPIO_BIT_NUM				11
#define JOYSTICK_LEFT_GPIO_PORT_NUM				6
#define JOYSTICK_LEFT_GPIO_BIT_NUM				12
#define JOYSTICK_RIGHT_GPIO_PORT_NUM			6
#define JOYSTICK_RIGHT_GPIO_BIT_NUM				13
#define JOYSTICK_PRESS_GPIO_PORT_NUM			6
#define JOYSTICK_PRESS_GPIO_BIT_NUM				8

/* Ethernet 
 */
#define LPC_EMAC_RMII 1     /**< Use RMII interface */

#define LPC_EMAC_ADDR0 0x00 /**< Hardware MAC address field 0 */
#define LPC_EMAC_ADDR1 0x60 /**< Hardware MAC address field 1 */
#define LPC_EMAC_ADDR2 0x37 /**< Hardware MAC address field 2 */
#define LPC_EMAC_ADDR3 0x12 /**< Hardware MAC address field 3 */
#define LPC_EMAC_ADDR4 0x34 /**< Hardware MAC address field 4 */
#define LPC_EMAC_ADDR5 0x56 /**< Hardware MAC address field 5 */

/* 
 * LCD data for MCB4300 board
 */
/* LCD Screen Physical Size */ 
#define XSIZE_PHYS 240
#define YSIZE_PHYS 320

/* Color Conversion */
#define COLOR_CONVERSION GUICC_M565

/* Buffers / VScreens */
#define NUM_BUFFERS  1 // Number of multiple buffers to be used
#define NUM_VSCREENS 1 // Number of virtual screens to be used

/* Video RAM Address */
#define VRAM_ADDR  0x28060000

/* Display orientation */
#define DISPLAY_ORIENTATION (GUI_SWAP_XY | GUI_MIRROR_Y)

/* Display driver */
#if   (DISPLAY_ORIENTATION == (GUI_MIRROR_X))
  #define DISPLAY_DRIVER GUIDRV_LIN_OX_16
#elif (DISPLAY_ORIENTATION == (GUI_MIRROR_Y))
  #define DISPLAY_DRIVER GUIDRV_LIN_OY_16
#elif (DISPLAY_ORIENTATION == (GUI_MIRROR_X | GUI_MIRROR_Y))
  #define DISPLAY_DRIVER GUIDRV_LIN_OXY_16
#elif (DISPLAY_ORIENTATION == (GUI_SWAP_XY))
  #define DISPLAY_DRIVER GUIDRV_LIN_OS_16
#elif (DISPLAY_ORIENTATION == (GUI_SWAP_XY | GUI_MIRROR_X))
  #define DISPLAY_DRIVER GUIDRV_LIN_OSX_16
#elif (DISPLAY_ORIENTATION == (GUI_SWAP_XY | GUI_MIRROR_Y))
  #define DISPLAY_DRIVER GUIDRV_LIN_OSY_16
#else
  #define DISPLAY_DRIVER GUIDRV_LIN_16
#endif

/* Touch screen calibration */
#define USE_TOUCH   1

#define TOUCH_X_MIN 0x00E0
#define TOUCH_X_MAX 0x0F40
#define TOUCH_Y_MIN 0x00C0
#define TOUCH_Y_MAX 0x0F60

#endif

