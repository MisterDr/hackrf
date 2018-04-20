/*
 * @brief Hitex EVA 1850/4350 board file
 *
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

#include "board.h"
#include "string.h"

/* Include other sources files specific to this board */
#include "lpc_phy_dp83848.c"
#include "retarget.c"

/** @ingroup BOARD_HITEX_EVA_18504350
 * @{
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Port and bit mapping for LEDs on GPIOs */
static const uint8_t ledports[] = {4, 7, 7, 7};
static const uint8_t ledbits[] = {1, 5, 6, 8};

/* TSC2046 control byte definitions */
#define TSC2046_CONVERSION_BITS            12
#define TSC_START                          (0x01 << 7)
#define TSC_CHANNEL_X                      (0x05 << 4)		/* differential */
#define TSC_CHANNEL_Y                      (0x01 << 4)		/* differential */
#define TSC_CHANNEL_Z1                     (0x03 << 4)		/* differential */
#define TSC_CHANNEL_Z2                     (0x04 << 4)		/* differential */
#define TSC_8BIT                           (0x01 << 3)
#define TSC_12BIT                          (0x00 << 3)
#define TSC_PD                              0x00
#define TSC_ADC_ON                          0x01
#define TSC_REF_ON                          0x02
#if (TSC2046_CONVERSION_BITS == 12)
#define TSC_CONVERSION_MODE                 TSC_12BIT
#else
#define TSC_CONVERSION_MODE                 TSC_8BIT
#endif

#define TSC_SER_MODE                       (0x01 << 2)	/* Single-Ended Reference Mode */
#define TSC_DFR_MODE                       (0x00 << 2)	/* Differential Reference Mode */

#define X_MEASURE      (TSC_START | TSC_CHANNEL_X  | TSC_CONVERSION_MODE | TSC_DFR_MODE | TSC_ADC_ON)
#define Y_MEASURE      (TSC_START | TSC_CHANNEL_Y  | TSC_CONVERSION_MODE | TSC_DFR_MODE | TSC_ADC_ON)
#define Z1_MEASURE     (TSC_START | TSC_CHANNEL_Z1 | TSC_CONVERSION_MODE | TSC_DFR_MODE | TSC_ADC_ON)
#define Z2_MEASURE     (TSC_START | TSC_CHANNEL_Z2 | TSC_CONVERSION_MODE | TSC_DFR_MODE | TSC_ADC_ON)
#define PWRDOWN          (TSC_START | TSC_CHANNEL_Y  | TSC_CONVERSION_MODE | TSC_DFR_MODE | TSC_PD)

typedef struct {
	int16_t ad_left;						/* left margin */
	int16_t ad_right;						/* right margin */
	int16_t ad_top;							/* top margin */
	int16_t ad_bottom;						/* bottom margin */
	int16_t lcd_width;						/* lcd horizontal size */
	int16_t lcd_height;						/* lcd vertical size */
	uint8_t swap_xy;						/* 1: swap x-y coords */
} TSC2046_Init_T;

#define LCD_DC_CMD          (Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 0x07, 8, false))
#define LCD_DC_DATA         (Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 0x07, 8, true))

#if (TSC2046_CONVERSION_BITS == 12)
#define TSC2046_COORD_MAX           (0xFFF)
#define TSC2046_DELTA_VARIANCE      (0x50)
#else
#define TSC2046_COORD_MAX           (0xFF)
#define TSC2046_DELTA_VARIANCE      (0x03)
#endif
#define COORD_GET_NUM                 (10)

/** Local variables */
static TSC2046_Init_T TSC_Config = {
	3758, 149, 3914, 163, 240, 320, 1
};

/**
 * LCD configuration data
 */
const LCD_Config_T EA320x240 = {
	28,		/* Horizontal back porch in clocks */
	10,		/* Horizontal front porch in clocks */
	2,		/* HSYNC pulse width in clocks */
	240,	/* Pixels per line */
	2,		/* Vertical back porch in clocks */
	1,		/* Vertical front porch in clocks */
	2,		/* VSYNC pulse width in clocks */
	320,	/* Lines per panel */
	0,		/* Invert output enable, 1 = invert */
	1,		/* Invert panel clock, 1 = invert */
	1,		/* Invert HSYNC, 1 = invert */
	1,		/* Invert VSYNC, 1 = invert */
	1,		/* AC bias frequency in clocks (not used) */
	6,		/* Maximum bits per pixel the display supports */
	LCD_TFT,		/* LCD panel type */
	LCD_COLOR_FORMAT_BGR,		/* BGR or RGB */
	0		/* Dual panel, 1 = dual panel display */
};

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Very simple (inaccurate) delay function */
static void delayMs(uint32_t ms)
{
	uint32_t i;
	for (i = 0; i < 100 * ms; i++) {}
}

