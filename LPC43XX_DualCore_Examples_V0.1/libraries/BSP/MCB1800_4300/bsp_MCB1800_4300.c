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


#include "../bsp_internal.h"
#include <string.h>

#if (BOARD == BOARD_MCB1800_4300)

#define LPC_I2C LPC_I2C0

#if !defined(__LPC18XX__)
#define CORE_CLOCK_ID	CGU_PERIPHERAL_M4CORE
#else
#define CORE_CLOCK_ID	CGU_PERIPHERAL_M3CORE
#endif

#define CPU_FREQ         96000000UL
#define WR_MODE(x) (*((volatile uint32_t *)(SDRAM_BASE_ADDR | (x))))

#define EMC_SDRAM_REFRESH(freq,time)  \
  (((uint64_t)((uint64_t)time * freq)/16000000000ull)+1)

static void EMCMemoryPinInit(void)
{
	/* DATA LINES 0..31 > D0..D31 */
  scu_pinmux(0x1,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_7: D0 (function 0) errata */
  scu_pinmux(0x1,  8,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_8: D1 (function 0) errata */
  scu_pinmux(0x1,  9,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_9: D2 (function 0) errata */
  scu_pinmux(0x1,  10, (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_10: D3 (function 0) errata */
  scu_pinmux(0x1,  11, (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_11: D4 (function 0) errata */
  scu_pinmux(0x1,  12, (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_12: D5 (function 0) errata */
  scu_pinmux(0x1,  13, (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_13: D6 (function 0) errata */
  scu_pinmux(0x1,  14, (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_14: D7 (function 0) errata */
  scu_pinmux(0x5,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_4: D8 (function 0) errata */
  scu_pinmux(0x5,  5,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_5: D9 (function 0) errata */
  scu_pinmux(0x5,  6,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_6: D10 (function 0) errata */
  scu_pinmux(0x5,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_7: D11 (function 0) errata */
  scu_pinmux(0x5,  0,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_0: D12 (function 0) errata */
  scu_pinmux(0x5,  1,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_1: D13 (function 0) errata */
  scu_pinmux(0x5,  2,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_2: D14 (function 0) errata */
  scu_pinmux(0x5,  3,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_3: D15 (function 0) errata */

  scu_pinmux(0xD,  2,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* PD_2: D16 (function 0) errata */
  scu_pinmux(0xD,  3,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* PD_3: D17 (function 0) errata */
  scu_pinmux(0xD,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* PD_4: D18 (function 0) errata */
  scu_pinmux(0xD,  5,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* PD_5: D19 (function 0) errata */
  scu_pinmux(0xD,  6,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* PD_6: D20 (function 0) errata */
  scu_pinmux(0xD,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* PD_7: D21 (function 0) errata */
  scu_pinmux(0xD,  8,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* PD_8: D22 (function 0) errata */
  scu_pinmux(0xD,  9,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* PD_9: D23 (function 0) errata */
  scu_pinmux(0xE,  5,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* PE_5: D24 (function 0) errata */
  scu_pinmux(0xE,  6,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* PE_6: D25 (function 0) errata */
  scu_pinmux(0xE,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* PE_7: D26 (function 0) errata */
  scu_pinmux(0xE,  8,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* PE_8: D27 (function 0) errata */
  scu_pinmux(0xE,  9,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* PE_9: D28 (function 0) errata */
  scu_pinmux(0xE, 10,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* PE_10: D29 (function 0) errata */
  scu_pinmux(0xE, 11,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* PE_11: D30 (function 0) errata */
  scu_pinmux(0xE, 12,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* PE_12: D31 (function 0) errata */

  /* ADDRESS LINES A0..A23 */
	scu_pinmux(0x2,  9,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_9 - EXTBUS_A0 — External memory address line 0 */
	scu_pinmux(0x2, 10,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_10 - EXTBUS_A1 — External memory address line 1 */
	scu_pinmux(0x2, 11,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_11 - EXTBUS_A2 — External memory address line 2 */
	scu_pinmux(0x2, 12,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_12 - EXTBUS_A3 — External memory address line 3 */
	scu_pinmux(0x2, 13,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_13 - EXTBUS_A4 — External memory address line 4 */
	scu_pinmux(0x1,  0,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P1_0 - EXTBUS_A5 — External memory address line 5 */
	scu_pinmux(0x1,  1,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P1_1 - EXTBUS_A6 — External memory address line 6 */
	scu_pinmux(0x1,  2,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P1_2 - EXTBUS_A7 — External memory address line 7 */
	scu_pinmux(0x2,  8,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_8 - EXTBUS_A8 — External memory address line 8 */
	scu_pinmux(0x2,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_7 - EXTBUS_A9 — External memory address line 9 */
	scu_pinmux(0x2,  6,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P2_6 - EXTBUS_A10 — External memory address line 10 */
	scu_pinmux(0x2,  2,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P2_2 - EXTBUS_A11 — External memory address line 11 */
	scu_pinmux(0x2,  1,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P2_1 - EXTBUS_A12 — External memory address line 12 */
	scu_pinmux(0x2,  0,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P2_0 - EXTBUS_A13 — External memory address line 13 */
	scu_pinmux(0x6,  8,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC1);	/* P6_8 - EXTBUS_A14 — External memory address line 14 */
	scu_pinmux(0x6,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC1);	/* P6_7 - EXTBUS_A15 — External memory address line 15 */
	scu_pinmux(0xD, 16,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* PD_16 - EXTBUS_A16 — External memory address line 16 */
	scu_pinmux(0xD, 15,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* PD_15 - EXTBUS_A17 — External memory address line 17 */
	scu_pinmux(0xE,  0,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_0 - EXTBUS_A18 — External memory address line 18 */
	scu_pinmux(0xE,  1,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_1 - EXTBUS_A19 — External memory address line 19 */
	scu_pinmux(0xE,  2,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_2 - EXTBUS_A20 — External memory address line 20 */
	scu_pinmux(0xE,  3,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_3 - EXTBUS_A21 — External memory address line 21 */
	scu_pinmux(0xE,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_4 - EXTBUS_A22 — External memory address line 22 */
	scu_pinmux(0xA,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PA_4 - EXTBUS_A23 — External memory address line 23 */

  scu_pinmux(0x1,	 3,	MD_PLN_FAST	,	FUNC3	);	//OE
	scu_pinmux(0x1,  4,	MD_PLN_FAST	,	FUNC3	);	//BLS0
	scu_pinmux(0x1,	 5,	MD_PLN_FAST	,	FUNC3	);	//CS0
	scu_pinmux(0x1,	 6,	MD_PLN_FAST	,	FUNC3	);	//WE

	//scu_pinmux(0x6,  1,	MD_PLN_FAST	,	FUNC1	);	//DYCS1
	//scu_pinmux(0x6,  2,	MD_PLN_FAST	,	FUNC1	);	//CKEOUT1
	//scu_pinmux(0x6,  3,	MD_PLN_FAST	,	FUNC3	);	//CS1
	scu_pinmux(0x6,  4,	MD_PLN_FAST	,	FUNC3	);	//CAS
	scu_pinmux(0x6,  5,	MD_PLN_FAST	,	FUNC3	);	//RAS
	scu_pinmux(0x6,  6,	MD_PLN_FAST	,	FUNC1	);	//BLS1
	scu_pinmux(0x6,  9,	MD_PLN_FAST	,	FUNC3	);	//DYCS0
	scu_pinmux(0x6,  10,MD_PLN_FAST	,	FUNC3	);	//DQMOUT1
	scu_pinmux(0x6,  11,MD_PLN_FAST	,	FUNC3	);	//CKEOUT0
	scu_pinmux(0x6,  12,MD_PLN_FAST	,	FUNC3	);	//DQMOUT0

	scu_pinmux(0xD,  0,MD_PLN_FAST	,	FUNC2	);	//DQMOUT2
	//scu_pinmux(0xD,  1,MD_PLN_FAST	,	FUNC2	);	//CKEOUT2
	scu_pinmux(0xD,  10,MD_PLN_FAST	,	FUNC2	);	//BLS3
	//scu_pinmux(0xD,  11,MD_PLN_FAST	,	FUNC2	);	//CS3
	//scu_pinmux(0xD,  12,MD_PLN_FAST	,	FUNC2	);	//CS2
	scu_pinmux(0xD,  13,MD_PLN_FAST	,	FUNC2	);	//BLS2
	//scu_pinmux(0xD,  14,MD_PLN_FAST	,	FUNC2	);	//DYCS2

	scu_pinmux(0xE,  13,MD_PLN_FAST	,	FUNC3	);	//DQMOUT3
	//scu_pinmux(0xE,  14,MD_PLN_FAST	,	FUNC3	);	//DYCS3
	//scu_pinmux(0xE,  15,MD_PLN_FAST	,	FUNC3	);	//CKEOUT3

	/* Select EMC clock-out */
  LPC_SCU->SFSCLK_0 = MD_PLN_FAST;
  LPC_SCU->SFSCLK_1 = MD_PLN_FAST;
  LPC_SCU->SFSCLK_2 = MD_PLN_FAST;
  LPC_SCU->SFSCLK_3 = MD_PLN_FAST;
}

static uint32_t NS2CLK(uint32_t freq,uint32_t time)
{
 return (((uint64_t)time*freq/1000000000));
}

static void sdram_WaitUS(volatile uint32_t us, uint32_t CoreClk)
{
	us *= (CoreClk / 1000000) / 3;
	while(us--);
}

static void EMC_InitSDRAM(void)
{
	uint32_t Core_Clock;
	uint64_t Temp;

	EMCMemoryPinInit();

	/* Get Core clock */
	Core_Clock = CGU_GetPCLKFrequency(CORE_CLOCK_ID);

	/* adjust the CCU delaye for EMI (default to zero) */
	if(Core_Clock <  80000000UL)
	{
			LPC_SCU->EMCDELAYCLK = 0;
	}
	else
	{
		LPC_SCU->EMCDELAYCLK = ((CLK0_DELAY)
							|  (CLK0_DELAY << 4)
							|  (CLK0_DELAY << 8)
							|  (CLK0_DELAY << 12));
	}

	/* Initialize EMC to interface with SDRAM */
	LPC_EMC->CONTROL 			= 0x00000001;   /* Enable the external memory controller */
	LPC_EMC->CONFIG 			= 0;

	/* Initialise the Dynamic memory registers */
	LPC_EMC->DYNAMICCONFIG0 	= ((2 << 7) | (2 << 9) | (0 << 12) | (1 << 14));
  LPC_EMC->DYNAMICRASCAS0 	= (3 << 0) | (3 << 8);  // aem
  LPC_EMC->DYNAMICREADCONFIG	= EMC_COMMAND_DELAYED_STRATEGY;

	LPC_EMC->DYNAMICRP 			= NS2CLK(Core_Clock, 20);
	LPC_EMC->DYNAMICRAS 		= NS2CLK(Core_Clock, 42);
	LPC_EMC->DYNAMICSREX 		= NS2CLK(Core_Clock, 63);
	LPC_EMC->DYNAMICAPR 		= NS2CLK(Core_Clock, 70);
	LPC_EMC->DYNAMICDAL 		= NS2CLK(Core_Clock, 70);
	LPC_EMC->DYNAMICWR 			= NS2CLK(Core_Clock, 30);
	LPC_EMC->DYNAMICRC 			= NS2CLK(Core_Clock, 63);
	LPC_EMC->DYNAMICRFC 		= NS2CLK(Core_Clock, 63);
	LPC_EMC->DYNAMICXSR 		= NS2CLK(Core_Clock, 63);
	LPC_EMC->DYNAMICRRD 		= NS2CLK(Core_Clock, 14);
	LPC_EMC->DYNAMICMRD 		= NS2CLK(Core_Clock, 30);

	sdram_WaitUS(100, Core_Clock);
	LPC_EMC->DYNAMICCONTROL 	= 0x00000183;
	sdram_WaitUS(200, Core_Clock);
	LPC_EMC->DYNAMICCONTROL 	= 0x00000103;

	LPC_EMC->DYNAMICREFRESH 	= EMC_SDRAM_REFRESH(Core_Clock,70);
	sdram_WaitUS(200, Core_Clock);

	Temp = (uint64_t)15625*(uint64_t)Core_Clock/1000000000/16;
  LPC_EMC->DYNAMICREFRESH 	= Temp;

	LPC_EMC->DYNAMICCONTROL 	= 0x00000083;

  /* Mode register: Burst Length: 4, Burst Type: Sequential, CAS Latency: 3   */
  WR_MODE(((3 << 4) | 2) << 12);
	sdram_WaitUS(10, Core_Clock);

	LPC_EMC->DYNAMICCONTROL 	= 0x00000002; // EMC_CE_ENABLE | EMC_CS_ENABLE;
	LPC_EMC->DYNAMICCONFIG0 	|=  (1 << 19);
}

void bsp_init(void)
{
  /* Set up core clock */
	CGU_Init();

	/* Initialize the external SDRAM */
	EMC_InitSDRAM();

	scu_pinmux(0x9, 5, MD_PUP | MD_EZI, FUNC2);				// P9_5 USB1_PWR_EN, USB1 VBus function
	scu_pinmux(0x2, 5, MD_PLN | MD_EZI | MD_ZI, FUNC2);		// P2_5 USB1_VBUS, MUST CONFIGURE THIS SIGNAL FOR USB1 NORMAL OPERATION
	scu_pinmux(0x6, 3, MD_PUP | MD_EZI, FUNC1);     		// P6_3 USB0_PWR_EN, USB0 VBus function
}

volatile uint32_t lpc43xx_iflash;
void bsp_misc_init(void)
{
	lpc43xx_iflash = 0x80;
}

void Serial_Init(const uint32_t BaudRate, const bool DoubleSpeed)
{
	UART_CFG_Type UARTConfigStruct;

	/*
	 * Initialize UART pin connect
	 */
//	scu_pinmux(0x2 ,0 , MD_PDN, FUNC1); 				// UART0_TXD
//	scu_pinmux(0x2 ,1 , MD_PLN|MD_EZI|MD_ZI, FUNC1); 	// UART0_RXD
	scu_pinmux(0x2 ,3 , MD_PDN, FUNC2); 				// UART3_TXD
	scu_pinmux(0x2 ,4 , MD_PLN|MD_EZI|MD_ZI, FUNC2);	// UART3_RXD

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);
	UARTConfigStruct.Baud_rate = BaudRate;	// Re-configure baudrate

	// Initialize uart peripheral with given to corresponding parameter
	UART_Init(DEBUG_UART_PORT, &UARTConfigStruct);

	// Enable UART Transmit
	UART_TxCmd(DEBUG_UART_PORT, ENABLE);
}

void Buttons_Init(void)	// FIXME not functional ATM
{
	scu_pinmux(0x4 ,0 , MD_PUP|MD_EZI|MD_ZI, FUNC0);		// P4_0 as GPIO2[0]
	GPIO_SetDir(BUTTONS_BUTTON1_GPIO_PORT_NUM, (1<<BUTTONS_BUTTON1_GPIO_BIT_NUM), 0); // input
}

void Joystick_Init(void)
{
	scu_pinmux(0xC ,9 , MD_PUP|MD_EZI|MD_ZI, FUNC4);		// PC_9  as GPIO6[8]
	scu_pinmux(0xC ,11, MD_PUP|MD_EZI|MD_ZI, FUNC4);		// PC_11 as GPIO6[10]
	scu_pinmux(0xC ,12, MD_PUP|MD_EZI|MD_ZI, FUNC4);		// PC_12 as GPIO6[11]
	scu_pinmux(0xC ,13, MD_PUP|MD_EZI|MD_ZI, FUNC4);		// PC_13 as GPIO6[12]
	scu_pinmux(0xC ,14, MD_PUP|MD_EZI|MD_ZI, FUNC4);		// PC_14 as GPIO6[13]

	GPIO_SetDir(JOYSTICK_UP_GPIO_PORT_NUM,(1<<JOYSTICK_UP_GPIO_BIT_NUM),0); 		// input
	GPIO_SetDir(JOYSTICK_DOWN_GPIO_PORT_NUM,(1<<JOYSTICK_DOWN_GPIO_BIT_NUM),0); 	// input
	GPIO_SetDir(JOYSTICK_LEFT_GPIO_PORT_NUM,(1<<JOYSTICK_LEFT_GPIO_BIT_NUM),0); 	// input
	GPIO_SetDir(JOYSTICK_RIGHT_GPIO_PORT_NUM,(1<<JOYSTICK_RIGHT_GPIO_BIT_NUM),0); 	// input
	GPIO_SetDir(JOYSTICK_PRESS_GPIO_PORT_NUM,(1<<JOYSTICK_PRESS_GPIO_BIT_NUM),0); 	// input
}
uint8_t Joystick_GetStatus(void)
{
	uint8_t ret = NO_BUTTON_PRESSED;

	if((GPIO_ReadValue(JOYSTICK_UP_GPIO_PORT_NUM) & (1<<JOYSTICK_UP_GPIO_BIT_NUM)) == 0x00)
	{
		ret |= JOY_UP;
	}
	else if((GPIO_ReadValue(JOYSTICK_DOWN_GPIO_PORT_NUM) & (1<<JOYSTICK_DOWN_GPIO_BIT_NUM)) == 0x00)
	{
		ret |= JOY_DOWN;
	}
	else if((GPIO_ReadValue(JOYSTICK_LEFT_GPIO_PORT_NUM) & (1<<JOYSTICK_LEFT_GPIO_BIT_NUM)) == 0x00)
	{
		ret |= JOY_LEFT;
	}
	else if((GPIO_ReadValue(JOYSTICK_RIGHT_GPIO_PORT_NUM) & (1<<JOYSTICK_RIGHT_GPIO_BIT_NUM)) == 0x00)
	{
		ret |= JOY_RIGHT;
	}
	else if((GPIO_ReadValue(JOYSTICK_PRESS_GPIO_PORT_NUM) & (1<<JOYSTICK_PRESS_GPIO_BIT_NUM)) == 0x00)
	{
		ret |= JOY_PRESS;
	}

	return ret;
}

void LEDs_Init(void)
{
	scu_pinmux(0xD, 10, MD_PLN, FUNC4);	// PD.10 : LED 0 (leftmost)
	scu_pinmux(0xD, 11, MD_PLN, FUNC4); // PD.11 : LED 1
	scu_pinmux(0xD, 12, MD_PLN, FUNC4);	// PD.12 : LED 2
	scu_pinmux(0xD, 13, MD_PLN, FUNC4);	// PD.13 : LED 3
	scu_pinmux(0xD, 14, MD_PLN, FUNC4);	// PD.14 : LED 4
	scu_pinmux(0x9,  0, MD_PLN, FUNC0);	// P9.0  : LED 5
	scu_pinmux(0x9,  1, MD_PLN, FUNC0);	// P9.1  : LED 6
	scu_pinmux(0x9,  2, MD_PLN, FUNC0);	// P9.2  : LED 7 (rightmost)

	GPIO_SetDir(LED1_GPIO_PORT_NUM, LED1_GPIO_BIT_NUM, 1); // output
	GPIO_SetDir(LED2_GPIO_PORT_NUM, LED2_GPIO_BIT_NUM, 1); // output
	GPIO_SetDir(LED3_GPIO_PORT_NUM, LED3_GPIO_BIT_NUM, 1); // output
	GPIO_SetDir(LED4_GPIO_PORT_NUM, LED4_GPIO_BIT_NUM, 1); // output
	GPIO_SetDir(LED5_GPIO_PORT_NUM, LED5_GPIO_BIT_NUM, 1); // output
	GPIO_SetDir(LED6_GPIO_PORT_NUM, LED6_GPIO_BIT_NUM, 1); // output
	GPIO_SetDir(LED7_GPIO_PORT_NUM, LED7_GPIO_BIT_NUM, 1); // output
	GPIO_SetDir(LED8_GPIO_PORT_NUM, LED8_GPIO_BIT_NUM, 1); // output
}

void LEDs_SetAllLEDs(uint32_t LEDMask)
{
	if(LEDMask & LEDS_LED1)
		GPIO_SetValue(LED1_GPIO_PORT_NUM, LED1_GPIO_BIT_NUM);
	else
		GPIO_ClearValue(LED1_GPIO_PORT_NUM, LED1_GPIO_BIT_NUM);

	if(LEDMask & LEDS_LED2)
		GPIO_SetValue(LED2_GPIO_PORT_NUM, LED2_GPIO_BIT_NUM);
	else
		GPIO_ClearValue(LED2_GPIO_PORT_NUM, LED2_GPIO_BIT_NUM);

	if(LEDMask & LEDS_LED3)
		GPIO_SetValue(LED3_GPIO_PORT_NUM, LED3_GPIO_BIT_NUM);
	else
		GPIO_ClearValue(LED3_GPIO_PORT_NUM, LED3_GPIO_BIT_NUM);

	if(LEDMask & LEDS_LED4)
		GPIO_SetValue(LED4_GPIO_PORT_NUM, LED4_GPIO_BIT_NUM);
	else
		GPIO_ClearValue(LED4_GPIO_PORT_NUM, LED4_GPIO_BIT_NUM);

	if(LEDMask & LEDS_LED5)
		GPIO_SetValue(LED5_GPIO_PORT_NUM, LED5_GPIO_BIT_NUM);
	else
		GPIO_ClearValue(LED5_GPIO_PORT_NUM, LED5_GPIO_BIT_NUM);

	if(LEDMask & LEDS_LED6)
		GPIO_SetValue(LED6_GPIO_PORT_NUM, LED6_GPIO_BIT_NUM);
	else
		GPIO_ClearValue(LED6_GPIO_PORT_NUM, LED6_GPIO_BIT_NUM);

	if(LEDMask & LEDS_LED7)
		GPIO_SetValue(LED7_GPIO_PORT_NUM, LED7_GPIO_BIT_NUM);
	else
		GPIO_ClearValue(LED7_GPIO_PORT_NUM, LED7_GPIO_BIT_NUM);

	if(LEDMask & LEDS_LED8)
		GPIO_SetValue(LED8_GPIO_PORT_NUM, LED8_GPIO_BIT_NUM);
	else
		GPIO_ClearValue(LED8_GPIO_PORT_NUM, LED8_GPIO_BIT_NUM);
}

void LEDs_ChangeLEDs(uint32_t LEDMask, uint32_t ActiveMask);
void LEDs_ChangeLEDs(uint32_t LEDMask, uint32_t ActiveMask)
{
	if (ActiveMask & LEDS_LED1) {
		GPIO_SetDir(LED1_GPIO_PORT_NUM, LED1_GPIO_BIT_NUM, (LEDMask & LEDS_LED1) != 0);
	}

	if (ActiveMask & LEDS_LED2) {
		GPIO_SetDir(LED2_GPIO_PORT_NUM, LED2_GPIO_BIT_NUM, (LEDMask & LEDS_LED2) != 0);
	}

	if (ActiveMask & LEDS_LED3) {
		GPIO_SetDir(LED3_GPIO_PORT_NUM, LED3_GPIO_BIT_NUM, (LEDMask & LEDS_LED3) != 0);
	}

	if (ActiveMask & LEDS_LED4) {
		GPIO_SetDir(LED4_GPIO_PORT_NUM, LED4_GPIO_BIT_NUM, (LEDMask & LEDS_LED4) != 0);
	}

	if (ActiveMask & LEDS_LED5) {
		GPIO_SetDir(LED5_GPIO_PORT_NUM, LED5_GPIO_BIT_NUM, (LEDMask & LEDS_LED5) != 0);
	}

	if (ActiveMask & LEDS_LED6) {
		GPIO_SetDir(LED6_GPIO_PORT_NUM, LED6_GPIO_BIT_NUM, (LEDMask & LEDS_LED6) != 0);
	}

	if (ActiveMask & LEDS_LED7) {
		GPIO_SetDir(LED7_GPIO_PORT_NUM, LED7_GPIO_BIT_NUM, (LEDMask & LEDS_LED7) != 0);
	}

	if (ActiveMask & LEDS_LED8) {
		GPIO_SetDir(LED8_GPIO_PORT_NUM, LED8_GPIO_BIT_NUM, (LEDMask & LEDS_LED8) != 0);
	}
}

void USB_Connect(void) {};

void bsp_mdelay(int32_t msec) __attribute__((weak));
void bsp_mdelay(int32_t msec)
{
	int32_t curr = (int32_t)LPC_RITIMER->COUNTER;
	int32_t final = curr + ((CPU_FREQ / 1000) * msec);

	/* If the value is zero let us not worry about it */
	if (!msec || msec < 0) return;

	if (final < 0 && curr > 0) {
		while((uint32_t)LPC_RITIMER->COUNTER < (uint32_t)final);
	} else {
		while((int32_t)LPC_RITIMER->COUNTER < final);
	}
}

/**
 * UDA1380 definition
 */
/* UDA1380 Register Address */
typedef enum {
  UDA_EVALM_CLK = 0x00,
  UDA_BUS_CTRL,
  UDA_POWER_CTRL,
  UDA_ANALOG_CTRL,
  UDA_HPAMP_CTRL,
  UDA_MASTER_VOL_CTRL = 0x10,
  UDA_MIXER_VOL_CTRL,
  UDA_MODE_CTRL,
  UDA_MUTE_CTRL,
  UDA_MIXER_FILTER_CTRL,
  UDA_DEC_VOL_CTRL = 0x20,
  UDA_PGA_CTRL,
  UDA_ADC_CTRL,
  UDA_AGC_CTRL,
  UDA_TOTAL_REG
}UDA1380_REG;

/* System Register Data Set */
unsigned short UDA_sys_regs_dat[]={
  /* UDA_EVALM_CLK */
  0xF <<8 | 0x3<<4 | 1,
  /* UDA_BUS_CTRL */
  0x00,
  /* UDA_POWER_CTRL */
  1<<15 | 1 <<13 | 1<<10 | 1<<8,
  /* UDA_ANALOG_CTRL */
  0x0000,
  /* UDA_HPAMP_CTRL */
  1<<9 | 2
};

/* System Register Data Set */
unsigned short UDA_interfil_regs_dat[]={
  /* UDA_MASTER_VOL_CTRL */
  0x0000, /* MAX volume */
  /* UDA_MIXER_VOL_CTRL */
  0x0000,
  /* UDA_MODE_CTRL */
  0x0000,
  /* UDA_MUTE_CTRL */
  2<<8 | 2,
  /* UDA_MIXER_FILTER_CTRL */
  0x0000,
};
/* decimator Register Data Set */
unsigned short UDA_decimator_regs_dat[]={
  /* UDA_DEC_VOL_CTRL */
  0x0000,
  /* UDA_PGA_CTRL */
  0x0000,
  /* UDA_ADC_CTRL */
  0x0000,
  /* UDA_AGC_CTRL */
  0x0000

};
void delay(UNS_32 cnt);
void delay(UNS_32 cnt)
{
    UNS_32 i = cnt;
    while (i != 0) i--;
    return;
}
/* UDA register read function */
unsigned short UDA_Reg_read(UDA1380_REG reg);
unsigned short UDA_Reg_read(UDA1380_REG reg){
  I2C_M_SETUP_Type transferMCfg;
  unsigned char TXdata, RXdata[2];
  unsigned short result=0;
	TXdata = reg;
//    transferMCfg.addr_mode = ADDR7BIT;
    transferMCfg.sl_addr7bit = (0x34>>1);//I2CDEV_UDA1380_ADDR
    transferMCfg.tx_data = &TXdata;
    transferMCfg.tx_length = 1;
    transferMCfg.rx_data = &RXdata[0];
    transferMCfg.rx_length = 2;
    transferMCfg.retransmissions_max = 5;
    I2C_MasterTransferData(LPC_I2C, &transferMCfg, I2C_TRANSFER_POLLING);
    //while (( i2c_mtxrx_setup.status & I2C_SETUP_STATUS_DONE) == 0);
    result = RXdata[0]<<8 | RXdata[1];
	return result;
}

/* UDA register write function */
void UDA_Reg_write(UDA1380_REG reg, unsigned short value);
void UDA_Reg_write(UDA1380_REG reg, unsigned short value){
  I2C_M_SETUP_Type transferMCfg;
  UNS_8  tx_data[3];
  tx_data[0] = reg;
  tx_data[1] = value>>8;
  tx_data[2] = value&0xFF;
//  i2c_mtx_setup.addr_mode = ADDR7BIT;
  transferMCfg.sl_addr7bit = (0x34>>1);//I2CDEV_UDA1380_ADDR
  transferMCfg.tx_data = &tx_data[0];
  transferMCfg.tx_length = 3;
  transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 5;
  I2C_MasterTransferData(LPC_I2C, &transferMCfg, I2C_TRANSFER_POLLING);
  //while (( i2c_mtx_setup.status & I2C_SETUP_STATUS_DONE) == 0);
  delay(10000);
}
BOOL_32 UDA1380_init(void);
BOOL_32 UDA1380_init(void){
  unsigned short temp;
  int i;

  LPC_CGU->IDIVC_CTRL = (0x8<<24) | (7<<2) |(1<<11);
  LPC_CGU->BASE_OUT_CLK = 0x0D<<24 | (1<<11);
  LPC_SCU->SFSCLK_3 = 1 | (0x3<<3);

  /* Reset device */
  UDA_Reg_write(0x7F, 0x00);

    /* Sys regs init */
	UDA_Reg_write(UDA_EVALM_CLK , 0x0502);
  for(i=1; i<5; i++){
    UDA_Reg_write(UDA_EVALM_CLK + i, UDA_sys_regs_dat[i]);
    temp = UDA_Reg_read(UDA_EVALM_CLK + i);
    if(temp!=UDA_sys_regs_dat[i]) return FALSE;
  }

  /* interfilter regs init */
  for(i=0; i<5; i++){
    UDA_Reg_write(UDA_MASTER_VOL_CTRL + i, UDA_interfil_regs_dat[i]);
    temp = UDA_Reg_read(UDA_MASTER_VOL_CTRL + i);
    if(temp!=UDA_interfil_regs_dat[i]) return FALSE;
  }
  /* decimator regs init */
  for(i=0; i<4; i++){
    UDA_Reg_write(UDA_DEC_VOL_CTRL + i, UDA_decimator_regs_dat[i]);
    temp = UDA_Reg_read(UDA_DEC_VOL_CTRL + i);
    if(temp!=UDA_decimator_regs_dat[i]) return FALSE;
  }
	UDA_Reg_write(UDA_EVALM_CLK, UDA_sys_regs_dat[0]);

  return TRUE;
}
/**
 * Audio API
 */
LPC_I2Sn_Type *I2S = LPC_I2S0;
#define I2SIRQ		I2S0_IRQn
#define AudioIrq	I2S0_IRQHandler
#define I2S_TX_LEVEL	4
/** Audio max packet count. */
#define AUDIO_MAX_PC	10

/** Audio Data Buffer, used by BSP output and ISO transfer.
	This is a store, not all space will be used in application. */
uint8_t audio_buffer[2048] __attribute__ ((aligned(4)));
uint32_t audio_buffer_size = 0;
uint32_t audio_buffer_rd_index = 0;
uint32_t audio_buffer_wr_index = 0;
uint32_t audio_buffer_count = 0;

/** Reset Audio Buffer */
void Audio_Reset_Data_Buffer(void);

void Audio_Init(uint32_t samplefreq)
{
	I2S_MODEConf_Type I2S_ClkConfig;
	I2S_CFG_Type I2S_ConfigStruct;

	Audio_Reset_Data_Buffer();
	NVIC_DisableIRQ(I2SIRQ);

	switch(samplefreq){
	case 11025:
	case 22050:
	case 44100:
		audio_buffer_size = 1764;
		break;
	case 8000:
	case 16000:
	case 32000:
	case 48000:
	default:
		audio_buffer_size = samplefreq * 4 * AUDIO_MAX_PC / 1000;
		break;
	}

	/* Reset UDA1380 */
	scu_pinmux(8,0,MD_PUP, FUNC0);
	GPIO_SetDir(4, 1<<0, 1);
	GPIO_ClearValue(4, 1<<0);

/* Initialize I2S peripheral ------------------------------------*/
	/* Init I2C */
	I2C_Init(LPC_I2C, 100000);
	/* Enable Slave I2C operation */
	I2C_Cmd(LPC_I2C, ENABLE);
	/* Init UDA1380 CODEC */
	UDA1380_init();
/* Initialize I2S peripheral ------------------------------------*/
	scu_pinmux(6,0, MD_PUP, 4);			// I2S_RX_SCK
	scu_pinmux(6,1, MD_PUP, 3);			// I2S_RX_WS
	scu_pinmux(6,2, MD_PUP, 3);			// I2S_RX_SDA

	scu_pinmux(3,0, MD_PUP, 2);			// I2S_TX_SCK
	scu_pinmux(7,1, MD_PUP, 2);			// I2S_TX_WS
	scu_pinmux(7,2, MD_PUP, 2);			// I2S_TX_SDA

	I2S_Init(I2S);

	/* setup:
	 * 		- wordwidth: 16 bits
	 * 		- stereo mode
	 * 		- master mode for I2S_TX
	 * 		- Frequency = 44.1 kHz
	 */

	/* Audio Config*/
	I2S_ConfigStruct.wordwidth = I2S_WORDWIDTH_16;
	I2S_ConfigStruct.mono = I2S_STEREO;
	I2S_ConfigStruct.stop = I2S_STOP_ENABLE;
	I2S_ConfigStruct.reset = I2S_RESET_ENABLE;
	I2S_ConfigStruct.ws_sel = I2S_MASTER_MODE;
	I2S_ConfigStruct.mute = I2S_MUTE_DISABLE;
	I2S_Config(I2S,I2S_TX_MODE,&I2S_ConfigStruct);

	/* Clock Mode Config*/
	I2S_ClkConfig.clksel = I2S_CLKSEL_FRDCLK;
	I2S_ClkConfig.fpin = I2S_4PIN_DISABLE;
	I2S_ClkConfig.mcena = I2S_MCLK_DISABLE;
	I2S_ModeConfig(I2S,&I2S_ClkConfig,I2S_TX_MODE);

	I2S_FreqConfig(I2S, samplefreq, I2S_TX_MODE);

	I2S_Stop(I2S, I2S_TX_MODE);

	/* TX FIFO depth is 4 */
	I2S_IRQConfig(I2S,I2S_TX_MODE,I2S_TX_LEVEL);
	I2S_IRQCmd(I2S,I2S_TX_MODE,ENABLE);
	I2S_Start(I2S);
	NVIC_EnableIRQ(I2SIRQ);
}

void AudioIrq(void);
void AudioIrq(void)
{
	uint32_t txlevel,i;
	txlevel = I2S_GetLevel(I2S,I2S_TX_MODE);
	if(txlevel <= I2S_TX_LEVEL)
	{
		for(i=0;i<8-txlevel;i++)
		{
			if(audio_buffer_count >= 4) /*has enough data */
			{
				audio_buffer_count -= 4;
				I2S->TXFIFO = *(uint32_t *)(audio_buffer + audio_buffer_rd_index);
				audio_buffer_rd_index+=4;
				if(audio_buffer_rd_index >= audio_buffer_size)
					audio_buffer_rd_index -= audio_buffer_size;
			}else{
				I2S->TXFIFO = 0;
			}

			/*Skip some samples if buffer run writting too fast. */
			if((audio_buffer_size != 0) && (audio_buffer_count >= (audio_buffer_size/2)))
			{
				audio_buffer_count-=4;
				audio_buffer_rd_index+=4;
				if(audio_buffer_rd_index >= audio_buffer_size)
					audio_buffer_rd_index -= audio_buffer_size;
			}
		}
	}
}

/** Provide Audio buffer for next iso transfer */
uint32_t Audio_Get_ISO_Buffer_Address(uint32_t last_packet_size)
{
	audio_buffer_wr_index += last_packet_size;
	audio_buffer_count += last_packet_size;

	if(audio_buffer_count > audio_buffer_size)
	{
		Audio_Reset_Data_Buffer();
	}

	if(audio_buffer_wr_index >= audio_buffer_size)
		audio_buffer_wr_index -= audio_buffer_size;

	return (uint32_t)&audio_buffer[audio_buffer_wr_index];
}
/* Reset Audio Data Buffer. */
void Audio_Reset_Data_Buffer(void)
{
	audio_buffer_count = 0;
	audio_buffer_wr_index = 0;
	audio_buffer_rd_index = 0;
}

#if 0
/* Timer API */
static uint32_t USBSystemTicks = 0;
void SysTick_Handler(void)
{
	USBSystemTicks++;
}

uint32_t System_Get10msTicks(void)
{
	return USBSystemTicks;
}

#define USE_SPIFI_LIB

#if defined(__GNUC__) && defined(__LPC18XX__)
#define CACHE_SIZE	(4096)
#else
#define CACHE_SIZE	(0x10000)
#endif

#include "spifi_rom_api.h"

static SPIFI_RTNS * pSpifi;
static SPIFIobj Spifi_obj;

/* cache for SPIFI chip
 * Spansion IC requires 64k block erase */
#if defined(__CC_ARM)
uint8_t* disk_cache = (uint8_t*)0x10008000;
#elif defined(__GNUC__)
uint8_t disk_cache[CACHE_SIZE];
#endif
#define SPIFI_ADDRESS	(0x80000000)
static int32_t disk_block = -1;
static uint32_t cache_update_flag = 0;
static uint32_t disk_cache_flush_time_state = 0;

/** Initialize the Mass Storage, depend on implementation of board, it can be
 * SPIFI, NOR Flash, NAND Flash, RAM.. */
void MassStorage_Init(void)
{
	/* set SPIFI clock */
	CGU_EntityConnect(CGU_CLKSRC_IRC, CGU_BASE_SPIFI); /* IRC 12 MHz is good enough for us */

	/* set up SPIFI I/O (undocumented bit 7 set as 1, Aug 2 2011) */
	LPC_SCU->SFSP3_3 = 0xF3; /* high drive for SCLK */
	/* IO pins */
	LPC_SCU->SFSP3_4 = 0xD3;
	LPC_SCU->SFSP3_5 = 0xD3;
	LPC_SCU->SFSP3_6 = 0xD3;
	LPC_SCU->SFSP3_7 = 0xD3;
	LPC_SCU->SFSP3_8 = 0x13; /* CS doesn't need feedback */

	pSpifi = &spifi_table;

	/* Initialize SPIFI driver */
	if (pSpifi->spifi_init(&Spifi_obj, 3, /*S_RCVCLK |*/S_MAXIMAL| S_FULLCLK, 12))
	{
		/*TODO: initialized fail */
		while(1);
	}
#if defined(__GNUC__) && defined(__LPC18XX__)
	Spifi_obj.erase_ops[0] = 0x20;
	Spifi_obj.erase_shifts[0] = 12;
#endif
}

uint32_t MassStorage_Cache_Flush(bool appcall);
/** Program Mass Memory */
void MassStorage_Write(uint32_t diskpos, void* buffer, uint32_t size)
{
	uint32_t block = (uint32_t)diskpos/CACHE_SIZE;
	uint32_t block_offset = (uint32_t)diskpos%CACHE_SIZE;
	if(block!=disk_block)
	{
		MassStorage_Cache_Flush(false);
		memcpy(disk_cache, (void*)(SPIFI_ADDRESS+block*CACHE_SIZE), CACHE_SIZE);
		disk_block = block;
	}
	memcpy(&disk_cache[block_offset], buffer, size);
	cache_update_flag = 1;
}

void MassStorage_Read(uint32_t diskpos, void* buffer, uint32_t size){
	uint32_t block = (uint32_t)diskpos/CACHE_SIZE;
	uint32_t block_offset = (uint32_t)diskpos%CACHE_SIZE;
	if(block!=disk_block)
	{
		MassStorage_Cache_Flush(false);
		memcpy(disk_cache, (void*)(SPIFI_ADDRESS+block*CACHE_SIZE), CACHE_SIZE);
		disk_block = block;
	}
	memcpy(buffer, &disk_cache[block_offset], size);
}

/** Verify Mass Memory with other Memory */
bool MassStorage_Verify(void*dest, void* source, uint32_t size){
	if(memcmp((void*)((uint32_t)dest + Spifi_obj.base), source, size)) return false;
	return true;

}
/** Get Mass Storage Size */
uint32_t MassStorage_GetCapacity(void)
{
#if defined(__GNUC__) && defined(__LPC18XX__)
	return 0x20000;	//use 2 first sectors
#else
	return Spifi_obj.devSize;
#endif
}

uint32_t MassStorage_Cache_Flush(bool appcall)
{
	SPIFIopers opers;
	int res;
	if(cache_update_flag)
	{
		if(appcall) HAL_DisableUSBInterrupt();
		opers.length = CACHE_SIZE;
		opers.scratch = (char*)0;
		opers.protect = 0;
		opers.options = 0;
		/* Write */
		opers.dest = (char *)((disk_block*CACHE_SIZE) + Spifi_obj.base);

		if (res = pSpifi->spifi_program (&Spifi_obj, (char *)disk_cache, &opers)) {
			/*TODO: Program fail */
			while(res);
		}
		cache_update_flag = 0;
		if(appcall) HAL_EnableUSBInterrupt();
	}
	return 0;
}

/*  Timer Expires at 640 ms */
bool MassStorage_FlushTimeOut(void)
{
	if((USBSystemTicks & 0x40) != disk_cache_flush_time_state)
	{
		disk_cache_flush_time_state = (USBSystemTicks & 0x40);
		return true;
	}
	return false;
}
#else
extern uint8_t DiskImage[];
#include "../DataRam.h"
#include <string.h>
/** Initialize the Mass Storage, depend on implementation of board, it can be
 * SPIFI, NOR Flash, NAND Flash, RAM.. */
void MassStorage_Init(void)
{
}

uint32_t MassStorage_Cache_Flush(bool appcall);
/** Program Mass Memory */
void MassStorage_Write(uint32_t diskpos, void* buffer, uint32_t size)
{
	uint32_t block = (uint32_t)diskpos/size;
	if (block < DATA_RAM_PHYSICAL_SIZE/size)
	{
		memcpy(&DiskImage[block*size], buffer, size);
	}
}

void MassStorage_Read(uint32_t diskpos, void* buffer, uint32_t size){
	uint32_t block = (uint32_t)diskpos/size;
	if (block < DATA_RAM_PHYSICAL_SIZE/size)
	{
		memcpy(buffer, &DiskImage[block*size], size);
	}else
		memset(buffer, 0, size);
}

/** Verify Mass Memory with other Memory */
bool MassStorage_Verify(void*dest, void* source, uint32_t size){
	return true;

}
/** Get Mass Storage Size */
uint32_t MassStorage_GetCapacity(void)
{
	return DATA_RAM_VIRTUAL_SIZE;	//use 2 first sectors
}

uint32_t MassStorage_Cache_Flush(bool appcall)
{
	return 0;
}

bool MassStorage_FlushTimeOut(void)
{
	return false;
}
#endif

/**
 * Ethernet API
 */
#ifndef LPC_EMAC_RMII
#error LPC_EMAC_RMII is not defined!
#endif

/* Hitex4350 RMII/MII board setup
RMII mode:
Jumper state    PHY        Chip              CPIN     I/O      FUNC
JP15-1-2        RMII boot
JP5-1-2         X1/50M
JP14-1-2                   ENET_REF_CLK/50M  P1.19    I        0
SV3-15-16 dm               ENET_REF_CLK/50M  P1.19    I        0
SV3-13-14 c     TX_EN      ENET_TX_EN        P0.1     O        6
SV3-11-12 c     TXD0       ENET_TXD0         P1.18    O        3
SV3-9-10  c     TXD1       ENET_TXD1         P1.20    O        3
SV3-7-8   dm    TXD2       ENET_TXD2         P9.4     na
SV3-5-6   dm    TXD3       ENET_TXD3         P9.5     na
SV5-3-4   dm    TXD3       ENET_TXD3         P9.5     na
SV3-3-4   c     MDIO       ENET_MDIO         P1.17    I/O      3
SV3-1-2   c     MDC        ENET_MDC          PC.1     O        3
SV13-1-2  o     MDC        ENET_MDC          PC.1     O        3
SV6-1-2   dm    RX_CLK     ENET_RX_CLK       PC.0     na
SV12-3-4  dm    RX_CLK     ENET_RX_CLK       PC.0     na
SV6-3-4   c     RX_DV      ENET_RX_DV        P1.16    I        7
SV6-5-6   dm    CRS        ENET_CRS          P9.0     na
SV8-7-8   dm    CRS        ENET_CRS          P9.0     na
SV6-7-8   dm    RX_ER      ENET_RX_ERR       P9.1     na
SV8-5-6   dm    RX_ER      ENET_RX_ERR       P9.1     na
SV6-9-10  dm    COL        ENET_COL          P9.6     na
SV5-5-6   dm    COL        ENET_COL          P9.6     na
SV6-11-12 c     RXD0       ENET_RXD0         P1.15    I        3
SV6-13-14 c     RXD1       ENET_RXD1         P0.0     I        2
SV6-15-16 dm    RXD2       ENET_RXD2         P9.3     na
SV8-3-4   dm    RXD2       ENET_RXD2         P9.3     na
SV6-17-18 dm    RXD3       ENET_RXD3         P9.2     na
SV8-1-2   dm    RXD3       ENET_RXD3         P9.2     na

MII mode:
Jumper state    PHY        Chip              CPIN     I/O      FUNC
JP15      o     MII boot
NOTE: JP15 is shown as a 2-pin jumper on the schematics, but is a
NOTE: 3-pin jumper on the board. Make sure the jumper is open and
NOTE: not connected between pins 2-3.
JP5-2-3         X1/25M
JP14-2-3                   ENET_REF_CLK/25M  P1.19    I        0
SV3-15-16 c                ENET_REF_CLK/25M  P1.19    I        0
SV3-13-14 c     TX_EN      ENET_TX_EN        P0.1     O        6
SV3-11-12 c     TXD0       ENET_TXD0         P1.18    O        3
SV3-9-10  c     TXD1       ENET_TXD1         P1.20    O        3
SV3-7-8   c     TXD2       ENET_TXD2         P9.4     O		   5
SV3-5-6   c     TXD3       ENET_TXD3         P9.5     O		   5
SV5-3-4   c     TXD3       ENET_TXD3         P9.5     O        5
SV3-3-4   c     MDIO       ENET_MDIO         P1.17    I/O      3
SV3-1-2   c     MDC        ENET_MDC          PC.1     O        3
SV13-1-2  o     MDC        ENET_MDC          PC.1     O        3
SV6-1-2   c     RX_CLK     ENET_RX_CLK       PC.0     I/O      3
SV12-3-4  dm    RX_CLK     ENET_RX_CLK       PC.0     I/O      3
SV6-3-4   c     RX_DV      ENET_RX_DV        P1.16    I        7
SV6-5-6   c     CRS        ENET_CRS          P9.0     I        5
SV8-7-8   o     CRS        ENET_CRS          P9.0     I        5
SV6-7-8   c     RX_ER      ENET_RX_ER        P9.1     I        5
SV8-5-6   o     RX_ER      ENET_RX_ER        P9.1     I        5
SV6-9-10  c     COL        ENET_COL          P9.6     I		   5
SV5-5-6   o     COL        ENET_COL          P9.6     I        5
SV6-11-12 c     RXD0       ENET_RXD0         P1.15    I        3
SV6-13-14 c     RXD1       ENET_RXD1         P0.0     I        2
SV6-15-16 c     RXD2       ENET_RXD2         P9.3     I        5
SV8-3-4   o     RXD2       ENET_RXD2         P9.3     I		   5
SV6-17-18 c     RXD3       ENET_RXD3         P9.2     I		   5
SV8-1-2   o     RXD3       ENET_RXD3         P9.2     I		   5

o = open, c = closed, dm = doesn't matter
Note: Repeated signals indicate multiple jumpers that need to be set
*/

/* SCR pin definitions for RMII and MII modes */
#define EMAC_HIGHSLEW_INPUT  (MD_EHS | MD_PLN | MD_EZI | MD_ZI)
#define EMAC_HIGHSLEW_OUTPUT (MD_EHS | MD_PLN | MD_ZI)
#define EMAC_INPUT           (MD_PLN | MD_EZI)
#define EMAC_OUTPUT          (MD_PLN)

void Ethernet_Init(void)
{
	/* EMAC clocking is handled in the EMAC driver, but make sure
	   the board has the correct jumper setup for RMII or MII mode.
	   See the specific RMII/MII jumper settings in the table in this
	   file. */

	LPC_CREG->CREG6 &= ~0x7;

#if LPC_EMAC_RMII == 1
	/* RMII mode setup only */
	LPC_CREG->CREG6 |= 0x4;

	/* MIIM pins */
	scu_pinmux(1,   17, EMAC_HIGHSLEW_INPUT,  FUNC3); /* P1.17 = ENET_MDIO    */
	scu_pinmux(0xC, 1,  EMAC_HIGHSLEW_OUTPUT, FUNC3); /* PC.1  = ENET_MDC     */

	/* RMII pins */
	scu_pinmux(1,   15, EMAC_HIGHSLEW_INPUT,  FUNC3); /* P1.15 = ENET_RXD0    */
	scu_pinmux(0,   0,  EMAC_HIGHSLEW_INPUT,  FUNC2); /* P0.0  = ENET_RXD1    */
	scu_pinmux(1,   18, EMAC_HIGHSLEW_OUTPUT, FUNC3); /* P1.18 = ENET_TXD0    */
	scu_pinmux(1,   20, EMAC_HIGHSLEW_OUTPUT, FUNC3); /* P1.20 = ENET_TXD1    */
	scu_pinmux(1,   16, EMAC_HIGHSLEW_INPUT,  FUNC7); /* P1.16 = ENET_RX_DV   */
	scu_pinmux(1,   19, EMAC_HIGHSLEW_INPUT,  FUNC0); /* P1.19 = ENET_TX_CLK  */
	scu_pinmux(0,   1,  EMAC_HIGHSLEW_OUTPUT, FUNC6); /* P0.1  = ENET_TX_EN   */

#else

	/* MIIM pins */
  scu_pinmux(1,   17, EMAC_INPUT,  FUNC3); /* P1.17 = ENET_MDIO    */
	scu_pinmux(0xC, 1,  EMAC_OUTPUT, FUNC3); /* PC.1  = ENET_MDC     */

	/* MII pins */
	scu_pinmux(1,   15, EMAC_INPUT,  FUNC3); /* P1.15 = ENET_RXD0    */
	scu_pinmux(0,   0,  EMAC_INPUT,  FUNC2); /* P0.0  = ENET_RXD1    */
	scu_pinmux(0xC, 6,  EMAC_INPUT,  FUNC3); /* PC.6  = ENET_RXD2    */
	scu_pinmux(0xC, 7,  EMAC_INPUT,  FUNC3); /* PC.7  = ENET_RXD3    */

	scu_pinmux(1,   18, EMAC_OUTPUT, FUNC3); /* P1.18 = ENET_TXD0    */
	scu_pinmux(1,   20, EMAC_OUTPUT, FUNC3); /* P1.20 = ENET_TXD1    */
	scu_pinmux(0xC, 2,  EMAC_OUTPUT, FUNC3); /* PC.2  = ENET_TXD2    */
	scu_pinmux(0xC, 3,  EMAC_OUTPUT, FUNC3); /* PC.3  = ENET_TXD3    */

	scu_pinmux(0,   1,  EMAC_INPUT,  FUNC2); /* P0.1  = ENET_COL     */
	scu_pinmux(1,   19, EMAC_INPUT,  FUNC0); /* P1.19 = ENET_TX_CLK  */
	scu_pinmux(0xC, 5,  EMAC_OUTPUT, FUNC3); /* PC.5  = ENET_TX_ER   */
	scu_pinmux(0xC, 0,  EMAC_INPUT,  FUNC3); /* PC.0  = ENET_RX_CLK  */
	scu_pinmux(0xC, 8,  EMAC_INPUT,  FUNC4); /* PC.8  = ENET_RX_DV  */
#endif
}

void Ethernet_Get_MACaddr(u8_t *macaddr)
{
	macaddr[0] = LPC_EMAC_ADDR0;
	macaddr[1] = LPC_EMAC_ADDR1;
	macaddr[2] = LPC_EMAC_ADDR2;
	macaddr[3] = LPC_EMAC_ADDR3;
	macaddr[4] = LPC_EMAC_ADDR4;
	macaddr[5] = LPC_EMAC_ADDR5;
}

uint32_t Ethernet_IsRMII(void)
{
	return LPC_EMAC_RMII;
}
#endif

