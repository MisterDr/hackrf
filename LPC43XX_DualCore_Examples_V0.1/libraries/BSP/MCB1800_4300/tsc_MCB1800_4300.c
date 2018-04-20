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


#include "../bsp_internal.h"
#include <string.h>

#if (BOARD == BOARD_MCB1800_4300)
/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#define  I2C_ID0 LPC_I2C0
#define  I2C_CLOCK_RATE  (400000)
#define  TSC_I2C_ADDR    (0x82 >> 1) 

#define BUFFER_SIZE			0x10

/* STMPE811 Register addresses */
#define SYS_CTRL1       0x03
#define SYS_CTRL2       0x04
#define INT_CTRL        0x09
#define INT_EN          0x0A
#define INT_STA         0x0B
#define GPIO_ALT_FUNCT  0x17
#define ADC_CTRL1       0x20
#define ADC_CTRL2       0x21
#define TSC_CTRL        0x40
#define TSC_CFG         0x41
#define FIFO_TH         0x4A
#define FIFO_STA        0x4B
#define FIFO_SIZE       0x4C
#define DATA_X          0x4D
#define DATA_Y          0x4F
#define DATA_Z          0x51
#define TSC_FRACTION_Z  0x56
#define TSC_I_DRIVE     0x58
#define TSC_SHIELD      0x59
#define DATA_XYZ        0xD7

#define C_GLCD_H_SIZE	240
#define C_GLCD_V_SIZE	320

static uint8_t master_txbuf[BUFFER_SIZE];
static uint8_t master_rxbuf[BUFFER_SIZE];

uint32_t g_isPenDn;
uint32_t g_isNewPenDn;

const int32_t ad_left = 3813;
const int32_t ad_top = 3805;//237;
const int32_t ad_right = 360;
const int32_t ad_bottom = 237;//3805;

static uint32_t TSC_WriteReg(uint8_t regAddr, uint8_t value)
{
	I2C_M_SETUP_Type transferMCfg;
	
	/* Initialise Tx buffer */
	master_txbuf[0] = regAddr;
	master_txbuf[1] = value;
	
	transferMCfg.sl_addr7bit = TSC_I2C_ADDR;
	transferMCfg.tx_data = master_txbuf ;
	transferMCfg.tx_length = 2;
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;
	transferMCfg.retransmissions_max = 3;
	transferMCfg.tx_count = 0;
	transferMCfg.rx_count = 0;
	transferMCfg.retransmissions_count = 0;
	I2C_MasterTransferData(I2C_ID0, &transferMCfg, I2C_TRANSFER_POLLING);
	
	return transferMCfg.tx_count;
}

static uint32_t TSC_ReadReg (uint8_t regAddr, uint8_t *value) 
{
	I2C_M_SETUP_Type transferMCfg;
	
	/* Initialise Tx buffer */
	master_txbuf[0] = regAddr;
	
	transferMCfg.sl_addr7bit = TSC_I2C_ADDR;
	transferMCfg.tx_data = master_txbuf ;
	transferMCfg.tx_length = 1;
	transferMCfg.rx_data = master_rxbuf;
	transferMCfg.rx_length = 1;
	transferMCfg.retransmissions_max = 3;
	transferMCfg.tx_count = 0;
	transferMCfg.rx_count = 0;
	transferMCfg.retransmissions_count = 0;
	I2C_MasterTransferData(I2C_ID0, &transferMCfg, I2C_TRANSFER_POLLING);
	
	*value = master_rxbuf[0];
	
	return transferMCfg.rx_count;
}

static bool TSC_TouchDetect (void) 
{
	uint8_t  CtrRegVal = 0;
  
	if (TSC_ReadReg (TSC_CTRL, &CtrRegVal) == 1) {
		if (CtrRegVal & (1 << 7)) {
			return true;
		}
	}
	return false;
}