/* Additional (SPI) pin configuration for LCD interface signals */
static void lcdPinConfig(void)
{
	/* PC.11 connected to GPIO = SSEL_MUX_A, PC.12 connected to GPIO = SSEL_MUX_B */
	Chip_SCU_PinMuxSet(0xC, 11, (SCU_MODE_MODE_PULLUP | SCU_MODE_FUNC4));
	Chip_SCU_PinMuxSet(0xC, 12, (SCU_MODE_MODE_PULLUP | SCU_MODE_FUNC4));
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 0x6, 10, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 0x6, 11, true);

	/* Configure SSP0 pins */
	/* PF.0 connected to SCL/SCLK */
	Chip_SCU_PinMuxSet(0xF, 0, (SCU_PINIO_FAST | SCU_MODE_FUNC0));
	/* PF.1 connected to nCS */
	Chip_SCU_PinMuxSet(0xF, 1, (SCU_PINIO_FAST | SCU_MODE_FUNC2));
	/* PF.2 connected to SO */
	Chip_SCU_PinMuxSet(0xF, 2, (SCU_PINIO_FAST | SCU_MODE_FUNC2));
	/* PF.3 connected to nSI */
	Chip_SCU_PinMuxSet(0xF, 3, (SCU_PINIO_FAST | SCU_MODE_FUNC2));

	/* DC PIN */
	Chip_SCU_PinMuxSet(0x0E, 8, (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4));
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 7, 8, true);
}

/* Write to a LCD register using SPI */
static void writeLCDReg(uint16_t addr, uint16_t data)
{
	uint8_t buf[2];

	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 0x06, 10, true);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 0x06, 11, false);

	LCD_DC_CMD;

	buf[0] = 0;
	buf[1] = (addr & 0xff);

	Chip_SSP_WriteFrames_Blocking(LPC_SSP0, buf, 2);

	LCD_DC_DATA;
	buf[0] = (data >> 8);
	buf[1] = (data & 0xff);
	Chip_SSP_WriteFrames_Blocking(LPC_SSP0, buf, 2);

	LCD_DC_CMD;

	buf[0] = (0);
	buf[1] = (0x22);
	Chip_SSP_WriteFrames_Blocking(LPC_SSP0, buf, 2);
}

/* Initialize SSD1289 LCD Controller */
static void initSSD1289(void)
{
	writeLCDReg(0x00, 0x0001);
	delayMs(15);
	writeLCDReg(0x03, 0x6E3E);	// 0xAEAC
	writeLCDReg(0x0C, 0x0007);
	writeLCDReg(0x0D, 0x000E);	// 0x000F
	writeLCDReg(0x0E, 0x2C00);	// 0x2900
	writeLCDReg(0x1E, 0x00AE);	// 0x00B3
	delayMs(15);
	writeLCDReg(0x07, 0x0021);
	delayMs(50);
	writeLCDReg(0x07, 0x0023);
	delayMs(50);
	writeLCDReg(0x07, 0x0033);
	delayMs(50);

	writeLCDReg(0x01, 0x2B3F);
	writeLCDReg(0x02, 0x0600);
	writeLCDReg(0x10, 0x0000);
	delayMs(15);
	writeLCDReg(0x11, 0xC5B0);	// 0x65b0
	delayMs(20);
	writeLCDReg(0x05, 0x0000);
	writeLCDReg(0x06, 0x0000);
	writeLCDReg(0x16, 0xEF1C);
	writeLCDReg(0x17, 0x0003);
	writeLCDReg(0x07, 0x0233);
	writeLCDReg(0x0B, 0x5312);
	writeLCDReg(0x0F, 0x0000);
	writeLCDReg(0x25, 0xE000);
	delayMs(20);
	writeLCDReg(0x41, 0x0000);
	writeLCDReg(0x42, 0x0000);
	writeLCDReg(0x48, 0x0000);
	writeLCDReg(0x49, 0x013F);
	writeLCDReg(0x44, 0xEF00);
	writeLCDReg(0x45, 0x0000);
	writeLCDReg(0x46, 0x013F);
	writeLCDReg(0x4A, 0x0000);
	writeLCDReg(0x4B, 0x0000);
	delayMs(20);
	writeLCDReg(0x30, 0x0707);
	writeLCDReg(0x31, 0x0704);
	writeLCDReg(0x32, 0x0005);	// 0x0204
	writeLCDReg(0x33, 0x0402);	// 0x0201
	writeLCDReg(0x34, 0x0203);
	writeLCDReg(0x35, 0x0204);
	writeLCDReg(0x36, 0x0204);
	writeLCDReg(0x37, 0x0401);	// 0x0502
	writeLCDReg(0x3A, 0x0302);
	writeLCDReg(0x3B, 0x0500);
	delayMs(20);
	writeLCDReg(0x22, 0x0000);
}

