NXP USBLIB Library:
===================
The NXP USBLIB code is built as a library which can be included in the applications. The NXP USBLIB library will include the following functionalities:
  a. Class drivers - This will have the USB class drivers for USB host & USB device (HID, Audio, CDC, Mass Storage, RNDIS, MIDI, Printer, StillImage)
  b. USB LPC43XX drivers - This has LPC43XX USB device driver 
  c. Core drivers - This has core USB drivers
  d. HCD & DCD drivers
  e. HAL layer - HAL layers for LPC43XX

The NXP USBLIB is tested on the following boards:
  a. HITEX LPC4350 A4 Evaluation board 
  b. Keil MCB4300 evaluation board

NXP USBLIB configurations:
==========================
The NXP USBLIB configuration file nxpUSBlibConfig.h is located in \libraries\nxpUSBLib directory
The user can modify the configuration options if required & build the NXP USBLIB library  
 
NXP USBLIB Library configurations:
==================================
The Keil project LPC43XX_nxpUSBlib.uvproj is provided for building the USBLIB library. The LPC43XX_nxpUSBlib.uvproj has the following configurations:
  1. LPC43xx_Host_M4 - This project configuration will build the USBLIB library code for Cortex-M4 core in USB Host configuration
  2. LPC43xx_Device_M4 - This project configuration will build the USBLIB library code for Cortex-M4 core in USB Device configuration
  3. LPC43xx_Host_M0 - This project configuration will build the USBLIB library code for Cortex-M0 core in USB Host configuration
  4. LPC43xx_Device_M0 - This project configuration will build the USBLIB library code for Cortex-M0 core in USB Device configuration

Building the NXP USBLIB library:
================================ 
1. Open the \applications\LPC43XX_Demo_Examples.uvmpw Keil workspace project. This Keil workspace project has all the Keil projects which can be built
2. Right click on "LPC43XX_nxpUSBlib" project & set as default project to build
3. Choose the project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - LPC43xx_Host_M4 - It will output USBLIB library file nxpUSBlib_LPC43xx_Host_M4.lib 
   - LPC43xx_Host_M0 - It will output USBLIB library file nxpUSBlib_LPC43xx_Host_M0.lib 
   - LPC43xx_Device_M4 - It will output USBLIB library file nxpUSBlib_LPC43xx_Device_M4.lib 
   - LPC43xx_Device_M0 - It will output USBLIB library file nxpUSBlib_LPC43xx_Device_M0.lib 