static void TSC_GetTouchCoord (int16_t* x, int16_t* y) 
{
	I2C_M_SETUP_Type transferMCfg;
	static uint8_t	fifo_size, i = 0;

	/* Read all samples except the last one */
	TSC_ReadReg (FIFO_SIZE, &fifo_size);
	for(i =0; i < fifo_size; ++i)
	{
		master_txbuf[0] = DATA_XYZ;
		transferMCfg.sl_addr7bit = TSC_I2C_ADDR;
		transferMCfg.tx_data = master_txbuf;
		transferMCfg.tx_length = 1;
		transferMCfg.rx_data = master_rxbuf;
		transferMCfg.rx_length = 4;
		transferMCfg.retransmissions_max = 3;
		transferMCfg.tx_count = 0;
		transferMCfg.rx_count = 0;
		transferMCfg.retransmissions_count = 0;
		I2C_MasterTransferData(I2C_ID0, &transferMCfg, I2C_TRANSFER_POLLING);
	}

	/* Retrieve last taken sample */
	master_txbuf[0] = DATA_XYZ;
	transferMCfg.sl_addr7bit = TSC_I2C_ADDR;
	transferMCfg.tx_data = master_txbuf;
	transferMCfg.tx_length = 1;
	transferMCfg.rx_data = master_rxbuf;
	transferMCfg.rx_length = 4;
	transferMCfg.retransmissions_max = 3;
	transferMCfg.tx_count = 0;
	transferMCfg.rx_count = 0;
	transferMCfg.retransmissions_count = 0;
	I2C_MasterTransferData(I2C_ID0, &transferMCfg, I2C_TRANSFER_POLLING);
	
	*x = (master_rxbuf[0] << 4) | ((master_rxbuf[1] & 0xF0) >> 4);
	*y = ((master_rxbuf[1] & 0x0F) << 8) | master_rxbuf[2];

	/* Clear interrupt flags */
	TSC_WriteReg (INT_STA, 0x1F);

	return;
}

void TSC_Controller_Init(void)
{
	volatile int i;
	
	/* Initialise I2C0 for TSC */
	I2C_Init(I2C_ID0, I2C_CLOCK_RATE);
	I2C_Cmd(I2C_ID0, ENABLE);
		
	/* Reset Touch-screen controller	*/
	TSC_WriteReg (SYS_CTRL1, 0x02);

	for (i = 0; i < 200000; i++);

	/* Initialise TSC registers */
	/* Enable TSC and ADC */
	TSC_WriteReg (SYS_CTRL2, 0x0C);
	
	/* Enable Touch detect, FIFO */
	TSC_WriteReg (INT_EN, 0x07);
	
	/* Set sample time, 12-bit mode */
	TSC_WriteReg (ADC_CTRL1, 0x69);

	for (i = 0; i < 40000; i++);

	/* ADC frequency 3.25 MHz */
	TSC_WriteReg (ADC_CTRL2, 0x01);
	
	/* Tracking index: 8, operation mode : XY only */
	TSC_WriteReg (TSC_CTRL, 0x22);
	
	/* Detect delay 10us Settle time 500us*/
	TSC_WriteReg (TSC_CFG, 0xC2);
	
	/* Threshold for FIFO */
	TSC_WriteReg (FIFO_TH, 0x01);
	
	/* FIFO reset */
	TSC_WriteReg (FIFO_STA, 0x01);
	
	/* FIFO not reset */
	TSC_WriteReg (FIFO_STA, 0x00);
	
	/* Drive 50 mA typical */
	TSC_WriteReg (TSC_I_DRIVE, 0x01);
	
	/* Pins are used for touchscreen */
	TSC_WriteReg (GPIO_ALT_FUNCT, 0x00);
	
	/* Enable TSC	*/
	TSC_WriteReg (TSC_CTRL, 0x01);
	
	/* Clear interrupt status */
	TSC_WriteReg (INT_STA, 0xFF);
}

bool TSC_Controller_GetTouchPos(int16_t *pX, int16_t *pY)
{
	int16_t x, y, rng;
	
	if (TSC_TouchDetect() == true)
	{
		TSC_GetTouchCoord (&x, &y);
		g_isPenDn = 1;
		g_isNewPenDn = 1;
		
		// calibrate X
		rng = ad_right - ad_left;
		if (rng < 0)
			rng = - rng;
		x -= (ad_right < ad_left) ? ad_right : ad_left;
		
		*pX = (x * C_GLCD_H_SIZE) / rng;
		if (ad_left > ad_right)
			*pX = C_GLCD_H_SIZE - *pX;
		
		// calibrate Y
		rng = ad_bottom - ad_top;
		if (rng < 0)
			rng = - rng;
		y -= (ad_bottom < ad_top) ? ad_bottom : ad_top;
		
		*pY = (y * C_GLCD_V_SIZE) / rng;
		if (ad_top > ad_bottom)
			*pY = C_GLCD_V_SIZE - *pY;
	}
	else
	{
			g_isPenDn = 0;
	}
	
	if (g_isNewPenDn)
	{
		g_isNewPenDn = 0;
		if (*pX < 0)
			*pX = -*pX;
		if (*pY < 0)
			*pY = -*pY;
		return true;
	}
	return false;
}

void GUI_TOUCH_X_ActivateX(void)
{
	return;
}

void GUI_TOUCH_X_ActivateY(void)
{
	return;
}

int  GUI_TOUCH_X_MeasureX(void)
{
  return 0;
}

int  GUI_TOUCH_X_MeasureY(void)
{
  return 0;
}

#endif
