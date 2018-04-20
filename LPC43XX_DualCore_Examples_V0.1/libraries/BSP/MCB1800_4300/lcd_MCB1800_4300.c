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
#ifndef   VRAM_ADDR
  #define VRAM_ADDR 0 
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif
#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens and multiple buffers are not allowed!
#endif

#if ((DISPLAY_ORIENTATION & GUI_SWAP_XY) != 0)
#define LANDSCAPE   1
#else
#define LANDSCAPE   0
#endif

#if (LANDSCAPE == 1)
#define WIDTH       YSIZE_PHYS  /* Screen Width (in pixels)         */
#define HEIGHT      XSIZE_PHYS  /* Screen Hight (in pixels)         */
#else
#define WIDTH       XSIZE_PHYS  /* Screen Width (in pixels)         */
#define HEIGHT      YSIZE_PHYS  /* Screen Hight (in pixels)         */
#endif

#if ((DISPLAY_ORIENTATION & GUI_SWAP_XY) != 0)
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_X) != 0)
    #define TOUCH_TOP    TOUCH_X_MAX
    #define TOUCH_BOTTOM TOUCH_X_MIN
  #else
    #define TOUCH_TOP    TOUCH_X_MIN
    #define TOUCH_BOTTOM TOUCH_X_MAX
  #endif
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_Y) != 0)
    #define TOUCH_LEFT   TOUCH_Y_MAX
    #define TOUCH_RIGHT  TOUCH_Y_MIN
  #else
    #define TOUCH_LEFT   TOUCH_Y_MIN
    #define TOUCH_RIGHT  TOUCH_Y_MAX
  #endif
#else
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_X) != 0)
    #define TOUCH_LEFT   TOUCH_X_MAX
    #define TOUCH_RIGHT  TOUCH_X_MIN
  #else
    #define TOUCH_LEFT   TOUCH_X_MIN
    #define TOUCH_RIGHT  TOUCH_X_MAX
  #endif
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_Y) != 0)
    #define TOUCH_TOP    TOUCH_Y_MAX
    #define TOUCH_BOTTOM TOUCH_Y_MIN
  #else
    #define TOUCH_TOP    TOUCH_Y_MIN
    #define TOUCH_BOTTOM TOUCH_Y_MAX
  #endif
#endif

/* SSP Interface: SSP0
   SSP PINS: 
   - SSP_CS  = PF_1 (GPIO7[16])
   - SSP_SCK = PF_0 (SCK0)
   - SSP_SDO = PF_2 (MISO0)
   - SSP_SDI = PF_3 (MOSI0)
*/

#define SSP_PIN_CS0   (1 << 16)

/* SSP Status register - bit definitions */
#define SSP_TFE         0x01
#define SSP_RNE         0x04
#define SSP_BSY         0x10

/* SCU pin configuration definitions */
#define SPI_PIN_SET ((1 << 7) | (1 << 6) | (1 << 5))
#define LCD_PIN_SET ((1 << 7) | (1 << 6) | (1 << 5))
#define LCD_NPR_SET ((1 << 5) | (1 << 4))

#define SSP_START   (0x70)           /* Start byte for SPI transfer        */
#define SSP_RD      (0x01)           /* WR bit 1 within start              */
#define SSP_WR      (0x00)           /* WR bit 0 within start              */
#define SSP_DATA    (0x02)           /* RS bit 1 within start byte         */
#define SSP_INDEX   (0x00)           /* RS bit 0 within start byte         */

/* LCD SSP chip select/deselect */
#define LCD_SSP_CS(x) ((x) ? (LPC_GPIO_PORT->SET[7] = SSP_PIN_CS0) : (LPC_GPIO_PORT->CLR[7] = SSP_PIN_CS0))

/* SSP Transfer function */
static __inline uint8_t SSP_Tfr(uint8_t byte)
{
  LPC_SSP0->DR = byte;
  while (!(LPC_SSP0->SR & SSP_RNE));   /* Wait for send to finish */
  return (LPC_SSP0->DR);
}