/* Send/Receive data to/from TSC2046. */
static void readWriteTSC2046(uint8_t command, uint16_t *data)
{
	uint8_t rx_data[2], tx_data[1] = {0x00};

	tx_data[0] = command;
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 6, 10, false);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 6, 11, true);

	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 7, 16, false);

	Chip_SSP_WriteFrames_Blocking(LPC_SSP0, tx_data, 1);
	Chip_SSP_ReadFrames_Blocking(LPC_SSP0, rx_data, 2);

#if (TSC2046_CONVERSION_BITS == 8)
	*data = (((rx_data[0] << 8) | (rx_data[1])) >> 7) & 0xFF;
#else
	*data = (((rx_data[0] << 8) | rx_data[1]) >> 3) & 0xFFF;
#endif
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 7, 16, true);
}

/* Evaluate the coordinates received from TSC2046. */
static Status evalTSC2046Coord(uint8_t command, uint16_t *coord)
{
	uint32_t i;
	uint16_t Tmp = 0, previousTmp;
	int16_t diff = 0;
	*coord = 0;
	for (i = 0; i < COORD_GET_NUM; i++) {
		previousTmp = Tmp;
		readWriteTSC2046(command, &Tmp);
		if (Tmp > TSC2046_COORD_MAX) {
			return ERROR;
		}
		if (i > 0) {
			diff = Tmp - previousTmp;
		}
		if (diff < 0) {
			diff = 0 - diff;
		}
		if (diff > TSC2046_DELTA_VARIANCE) {
			return ERROR;
		}
		*coord += Tmp;
	}
	*coord /= COORD_GET_NUM;
	return SUCCESS;
}

/* Convert the coord received from TSC to a value on truly LCD */
static int16_t calibrateTSC2046(int16_t Coord, int16_t MinVal, int16_t MaxVal, int16_t TrueSize)
{
	int16_t tmp;
	int16_t ret;
	uint8_t convert = 0;

	/* Swap value? */
	if (MinVal > MaxVal) {
		tmp = MaxVal;
		MaxVal = MinVal;
		MinVal = tmp;
		convert = 1;
	}

	ret = (Coord - MinVal) * TrueSize / (MaxVal - MinVal);
	if (convert) {
		ret = TrueSize - ret;
	}

	return ret;
}

static void delay(uint32_t i) {
	while (i--) {}
}

/* Initialize UDA1380 CODEC */
static Status Board_Audio_CodecInit(int micIn)
{
	/* Reset UDA1380 on board Hitex A4*/
	/* PE_9: UDA_RST on Hitex A4 */
	Chip_SCU_PinMuxSet(0x0E, 9, (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4));
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 7, 9, true);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 7, 9, true);
	/* delay 1us */
	delay(100000);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 7, 9, false);
	delay(100000);

	if (!UDA1380_Init(UDA1380_MIC_IN_LR & - (micIn != 0))) {
		return ERROR;
	}

	return SUCCESS;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize pin muxing for a UART */
