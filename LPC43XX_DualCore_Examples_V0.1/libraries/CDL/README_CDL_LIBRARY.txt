Common Driver Library (CDL) Library:
===================================
The CDL Library code consists of CMSIS code (CMSISv2p00_LPC43xx) & IP drivers (LPC43xxLib) for LPC43XX.
  a. CMSISv2p00_LPC43xx - The LPC43XX CMSIS code Version 2.00 is used. The CMSIS has start up code & System initialisation code for both Cortex-M4 & Cortex-M0 cores
  b. LPC43xxLib - This has the IP drivers for various LPC43XX peripherals like UART, Timer, EMC, etc     

CDL Library configurations:
==========================
The Keil project LPC43XX_CDL.uvproj is provided for building the CDL library. The LPC43XX_CDL.uvproj has two configurations
  1. LPC43xx_M4 - This project configuration will build the CDL library code for Cortex-M4 core in LPC43XX MCU
  2. LPC43xx_M0 - This project configuration will build the CDL libray code for Cortex-M0 core in LPC43XX MCU

Building the CDL Library:
=========================
1. Open the \applications\LPC43XX_Demo_Examples.uvmpw Keil workspace project. This Keil workspace project has all the Keil projects which can be built
2. Right click on "LPC43XX_CDL" project & set as default project to build
3. Choose the project configuration to be build from drop down menu (LPC43xx_M4 or LPC43xx_M0), from project menu in Keil GUI, select Build option to build
   - If LPC43xx_M4 is built, it will output CDL library file CDL_LPC43xx_M4.lib 
   - If LPC43xx_M0 is built, it will output CDL library file CDL_LPC43xx_M0.lib 

