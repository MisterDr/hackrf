Inter Processor Communication Library:
======================================
The Inter Processor Communication code to communicate between Cortex-M4 & Cortex-M0 cores is provided as library. The IPC library includes the following functions:
  a. Queues & Qeuue functions for IPC communication between Cortex-M4 & Cortex-M0 cores
  b. IPC Interrupt handlers for Cortex-M4 & Cortex-M0 cores
  c. IPC Boot function to start Cortex-M0 core from Cortex-M4 core

NOTE: For more information on the IPC library, please refer to IPC Application Note  
 
The IPC library is tested on the following boards:
  a. HITEX LPC4350 A4 Evaluation board  
  b. Keil MCB4300 evaluation board 

IPC configurations:
====================
For more information on the IPC configuration options, please refer to IPC Application Note  
 
IPC Library configurations:
============================
The Keil project LPC43XX_IPC_m4m0.uvproj is provided for building the IPC library. The LPC43XX_IPC_m4m0.uvproj has the following configurations:
  1. Standalone_M4 - This project configuration will build the IPC library code for Cortex-M4 core in Stand-alone configuration (without OS)
  2. Standalone_M0 - This project configuration will build the IPC library code for Cortex-M0 core in Stand-alone configuration (without OS)
  3. FreeRTOS_M4 - This project configuration will build the IPC library code for Cortex-M4 core in FreeRTOS configuration
  4. FreeRTOS_M0 - This project configuration will build the IPC library code for Cortex-M0 core in FreeRTOS configuration
  5. uCOSIII_M4 - This project configuration will build the IPC library code for Cortex-M4 core in uCOS-III configuration
  6. uCOSIII_M0 - This project configuration will build the IPC library code for Cortex-M0 core in uCOS-III configuration

Building the IPC library:
========================= 
1. Open the \applications\LPC43XX_Demo_Examples.uvmpw Keil workspace project. This Keil workspace project has all the Keil projects which can be built
2. Right click on "LPC43xx_IPC_m4m0" project & set as default project to build
3. Choose the project configuration to be build from drop down menu, from "project" menu in Keil GUI, click on "Build" option to build
   - Standalone_M4 - It will output IPC library file IPC_noOS_m4.lib 
   - Standalone_M0 - It will output IPC library file IPC_noOS_m0.lib 
   - FreeRTOS_M4 - It will output IPC library file IPC_FreeRTOS_m4.lib 
   - FreeRTOS_M0 - It will output IPC library file IPC_FreeRTOS_m0.lib 
   - uCOSIII_M4 - It will output IPC library file IPC_uCOSIII_m4.lib 
   - uCOSIII_M0 - It will output IPC library file IPC_UCOSIII_m0.lib 