void Board_UART_Init(LPC_USART_T *pUART)
{
	if (pUART == LPC_USART0) {
		Chip_SCU_PinMuxSet(0xF, 10, (SCU_MODE_MODE_REPEATER | SCU_MODE_FUNC1));	/* PF.10 : UART0_TXD */
		Chip_SCU_PinMuxSet(0xF, 11, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC1));	/* PF.11 : UART0_RXD */
	}
	else if (pUART == LPC_UART1) {
		Chip_SCU_PinMuxSet(0xC, 13, (SCU_MODE_MODE_REPEATER | SCU_MODE_FUNC2));	/* PC.13 : UART1_TXD - pin 1 of SV14 */
		Chip_SCU_PinMuxSet(0xC, 14, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2));	/* PC.14 : UART1_RX - pin 2 of SV14 */
	}
	else if (pUART == LPC_USART2) {
		/* P1.15 : UART2_TXD - pin 11 of SV6, P1.16 : UART2_RXD - pin 3 of SV6 */
		Chip_SCU_PinMuxSet(0x1, 15, (SCU_MODE_MODE_REPEATER | SCU_MODE_FUNC1));
		Chip_SCU_PinMuxSet(0x1, 16, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC1));
	}
	else if (pUART == LPC_USART3) {
		/* P9.3 : UART3_TXD - pin 15 of SV6, P9.4 : UART3_RXD - pin 7 of SV3 */
		Chip_SCU_PinMuxSet(0x9, 3, (SCU_MODE_MODE_REPEATER | SCU_MODE_FUNC7));
		Chip_SCU_PinMuxSet(0x9, 4, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC7));
	}
}

/* Initialize debug output via UART for board */
void Board_Debug_Init(void)
{
#if defined(DEBUG_UART)
	Board_UART_Init(DEBUG_UART);

	Chip_UART_Init(DEBUG_UART);
	Chip_UART_SetBaud(DEBUG_UART, 115200);
	Chip_UART_ConfigData(DEBUG_UART, UART_DATABIT_8, UART_PARITY_NONE, UART_STOPBIT_1);

	/* Enable UART Transmit */
	Chip_UART_TxCmd(DEBUG_UART, ENABLE);
#endif
}

/* Sends a character on the UART */
void Board_UARTPutChar(char ch)
{
#if defined(DEBUG_UART)
	while (Chip_UART_SendByte(DEBUG_UART, (uint8_t) ch) == ERROR) {}
#endif
}

/* Gets a character from the UART, returns EOF if no character is ready */
int Board_UARTGetChar(void)
{
#if defined(DEBUG_UART)
	uint8_t data;

	if (Chip_UART_ReceiveByte(DEBUG_UART, &data) == SUCCESS) {
		return (int) data;
	}
#endif
	return EOF;
}

/* Outputs a string on the debug UART */
void Board_UARTPutSTR(char *str)
{
#if defined(DEBUG_UART)
	while (*str != '\0') {
		Board_UARTPutChar(*str++);
	}
#endif
}

/* Initializes board LED(s) */
static void Board_LED_Init()
{
	int i;

	/* Set ports as outputs with initial states off */
	for (i = 0; i < (sizeof(ledports) / sizeof(ledports[0])); i++) {
		Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, ledports[i], ledbits[i], true);
		Chip_GPIO_WritePortBit(LPC_GPIO_PORT, ledports[i], ledbits[i], true);
	}
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, ledports[0], ledbits[0], false);
}

/* Sets the state of a board LED to on or off */
void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	/* Must connect JP3 to see LED0 and JP4 to see LED1 */
	if (LEDNumber == 0) {
		On = !On;
	}

	if (LEDNumber < 4) {
		Chip_GPIO_WritePortBit(LPC_GPIO_PORT, ledports[LEDNumber], ledbits[LEDNumber], !On);
	}
}

/* Returns the current state of a board LED */
bool Board_LED_Test(uint8_t LEDNumber)
{
	bool On = false;

	if (LEDNumber < 4) {
		On = (bool) !Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, ledports[LEDNumber], ledbits[LEDNumber]);

		if (LEDNumber == 0) {
			On = ~On;
		}
	}

	return On;
}

/* Initialize button(s) interface on board */
void Board_Buttons_Init(void)	// FIXME not functional ATM
{
	Chip_SCU_PinMuxSet(0xD, 7, (SCU_MODE_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC4));			// GPIO6[21]
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM, false);	// input
}

