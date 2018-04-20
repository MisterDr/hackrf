FreeRTOS Library:
=================
The FreeRTOS code is built as a library which can be included in the applications. The following FreeRTOS functionalities are included in the FreeRTOS library:
  a. FreeRTOS ports for Cortex-M4 & Cortex-M0 cores - This has Tick functionality, stack initialisation, Context switching functions. The Cortex-M0 port uses the Repetetive Interrupt timer for tick functionality    
  b. Queue & lists functions - This has FreeRTOS Queues & lists functionality
  c. Task management functions - This has FreeRTOS task management functions 
  d. Timer functions - This has FreeRTOS timer management functions
  e. Heap management functions - This has FreeRTOS heap3 management functionality
  f. FreeRTOS hook functions - This has FreeRTOS hook functions for Idle, Malloc failure & stack overflows
 
The FreeRTOS port is tested on the following boards:
  a. HITEX LPC4350 A4 Evaluation board - This has LPC4350 MCU
  b. Keil MCB4300 evaluation board  - This has LPC4357 MCU

FreeRTOS configuration:
======================
The FreeRTOS configuration files are located in \libraries\FreeRTOS\config directory
  - lpc43xx_m0_FreeRTOSConfig.h - This is the FreeRTOS configuration file for Cortex-M0 core
  - lpc43xx_m4_FreeRTOSConfig.h - This is the FreeRTOS configuration file for Cortex-M4 core
The user can modify the FreeRTOS configurations if required & build the FreeRTOs library  
 
FreeRTOS Library configurations:
================================
The Keil project LPC43XX_FreeRTOS.uvproj is provided for building the FreeRTOS library. The LPC43XX_FreeRTOS.uvproj has the following configurations:
  1. LPC43XX_M4 - This project configuration will build the FreeRTOS library code for Cortex-M4 core
  2. LPC43XX_M0 - This project configuration will build the FreeRTOS library code for Cortex-M0 core

Building the FreeRTOS library:
============================== 
1. Open the \applications\LPC43XX_Demo_Examples.uvmpw Keil workspace project. This Keil workspace project has all the Keil projects which can be built
2. Right click on "LPC43XX_FreeRTOS" project & set as default project to build
3. Choose the project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - LPC43XX_M4 - It will output FreeRTOS library file FreeRTOS_LPC43XX_M4.lib 
   - LPC43XX_M0 - It will output FreeRTOS library file FreeRTOS_LPC43XX_M0.lib 