/* SSP Send command */
static __inline void ssp_wr_cmd (uint8_t cmd)
{
  LCD_SSP_CS(0);
  SSP_Tfr(SSP_START | SSP_WR | SSP_INDEX);   
  SSP_Tfr(0);
  SSP_Tfr(cmd);
  LCD_SSP_CS(1);
}

/* SSP data send */
static __inline void ssp_wr_dat (uint16_t dat)
{
  LCD_SSP_CS(0);
  SSP_Tfr(SSP_START | SSP_WR | SSP_DATA);    
  SSP_Tfr((dat >>   8));                     
  SSP_Tfr((dat & 0xFF));                     
  LCD_SSP_CS(1);
}

/* LCD register write */
static __inline void lcd_wr_reg (uint8_t reg, uint16_t val)
{
  ssp_wr_cmd(reg);
  ssp_wr_dat(val);
}

static void SSP0_Init(void)
{
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_SSP0);
	
	/* Configure SSP0 peripheral -----------------------------------------------*/
  LPC_SSP0->CR0  = (1<<7)|(1<<6)|0x7;   /* 8-bit transfer CPHA = 1, CPOL = 1  */
  LPC_SSP0->CPSR = 0x08;                /* SPI Clock = PCLK / (8 + 1)         */
  LPC_SSP0->CR1  = (0<<2)|(1<<1);       /* Enable SSP controller, master mode */

  /* Configure SPI peripheral pins -------------------------------------------*/
	scu_pinmux(0xF, 0, SPI_PIN_SET, FUNC0);	/* PF_0: SCL (SSP0 pin)             */
 	scu_pinmux(0xF, 1, SPI_PIN_SET, FUNC4);	/* PF_1: CS  (GPIO7[16] pin)        */
 	scu_pinmux(0xF, 2, SPI_PIN_SET, FUNC2);	/* PF_2: SDO (SSP0 pin)             */
 	scu_pinmux(0xF, 3, SPI_PIN_SET, FUNC2);	/* PF_3: SDI (SSP0 pin)             */

  LPC_GPIO_PORT->DIR[7] |= SSP_PIN_CS0; /* Chip select pin is GPIO output     */
	
	return;
}