/* Returns button(s) state on board */
uint32_t Buttons_GetStatus(void)
{
	uint8_t ret = NO_BUTTON_PRESSED;
	if (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM) == 0) {
		ret |= BUTTONS_BUTTON1;
	}
	return ret;
}

/* Initialize joystick interface on board */
void Board_Joystick_Init(void)
{}

/* Returns joystick states on board */
uint8_t Joystick_GetStatus(void)
{
	return NO_BUTTON_PRESSED;
}

/**
 * System Clock Frequency (Core Clock)
 */
uint32_t SystemCoreClock;

/* Update system core clock rate, should be called if the system has
   a clock rate change */
void SystemCoreClockUpdate(void)
{
	/* CPU core speed */
	SystemCoreClock = Chip_Clock_GetRate(CLK_MX_MXCORE);
}

/* Returns the MAC address assigned to this board */
void Board_ENET_GetMacADDR(uint8_t *mcaddr)
{
	const uint8_t boardmac[] = {0x00, 0x60, 0x37, 0x12, 0x34, 0x56};

	memcpy(mcaddr, boardmac, 6);
}

/* Set up and initialize all required blocks and functions related to the
   board hardware */
void Board_Init(void)
{
	/* Sets up DEBUG UART */
	DEBUGINIT();

	/* Updates SystemCoreClock global var with current clock speed */
	SystemCoreClockUpdate();

	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);

	/* Setup GPIOs for USB demos */
#if 0	/* FIXME: the following call removed on the Hitex board as it interferes with muxed MII state */
		//	Chip_SCU_PinMuxSet(0x9, 5, (SCU_MODE_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC2));			// P9_5 USB1_VBUS_EN, USB1 VBus function
#endif
	Chip_SCU_PinMuxSet(0x2, 5, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2));	// P2_5 USB1_VBUS, MUST CONFIGURE THIS SIGNAL FOR USB1 NORMAL OPERATION
	Chip_SCU_PinMuxSet(0x6, 3, (SCU_MODE_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC1));			// P6_3 USB0_PWR_EN, USB0 VBus function

	/* Initialize LEDs */
	Board_LED_Init();
}

/* Sets up board specific ADC interface */
void Board_ADC_Init(void)
{
	/* Analog function ADC1_2 selected on pin PF_9 */
	Chip_SCU_ADC_Channel_Config(1, 2);
}

/* Sets up board specific I2C interface */
void Board_I2C_Init(I2C_ID_T id)
{
	if (id == I2C1) {
		/* Configure pin function for I2C1 on PE.13 (I2C1_SDA) and PE.15 (I2C1_SCL) */
		Chip_SCU_PinMuxSet(0xE, 13, (SCU_MODE_ZIF_DIS | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC2));
		Chip_SCU_PinMuxSet(0xE, 15, (SCU_MODE_ZIF_DIS | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC2));
	}
	else {
		Chip_SCU_I2C0PinConfig(I2C0_STANDARD_FAST_MODE);
	}
}

/* Initialize LCD Interface */
void Board_LCD_Init(void)
{
	/* Reset LCD and wait for reset to complete */
	Chip_RGU_TriggerReset(RGU_LCD_RST);
	while (Chip_RGU_InReset(RGU_LCD_RST)) {}

	lcdPinConfig();
	Board_InitLCDController();
}

/* Initialize the LCD controller on the external QVGA (320x240) TFT LCD*/
void Board_InitLCDController(void)
{
	SSP_ConfigFormat ssp_format;

	Chip_SSP_Init(LPC_SSP0);
	Chip_SSP_SetMaster(LPC_SSP0, true);
	Chip_SSP_SetBitRate(LPC_SSP0, 1000000);

	ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
	ssp_format.bits = SSP_BITS_8;
	ssp_format.clockMode = SSP_CLOCK_MODE0;

	Chip_SSP_SetFormat(LPC_SSP0, &ssp_format);
	Chip_SSP_Enable(LPC_SSP0);

	delayMs(200);

	/* initialize LCD controller */
	initSSD1289();

	Chip_SSP_Disable(LPC_SSP0);
	Chip_SSP_DeInit(LPC_SSP0);
}

