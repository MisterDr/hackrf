/*
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
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
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __SYS_CONFIG_H_
#define __SYS_CONFIG_H_

/** @ingroup BOARD_NXP_XPRESSO_11C24_OPTIONS
 * @{
 */

/* Build for 11Cxx chip family */
#define CHIP_LPC11CXX

/* Un-comment DEBUG_ENABLE for IO support via the UART */
//#define DEBUG_ENABLE

/* Enable DEBUG_SEMIHOSTING along with DEBUG to enable IO support
   via semihosting */
//#define DEBUG_SEMIHOSTING

/* Board UART used for debug output */
#define DEBUG_UART LPC_USART

/* Crystal frequency into device */
#define CRYSTAL_MAIN_FREQ_IN (12000000)

/* Frequency on external clock in pin (not applicable to LPC11Uxx) */
#define EXTCLKIN_FREQ_IN (0)

/**
 * @}
 */

#endif /* __SYS_CONFIG_H_ */
