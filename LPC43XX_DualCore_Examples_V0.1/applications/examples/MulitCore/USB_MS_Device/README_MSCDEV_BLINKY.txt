USB Mass Storage example:
=========================
The USB_MS_Device example demonstrates the USB Mass storage example using the NXPUSBLIB library. The USB Mass Storage functionality can be executed on Cortex-M4 or Cortex-M0 cores.
For example, USB Mass storage & Blinky example can be executed on M4 core & Blinky example can be executed on M0 core

USB Mass storage Configurations:
================================
The USB Mass storage example builds the application code with required libraries to build the executable.
The USB Mass storage example can be built by using the Example_MSDevice_Blinky.uvproj Keil project. This has the following configurations:
1. USB_noOS_M0 - This will build the demo with USB Mass storage & Blinky example in stand alone configuration for M0 core 
2. Blinky_noOS_M0 - This will build the demo with Blinky example in stand alone configuration for M0 core
3. USB_noOS_M4 - This will build the demo with USB Mass storage & Blinky example in stand alone configuration for M4 core 
4. Blinky_noOS_M4 - This will build the demo with Blinky example in stand alone configuration for M4 core 
5. USB_FreeRTOS_M0 - This will build the demo with USB Mass storage & Blinky example in FreeRTOS configuration for M0 core 
6. Blinky_FreeRTOS_M0 - This will build the demo with Blinky example in FreeRTOS configuration for M0 core
7. USB_FreeRTOS_M4 - This will build the demo with USB Mass storage & Blinky example in FreeRTOS configuration for M4 core 
8. Blinky_FreeRTOS_M4 - This will build the demo with Blinky example in FreeRTOS configuration for M4 core 

Builiding the demo:
===================
Initially M0 image has to be built which will be created into c source file. The c source file is built with the M4 image. 
The user can build the demo to have USB Mass storage running on either M4 core or M0 core.
1. Open the \applications\LPC43XX_Demo_Examples.uvmpw Keil workspace project. This Keil workspace project has all the Keil projects which can be built
2. Right click on "Example_MSDevice_Blinky" project & set as default project to build

The user can choose whether to have USB Mass storage running on M4 or M0 core.

M4 Running USB Mass storage & M0 core running Blinky in stand-alone configuration:
==================================================================================
3. Build the libraries required for the M4 & M0 applications by using the "Project->Batch Build" option in Keil GUI
   Blinky_noOS_M0 - CDL_LPC43xx_M0.lib, BSP_LPC43XX_M0.lib, IPC_noOS_m0.lib
   USB_noOS_M4 - CDL_LPC43xx_M4.lib, BSP_LPC43XX_M4.lib, IPC_noOS_m4.lib, nxpUSBlib_LPC43xx_Device_M4.lib 
4. Choose M0 Blinky project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - Blinky_noOS_M0 - It will output m0_image.c file which is included with M4 image 
5. Choose M4 USB project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - USB_noOS_M4 - It will output USBMSDevice_noOS_M4.axf executable 

M0 Running USB Mass storage & M4 core running Blinky in stand-alone configuration:
===================================================================================
3. Build the libraries required for the M4 & M0 applications by using the "Project->Batch Build" option in Keil GUI
   USB_noOS_M0 - CDL_LPC43xx_M0.lib, BSP_LPC43XX_M0.lib, IPC_noOS_m0.lib, nxpUSBlib_LPC43xx_Device_M0.lib 
   Blinky_noOS_M4 - CDL_LPC43xx_M4.lib, BSP_LPC43XX_M4.lib, IPC_noOS_m4.lib
4. Choose M0 USB project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - USB_noOS_M0 - It will output m0_image_usb.c file which is included with M4 image 
5. Choose M4 Blinky project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - Blinky_noOS_M4 - It will output Blinky_noOS_M4.axf executable 

M4 Running USB Mass storage & M0 core running Blinky in FreeRTOS configuration:
===============================================================================
3. Build the libraries required for the M4 & M0 applications by using the "Project->Batch Build" option in Keil GUI
   Blinky_FreeRTOS_M0 - CDL_LPC43xx_M0.lib, BSP_LPC43XX_M0.lib, IPC_noOS_m0.lib, FreeRTOS_LPC43XX_M0.lib
   USB_FreeRTOS_M4 - CDL_LPC43xx_M4.lib, BSP_LPC43XX_M4.lib, IPC_noOS_m4.lib, nxpUSBlib_LPC43xx_Device_M4.lib, FreeRTOS_LPC43XX_M4.lib  
4. Choose M0 Blinky project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - Blinky_FreeRTOS_M0 - It will output m0_image.c file which is included with M4 image 
5. Choose M4 USB project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - USB_FreeRTOS_M4 - It will output USBMSDevice_FreeRTOS_M4.axf executable  

M0 Running USB Mass storage & M4 core running Blinky in FreeRTOS configuration:
===============================================================================
3. Build the libraries required for the M4 & M0 applications by using the "Project->Batch Build" option in Keil GUI
   USB_FreeRTOS_M0 - CDL_LPC43xx_M0.lib, BSP_LPC43XX_M0.lib, IPC_noOS_m0.lib, nxpUSBlib_LPC43xx_Device_M0.lib, FreeRTOS_LPC43XX_M0.lib  
   Blinky_FreeRTOS_M4 - CDL_LPC43xx_M4.lib, BSP_LPC43XX_M4.lib, IPC_noOS_m4.lib, FreeRTOS_LPC43XX_M4.lib
4. Choose M0 USB project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - USB_FreeRTOS_M0 - It will output m0_image_usb.c file which is included with M4 image 
5. Choose M4 Blinky project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - Blinky_FreeRTOS_M4 - It will output Blinky_FreeRTOS_M4.axf executable  

Linker scripts:
===============
Flash configuration:
====================
By default, the Blinky application is built for flash. It uses the \applications\examples\MultiCore\common\lpc43xx_flash.sct scatter file.
The scatter file will check for lpc43xx_iflash variable in the code. 
If lpc43xx_iflash variable defined in the code, then  application will be built for the Internal Flash.
If lpc43xx_iflash variable not defined, then the application will be built for external NOR Flash.
  - For MCB4300 board, the application built for internal flash configuration (lpc43xx_iflash defined in BSP library code)
  - For Hitex4350 A4 board, the application built for extrenal NOR flash configuration

IRAM configuration:
===================
Please use the scatter files (inram_usb_m4.sct & inram_usb_m0.sct) provided in \applications\examples\MultiCore\common\linker directory to build the application for IRAM configurations

NOTE: MAKE SURE THAT YOU ARE BUILDING BOTH M0 & M4 IMAGES FOR THE SAME CONFIGURATION

Demo set up:
============
Connect the UART cable from board to PC. Open the TeraTerm or COM port with 115200 baud, 8 data bits, 1 stop bit, no parity & no flow control.

Demo output:
===========
Once the demo code is run on the board.
 - On MCB4300 board, the LEDS connected to LED1 (PD_10) & LED2 (PD_11) (Next to CAN connector) will starts blinking 
 - On Hitex4350 A4 board, the LED will blink with red & green colors
 - The USB Mass storage device should be enumerated on host PC. The enumerated disk should be accessible (read, write) from host PC. 