/* Initialize TSC2046 touchscreen controller */
void Board_InitTouchController(void)
{
	uint16_t dummy_data;
	SSP_ConfigFormat ssp_format;

	/* Configure SSP0 pins*/
	lcdPinConfig();
	Chip_SCU_PinMuxSet(0xF, 1, (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4));

	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 7, 16, true);

	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 6, 10, false);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 6, 11, true);

	Chip_SSP_Init(LPC_SSP0);

	Chip_SSP_SetMaster(LPC_SSP0, true);
	Chip_SSP_SetBitRate(LPC_SSP0, 200000);

	ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
	ssp_format.bits = SSP_BITS_8;
	ssp_format.clockMode = SSP_CLOCK_MODE3;

	Chip_SSP_SetFormat(LPC_SSP0, &ssp_format);
	Chip_SSP_Enable(LPC_SSP0);

	/* Enable Touch Screen Controller */
	readWriteTSC2046(PWRDOWN, &dummy_data);
}

/* Get Touch coordinates */
bool Board_GetTouchPos(int16_t *pX, int16_t *pY)
{
	uint16_t tmp;
	uint16_t x, y, z1, z2, z = 0;
	Status Sts = SUCCESS;

	readWriteTSC2046(X_MEASURE, &x);
	readWriteTSC2046(Y_MEASURE, &y);
	readWriteTSC2046(Z1_MEASURE, &z1);
	readWriteTSC2046(Z2_MEASURE, &z2);

	if (z1 != 0) {
		z = x * ((z2 / z1) - 1);
	}
	if ((z <= 0) || (z > 35000)) {
		return false;
	}
	/* Get X-Coordinate */
	Sts = evalTSC2046Coord(X_MEASURE, &x);

	if (Sts == ERROR) {
		return false;
	}
	/* Get Y-Coordinate */
	Sts = evalTSC2046Coord(Y_MEASURE, &y);
	if (Sts == ERROR) {
		return false;
	}
	/* Get Z1-Value */
	Sts = evalTSC2046Coord(Z1_MEASURE, &z1);
	if (Sts == ERROR) {
		return false;
	}
	/* Get Z2-Value */
	Sts = evalTSC2046Coord(Z2_MEASURE, &z2);
	if (Sts == ERROR) {
		return false;
	}

	z = x * ((z2 / z1) - 1);
	if ((z <= 0) || (z > 35000)) {
		return false;
	}
	else {
		/* Swap, adjust to truly size of LCD */
		if (TSC_Config.swap_xy) {
			*pY = calibrateTSC2046(x, TSC_Config.ad_top, TSC_Config.ad_bottom, TSC_Config.lcd_height);
			*pX = calibrateTSC2046(y, TSC_Config.ad_left, TSC_Config.ad_right, TSC_Config.lcd_width);
		}
		else {
			*pX = calibrateTSC2046(x, TSC_Config.ad_top, TSC_Config.ad_bottom, TSC_Config.lcd_width);
			*pY = calibrateTSC2046(y, TSC_Config.ad_left, TSC_Config.ad_right, TSC_Config.lcd_height);
		}
	}
	readWriteTSC2046(PWRDOWN, &tmp);

	return true;
}

/* Turn on Board LCD Backlight */
void Board_SetLCDBacklight(uint8_t Intensity)
{
	bool OnOff = (bool) (Intensity != 0);

	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 3, 8, OnOff);
}

/* Initialize pin muxing for SDMMC interface */
void Board_SDMMC_Init(void)
{
	Chip_SCU_PinMuxSet(0xc, 9, (SCU_MODE_MODE_PULLUP | SCU_MODE_FUNC7));				/* Pc.9 SDIO power */
	Chip_SCU_PinMuxSet(0xc, 2, (SCU_MODE_MODE_PULLUP | SCU_MODE_FUNC7));				/* Pc.2 SDIO LED */
	Chip_SCU_PinMuxSet(0xf, 10, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC6));	/* Pf.10 SDIO WP */
	Chip_SCU_PinMuxSet(0xc, 8, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC7));	/* Pc.8 SDIO CD */
	Chip_SCU_PinMuxSet(0xc, 6, (SCU_PINIO_FAST | SCU_MODE_FUNC7));		/* Pc.6 SDIO D2 */
	Chip_SCU_PinMuxSet(0xc, 5, (SCU_PINIO_FAST | SCU_MODE_FUNC7));		/* Pc.5 SDIO D1 */
	Chip_SCU_PinMuxSet(0xc, 4, (SCU_PINIO_FAST | SCU_MODE_FUNC7));		/* Pc.4 SDIO D0 */
	Chip_SCU_PinMuxSet(0xc, 0, (SCU_MODE_MODE_PULLUP | SCU_MODE_HIGHSPEEDSLEW_EN | SCU_MODE_FUNC7));	/* Pc.0 SDIO clock */
	Chip_SCU_PinMuxSet(0xc, 10, (SCU_PINIO_FAST | SCU_MODE_FUNC7));		/* Pc.10 SDIO command */
	Chip_SCU_PinMuxSet(0xc, 7, (SCU_PINIO_FAST | SCU_MODE_FUNC7));		/* Pc.7 SDIO D3 */
}