static void LCD_X_Init(void)
{
  int  i;

  CGU_SetDIV(CGU_CLKSRC_IDIVE, 33);
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_CLKSRC_IDIVE);
	CGU_EntityConnect(CGU_CLKSRC_IDIVE,CGU_BASE_LCD);

	/* Initialise SSP0 */
	SSP0_Init();
	
  /* Configure LCD controller pins ------------------------------*/
	scu_pinmux(0x04, 1, LCD_PIN_SET, FUNC5); /* P4_1:  LCD_VD19    */
	scu_pinmux(0x04, 2, LCD_PIN_SET, FUNC2); /* P4_2:  LCD_VD3     */
	scu_pinmux(0x04, 5, LCD_PIN_SET, FUNC2); /* P4_5:  LCD_VSYNC   */
  scu_pinmux(0x04, 6, LCD_PIN_SET, FUNC2); /* P4_6:  LCD_EN      */
  scu_pinmux(0x04, 7, LCD_PIN_SET, FUNC0); /* P4_7:  LCD_DOTCLK  */
  scu_pinmux(0x04, 9, LCD_PIN_SET, FUNC2); /* P4_9:  LCD_VD11    */
  scu_pinmux(0x04, 10, LCD_PIN_SET, FUNC2);/* P4_10: LCD_VD10    */

  scu_pinmux(0x07, 0, LCD_PIN_SET, FUNC0); /* P7_0:  LCD_BL_EN   */
  scu_pinmux(0x07, 6, LCD_PIN_SET, FUNC3); /* P7_6:  LCD_HSYNC   */

  scu_pinmux(0x08, 3, LCD_PIN_SET, FUNC3); /* P8_3: LCD_VD12     */
  scu_pinmux(0x08, 4, LCD_PIN_SET, FUNC3); /* P8_4: LCD_VD7      */
  scu_pinmux(0x08, 5, LCD_PIN_SET, FUNC3); /* P8_5: LCD_VD6      */
  scu_pinmux(0x08, 6, LCD_PIN_SET, FUNC3); /* P8_6: LCD_VD5      */
  scu_pinmux(0x08, 7, LCD_PIN_SET, FUNC3); /* P8_7: LCD_VD4      */

  scu_pinmux(0x0B, 0, LCD_PIN_SET, FUNC2); /* PB_0: LCD_VD23     */
  scu_pinmux(0x0B, 1, LCD_PIN_SET, FUNC2); /* PB_1: LCD_VD22     */
  scu_pinmux(0x0B, 2, LCD_PIN_SET, FUNC2); /* PB_2: LCD_VD21     */
  scu_pinmux(0x0B, 3, LCD_PIN_SET, FUNC2); /* PB_3: LCD_VD20     */
  scu_pinmux(0x0B, 4, LCD_PIN_SET, FUNC2); /* PB_4: LCD_VD15     */
  scu_pinmux(0x0B, 5, LCD_PIN_SET, FUNC2); /* PB_5: LCD_VD14     */
  scu_pinmux(0x0B, 6, LCD_PIN_SET, FUNC2); /* PB_6: LCD_VD13     */	


  /* Configure LCD controller ------------------------------------------------*/
  LPC_RGU->RESET_CTRL0 = (1U << 16);
  while ((LPC_RGU->RESET_ACTIVE_STATUS0 & (1U << 16)) == 0);

  LPC_LCD->CTRL &= ~(1 << 0);           /* Disable LCD                        */ 
  LPC_LCD->INTMSK = 0;                  /* Disable all interrupts             */

  LPC_LCD->UPBASE = VRAM_ADDR;

  LPC_LCD->TIMH  = (7    << 24) |       /* Horizontal back porch              */
                   (3    << 16) |       /* Horizontal front porch             */
                   (3    <<  8) |       /* Horizontal sync pulse width        */
                   (14   <<  2) ;       /* Pixels-per-line                    */
  LPC_LCD->TIMV  = (3    << 24) |       /* Vertical back porch                */
                   (2    << 16) |       /* Vertical front porch               */
                   (3    << 10) |       /* Vertical sync pulse width          */
                   (319  <<  0) ;       /* Lines per panel                    */
  LPC_LCD->POL   = (1    << 26) |       /* Bypass pixel clock divider         */
                   (239  << 16) |       /* Clocks per line: num of LCDCLKs    */
                   (1    << 13) |       /* Invert panel clock                 */
                   (1    << 12) |       /* Invert HSYNC                       */
                   (1    << 11) ;       /* Invert VSYNC                       */  
  LPC_LCD->LE    = (1    << 16) |       /* LCDLE Enabled: 1, Disabled: 0      */
                   (9    <<  0) ;       /* Line-end delay: LCDCLK clocks - 1  */
  LPC_LCD->CTRL  = (1    << 11) |       /* LCD Power Enable                   */
                   (1    <<  5) |       /* 0 = STN display, 1: TFT display    */
                   (6    <<  1) ;       /* Bits per pixel: 16bpp (5:6:5)      */
  
  for (i = 0; i < 256; i++)
	{
    LPC_LCD->PAL[i] = 0;                /* Clear palette                      */
  }
	
  LPC_LCD->CTRL |= (1 <<  0);           /* LCD enable                         */

  /* Turn on backlight -------------------------------------------------------*/
  LPC_GPIO_PORT->DIR[3] |= (1 << 8);
}

static void delay (int cnt)
{
  cnt <<= 15;
  while (cnt--);
}

/*
 * LCD APIs
 */
