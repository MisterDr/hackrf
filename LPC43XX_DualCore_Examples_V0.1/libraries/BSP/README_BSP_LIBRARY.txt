Board Support Package (BSP) Library:
===================================
The BSP Library code consists of Board Support functions. It provides the configuration information & functions for controlling the on-board peripherals. 
The following functionalities are exported in BSP library:
  a. Board initialisation functions (bsp_init & bsp_misc_init) - These functions will initialise the CGU & SDRAM    
  b. Timer functions - This provide Timer initialisation & deinitialisation functions
  c. Serial interface functions - This has functions for controlling Serial interfaces (UART) 
  d. Button interface functions - This has functions for controlling the on-board buttons
  e. LED interface functions - This has functions for controlling the on-board LEDs
  f. Joystick functions - This has functions for controlling Joystick interface
  g. Audio functions  - This has Audio interface functions
  h. Mass storage functions - This has mass storage APIs
  i. Ethernet functions - This has Ethernet interface functions
  h. LCD functions - This has LCD controller functions
 
The BSP Library code is tested on the following boards:
  a. HITEX LPC4350 A4 Evaluation board - This has LPC4350 MCU
  b. Keil MCB4300 evaluation board  - This has LPC4357 MCU

BSP Library configurations:
==========================
The Keil project LPC43XX_BSP.uvproj is provided for building the BSP library. The LPC43XX_BSP.uvproj has the following configurations:
  1. HITEX4350_M4 - This project configuration will build the BSP library code for Hitex LPC4350 board to run on Cortex-M4 core
  2. HITEX4350_M0 - This project configuration will build the BSP library code for Hitex LPC4350 board to run on Cortex-M0 core
  3. MCB4300_M4 - This project configuration will build the BSP library code for MCB4300 board to run on Cortex-M4 core
  4. MCB4300_M4 - This project configuration will build the BSP library code for MCB4300 board to run on Cortex-M0 core

Building the BSP Library:
========================= 
1. Open the \applications\LPC43XX_Demo_Examples.uvmpw Keil workspace project. This Keil workspace project has all the Keil projects which can be built
2. Right click on "LPC43XX_BSP" project & set as default project to build
3. Choose the project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - HITEX4350_M4 - It will output BSP library file BSP_LPC43XX_M4.lib 
   - HITEX4350_M0 - It will output BSP library file BSP_LPC43XX_M0.lib 
   - MCB4300_M4 - It will output BSP library file BSP_LPC43XX_M4.lib 
   - MCB4300_M0 - It will output BSP library file BSP_LPC43XX_M0.lib

Adding support for new board in BSP library:
============================================
1. Create a directory \libraries\BSP\BOARD_X directory which will have board specific files 
2. The BSP interface functions need to be implemented in BSP specific files (For example, \libraries\BSP\BOARD_X\bsp_board_x.c)
3. Create the new project targets for BOARD_X_M4 & BOARD_X_M0 in Keil LPC43XX_BSP.uvproj project
4. Make sure that the other BSP files are not included in the build in this project configurations   