/* Initialize pin muxing for SSP interface */
void Board_SSP_Init(LPC_SSP_T *pSSP)
{
	if (pSSP == LPC_SSP0) {
		/* Set up clock and muxing for SSP0 interface */
		// #if !defined(HITEX_LCD_TERM)
		/* PC.11 connected to GPIO = SSEL_MUX_A, PC.12 connected to GPIO = SSEL_MUX_B */
		Chip_SCU_PinMuxSet(0xC, 11, (SCU_MODE_MODE_PULLUP | SCU_MODE_FUNC4));
		Chip_SCU_PinMuxSet(0xC, 12, (SCU_MODE_MODE_PULLUP | SCU_MODE_FUNC4));
		Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 0x6, 10, true);
		Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 0x6, 11, true);
		// #endif
		/* PF.0 connected to SCL/SCLK	SCU_MODE_FUNC2=SSP0 SCK0 */
		Chip_SCU_PinMuxSet(0xF, 0, (SCU_PINIO_FAST | SCU_MODE_FUNC0));
		/* PF.1 connected to nCS		SCU_MODE_FUNC2=SSP0 SSEL0 */
		Chip_SCU_PinMuxSet(0xF, 1, (SCU_PINIO_FAST | SCU_MODE_FUNC2));
		/* PF.2 connected to SO			SCU_MODE_FUNC2=SSP0 MISO0 */
		Chip_SCU_PinMuxSet(0xF, 2, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2));
		/* PF.3 connected to nSI		SCU_MODE_FUNC2=SSP0 MOSI0 */
		Chip_SCU_PinMuxSet(0xF, 3, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2));
	}
	else if (pSSP == LPC_SSP1) {
		/* Set up clock and muxing for SSP1 interface */
		/* P1.19 connected to SCL/SCLK	SCU_MODE_FUNC1=SSP1 SCK1 */
		Chip_SCU_PinMuxSet(0x1, 19, (SCU_PINIO_FAST | SCU_MODE_FUNC1));
		/* P1.20 connected to nCS		SCU_MODE_FUNC1=SSP1 SSEL1 */
		Chip_SCU_PinMuxSet(0x1, 20, (SCU_PINIO_FAST | SCU_MODE_FUNC1));
		/* P0.0 connected to SO			SCU_MODE_FUNC1=SSP1 MISO1 */
		Chip_SCU_PinMuxSet(0x0, 0, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC1));
		/* P0.1 connected to nSI		SCU_MODE_FUNC2=SSP1 MOSI1 */
		Chip_SCU_PinMuxSet(0x0, 1, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC1));
	}
}

/* Initialize I2S interface for the board and UDA1380 */
void Board_Audio_Init(LPC_I2S_T *pI2S, int micIn)
{
	Chip_I2S_Audio_Format_T I2S_Config;

	I2S_Config.SampleRate = 48000;
	I2S_Config.ChannelNumber = 2;	// 1 is mono, 2 is stereo
	I2S_Config.WordWidth =  16;	// 8, 16 or 32 bits
	Chip_I2S_Init(pI2S);
	Chip_I2S_Config(pI2S, I2S_TX_MODE, &I2S_Config);

	/* Init UDA1380 CODEC */
	while (Board_Audio_CodecInit(micIn) != SUCCESS) {}

}

/* Initialize DAC interface for the board */
void Board_DAC_Init(LPC_DAC_T *pDAC)
{
	Chip_SCU_DAC_Analog_Config();
}

/* FIXME */
void Serial_CreateStream(void *Stream)
{
	// implement later
}

/**
 * @}
 */