void LCD_Controller_Init(void)
{
   LCD_X_Init();

  /* LCD with HX8347-D LCD Controller                                         */

  /* Driving ability settings ------------------------------------------------*/
  lcd_wr_reg(0xEA, 0x00);                   /* Power control internal used (1)    */
  lcd_wr_reg(0xEB, 0x20);                   /* Power control internal used (2)    */
  lcd_wr_reg(0xEC, 0x0C);                   /* Source control internal used (1)   */
  lcd_wr_reg(0xED, 0xC7);                   /* Source control internal used (2)   */
  lcd_wr_reg(0xE8, 0x38);                   /* Source output period Normal mode   */
  lcd_wr_reg(0xE9, 0x10);                   /* Source output period Idle mode     */
  lcd_wr_reg(0xF1, 0x01);                   /* RGB 18-bit interface ;0x0110       */
  lcd_wr_reg(0xF2, 0x10);

  /* Adjust the Gamma Curve --------------------------------------------------*/
  lcd_wr_reg(0x40, 0x01);
  lcd_wr_reg(0x41, 0x00);
  lcd_wr_reg(0x42, 0x00);
  lcd_wr_reg(0x43, 0x10);
  lcd_wr_reg(0x44, 0x0E);
  lcd_wr_reg(0x45, 0x24);
  lcd_wr_reg(0x46, 0x04);
  lcd_wr_reg(0x47, 0x50);
  lcd_wr_reg(0x48, 0x02);
  lcd_wr_reg(0x49, 0x13);
  lcd_wr_reg(0x4A, 0x19);
  lcd_wr_reg(0x4B, 0x19);
  lcd_wr_reg(0x4C, 0x16);

  lcd_wr_reg(0x50, 0x1B);
  lcd_wr_reg(0x51, 0x31);
  lcd_wr_reg(0x52, 0x2F);
  lcd_wr_reg(0x53, 0x3F);
  lcd_wr_reg(0x54, 0x3F);
  lcd_wr_reg(0x55, 0x3E);
  lcd_wr_reg(0x56, 0x2F);
  lcd_wr_reg(0x57, 0x7B);
  lcd_wr_reg(0x58, 0x09);
  lcd_wr_reg(0x59, 0x06);
  lcd_wr_reg(0x5A, 0x06);
  lcd_wr_reg(0x5B, 0x0C);
  lcd_wr_reg(0x5C, 0x1D);
  lcd_wr_reg(0x5D, 0xCC);

  /* Power voltage setting ---------------------------------------------------*/
  lcd_wr_reg(0x1B, 0x1B);
  lcd_wr_reg(0x1A, 0x01);
  lcd_wr_reg(0x24, 0x2F);
  lcd_wr_reg(0x25, 0x57);
  lcd_wr_reg(0x23, 0x88);

  /* Power on setting --------------------------------------------------------*/
  lcd_wr_reg(0x18, 0x36);                   /* Internal oscillator frequency adj  */
  lcd_wr_reg(0x19, 0x01);                   /* Enable internal oscillator         */
  lcd_wr_reg(0x01, 0x00);                   /* Normal mode, no scrool             */
  lcd_wr_reg(0x1F, 0x88);                   /* Power control 6 - DDVDH Off        */
  delay(200);
  lcd_wr_reg(0x1F, 0x82);                   /* Power control 6 - Step-up: 3 x VCI */
  delay(50);                 
  lcd_wr_reg(0x1F, 0x92);                   /* Power control 6 - Step-up: On      */
  delay(50);
  lcd_wr_reg(0x1F, 0xD2);                   /* Power control 6 - VCOML active     */
  delay(50);

  /* Color selection ---------------------------------------------------------*/
  lcd_wr_reg(0x17, 0x55);                   /* RGB, System interface: 16 Bit/Pixel*/
  lcd_wr_reg(0x00, 0x00);                   /* Scrolling off, no standby          */

  /* Interface config --------------------------------------------------------*/
  lcd_wr_reg(0x2F, 0x11);                   /* LCD Drive: 1-line inversion        */
  lcd_wr_reg(0x31, 0x02);                   /* Value for SPI: 0x00, RBG: 0x02     */
  lcd_wr_reg(0x32, 0x00);                   /* DPL=0, HSPL=0, VSPL=0, EPL=0       */

  /* Display on setting ------------------------------------------------------*/
  lcd_wr_reg(0x28, 0x38);                   /* PT(0,0) active, VGL/VGL            */
  delay(200);
  lcd_wr_reg(0x28, 0x3C);                   /* Display active, VGL/VGL            */

  lcd_wr_reg(0x16, 0x00);                   /* Mem Access Control (MX/Y/V/L,BGR)  */

  /* Display scrolling settings ----------------------------------------------*/
  lcd_wr_reg(0x0E, 0x00);                   /* TFA MSB                            */
  lcd_wr_reg(0x0F, 0x00);                   /* TFA LSB                            */
  lcd_wr_reg(0x10, 320 >> 8);               /* VSA MSB                            */
  lcd_wr_reg(0x11, 320 &  0xFF);            /* VSA LSB                            */
  lcd_wr_reg(0x12, 0x00);                   /* BFA MSB                            */
  lcd_wr_reg(0x13, 0x00);                   /* BFA LSB                            */
}

