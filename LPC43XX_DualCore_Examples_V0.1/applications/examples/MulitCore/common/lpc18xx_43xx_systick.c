/**********************************************************************
* $Id$		lpc18xx_43xx_systick_arch.c			2011-11-20
*//**
* @file		lpc18xx_43xx_systick_arch.c
* @brief	Setups up the system tick to generate a reference timebase
* @version	1.0
* @date		20. Nov. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

#if NO_SYS == 1

#ifdef LPC43XX
#include "lpc43xx_cgu.h"
#include "lpc43xx_rit.h"
#else
#ifdef LPC18XX
#include "lpc18xx_cgu.h"
#else
#error LPC18XX or LPC43XX for target system not defined!
#endif
#endif

#include "bsp.h"

#ifdef EXAMPLE_EMWIN
#include "GUI.h"
#endif

/** @defgroup LPC18xx_43xx_systick	LPC18xx_43xx LWIP (standalone) timer base
 * @ingroup LPC18xx_43xx
 * @{
 */

#ifdef EXAMPLE_EMWIN
static int16_t old_tmp_x = -1, old_tmp_y = -1;
extern volatile int tsc_init_done;
#define TSC_CHECK_DELAY   (20)   /* Check every 20ms for TSC events */
#endif

#ifdef CORE_M0
/* RI Timer interrupt priority */
#define RITIMER_OR_WWDT_IRQn_PRI  (255)
#define RITENCLR               (1 << 1)
#define RITINT                 (1 << 0)
#endif

/* Saved reference period */
static uint32_t saved_period;

/* Saved total time in mS since timer was enabled */
volatile uint32_t systick_timems;

#ifdef CORE_M0
static uint32_t reload_val;
#endif

/* Enable systick rate and interrupt */
void SysTick_Enable(uint32_t period)
{
#ifdef CORE_M0
	LPC_RITIMER->CTRL |= RITINT;
	saved_period = period;
	reload_val = (CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / (1000 / period));
	NVIC_SetPriority(M0_RITIMER_OR_WWDT_IRQn, RITIMER_OR_WWDT_IRQn_PRI);
	NVIC_EnableIRQ(M0_RITIMER_OR_WWDT_IRQn);
	LPC_RITIMER->COMPVAL = LPC_RITIMER->COUNTER + reload_val; /* Reload value */
#else
	saved_period = period;
	SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / (1000 / period));
#endif
}

/* Disable systick */
void SysTick_Disable(void)
{
#ifdef CORE_M0
	LPC_RITIMER->CTRL &= ~RITENCLR;
#else
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
#endif
}

/** \brief  SysTick IRQ handler and timebase management
 *
 *  This function keeps a timebase for the sysTick that can be
 *  used for other functions.
 */
#ifdef CORE_M0
void M0_RIT_OR_WWDT_IRQHandler(void)
{
#ifdef EXAMPLE_EMWIN
	int16_t tmp_x = -1, tmp_y = -1;
	int16_t tmp_x1 = -1, tmp_y1 = -1;
	static uint8_t tsc_tick = 0;
	bool touched;
#endif	
	
	/* Clear Interrupt */
	LPC_RITIMER->CTRL |= RITINT;
	LPC_RITIMER->COMPVAL = LPC_RITIMER->COUNTER + reload_val; /* Reload value */

	/* Increment tick count */
	systick_timems += saved_period;
	
#ifdef EXAMPLE_EMWIN
	/* If TSC enabled, store Touch event */
	if (tsc_init_done)
	{
		tsc_tick += saved_period;
		if(tsc_tick == TSC_CHECK_DELAY)
		{
			tsc_tick = 0;
			touched = TSC_Controller_GetTouchPos((int16_t*)&tmp_x, (int16_t*)&tmp_y);
			if(touched == true)
			{
				if((tmp_x >= 0) && (tmp_y >0) && (tmp_x != old_tmp_x || tmp_y!= old_tmp_y))
				{
					tmp_x1 = tmp_y;
					tmp_y1 = tmp_x;
					GUI_TOUCH_StoreState(320 - tmp_x1, tmp_y1);
					old_tmp_x = tmp_x;
					old_tmp_y = tmp_y;
				}
				else
				{
					GUI_TOUCH_StoreState(-1, -1);
				}
			}
		}
	}
#endif	
}
#else
void SysTick_Handler(void)
{
#ifdef EXAMPLE_EMWIN
	int16_t tmp_x = -1, tmp_y = -1;
	int16_t tmp_x1 = -1, tmp_y1 = -1;
	static uint8_t tsc_tick = 0;
	bool touched;
#endif	
	
	/* Increment tick count */
	systick_timems += saved_period;
	
#ifdef EXAMPLE_EMWIN
	/* If TSC enabled, store Touch event */
	if (tsc_init_done)
	{
		tsc_tick += saved_period;
		if(tsc_tick == TSC_CHECK_DELAY)
		{
			tsc_tick = 0;
			touched = TSC_Controller_GetTouchPos((int16_t*)&tmp_x, (int16_t*)&tmp_y);
			if(touched == true)
			{
				if((tmp_x >= 0) && (tmp_y >0) && (tmp_x != old_tmp_x || tmp_y!= old_tmp_y))
				{
					tmp_x1 = tmp_y;
					tmp_y1 = tmp_x;
					GUI_TOUCH_StoreState(320 - tmp_x1, tmp_y1);
					old_tmp_x = tmp_x;
					old_tmp_y = tmp_y;
				}
				else
				{
					GUI_TOUCH_StoreState(-1, -1);
				}
			}
		}
	}
#endif	
}
#endif
#endif

/**
 * @}
 */

 /* --------------------------------- End Of File ------------------------------ */