void LCD_Backlight_on(int Turnon)
{
	if(Turnon)
	{
		LPC_GPIO_PORT->SET[3]  = (1 << 8);
	}
	else
	{
		LPC_GPIO_PORT->CLR[3]  = (1 << 8);
	}
}

/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*   
*/
void LCD_X_Config(void)
{
  //
  // At first initialize use of multiple buffers on demand
  //
  #if (NUM_BUFFERS > 1)
    GUI_MULTIBUF_Config(NUM_BUFFERS);
  #endif
  //
  // Set display driver and color conversion for 1st layer
  //
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  if (LCD_GetSwapXY())
	{
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
  }
	else
	{
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
  }
  LCD_SetVRAMAddrEx(0, (void *)VRAM_ADDR);
	
	#if (USE_TOUCH == 1)
    //
    // Set orientation of touch screen
    //
    GUI_TOUCH_SetOrientation(DISPLAY_ORIENTATION);
    //
    // Calibrate touch screen
    //
    GUI_TOUCH_Calibrate(GUI_COORD_X, 0, WIDTH  - 1, TOUCH_LEFT, TOUCH_RIGHT);
    GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, HEIGHT - 1, TOUCH_TOP,  TOUCH_BOTTOM);
  #endif

	//
	// Set user palette data (only required if no fixed palette is used)
  //
  #if defined(PALETTE)
    LCD_SetLUTEx(0, PALETTE);
  #endif
}

static void _SetVRAMAddr(void * pVRAM)
{
	return;
}

static void _SetOrg(int xPos, int yPos)
{
	return;
}

static void _SetLUTEntry(LCD_COLOR Color, U8 Pos)
{
	return;
}


int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData)
{
  int r;

  switch (Cmd) {
  case LCD_X_INITCONTROLLER: 
	{
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    // ...
    return 0;
  }
  case LCD_X_SETVRAMADDR:
	{
    //
    // Required for setting the address of the video RAM for drivers
    // with memory mapped video RAM which is passed in the 'pVRAM' element of p
    //
    LCD_X_SETVRAMADDR_INFO * p;
    p = (LCD_X_SETVRAMADDR_INFO *)pData;
		_SetVRAMAddr(p->pVRAM);
    return 0;
  }
  case LCD_X_SETORG:
	{
    //
    // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
    //
    LCD_X_SETORG_INFO * p;
    p = (LCD_X_SETORG_INFO *)pData;
		_SetOrg(p->xPos, p->yPos);
    return 0;
  }
  case LCD_X_SETLUTENTRY:
	{
    //
    // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
    //
    LCD_X_SETLUTENTRY_INFO * p;
    p = (LCD_X_SETLUTENTRY_INFO *)pData;
    _SetLUTEntry(p->Color, p->Pos);
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}
#endif
