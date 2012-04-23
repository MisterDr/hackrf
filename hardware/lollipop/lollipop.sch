EESchema Schematic File Version 2  date Sun Apr 22 22:13:19 2012
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:hackrf
EELAYER 43  0
EELAYER END
$Descr User 17000 11000
encoding utf-8
Sheet 1 1
Title ""
Date "23 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L BALUN T?
U 1 1 4F94D6C4
P 7600 6300
F 0 "T?" H 7600 6550 70  0000 C CNN
F 1 "BALUN" H 7600 6000 70  0000 C CNN
F 4 "Pulse Electronics" H 7600 6300 60  0001 C CNN "Manufacturer"
F 5 "CX2156NL" H 7600 6300 60  0001 C CNN "Part Number"
F 6 "TRANSFORMR BALUN 2.3-2700MHZ SMD" H 7600 6300 60  0001 C CNN "Description"
	1    7600 6300
	-1   0    0    1   
$EndComp
$Comp
L BALUN T?
U 1 1 4F94D601
P 7600 5500
F 0 "T?" H 7600 5750 70  0000 C CNN
F 1 "BALUN" H 7600 5200 70  0000 C CNN
F 4 "TDK" H 7600 5500 60  0001 C CNN "Manufacturer"
F 5 "HHM1595A1" H 7600 5500 60  0001 C CNN "Part Number"
F 6 "XFMR BALUN RF 3.0-8.0GHZ UWB SMD" H 7600 5500 60  0001 C CNN "Description"
	1    7600 5500
	-1   0    0    1   
$EndComp
$Comp
L BALUN T?
U 1 1 4F94D59C
P 7600 7100
F 0 "T?" H 7600 7350 70  0000 C CNN
F 1 "BALUN" H 7600 6800 70  0000 C CNN
F 4 "Johanson Technology" H 7600 7100 60  0001 C CNN "Manufacturer"
F 5 "2500BL14M100T" H 7600 7100 60  0001 C CNN "Part Number"
F 6 "BALUN CERAMIC CHIP WIMAX 2.5GHZ" H 7600 7100 60  0001 C CNN "Description"
	1    7600 7100
	-1   0    0    1   
$EndComp
$Comp
L FIL-LP0603 U?
U 1 1 4F94D561
P 6300 6250
F 0 "U?" H 6300 6300 60  0000 C CNN
F 1 "FIL-LP0603" H 6300 6200 60  0000 C CNN
F 4 "AVX" H 6300 6250 60  0001 C CNN "Manufacturer"
F 5 "LP0603A1880ANTR" H 6300 6250 60  0001 C CNN "Part Number"
F 6 "FILTER LOW PASS 1880MHZ 0603 SMD" H 6300 6250 60  0001 C CNN "Description"
	1    6300 6250
	1    0    0    -1  
$EndComp
$Comp
L FIL-DEA U?
U 1 1 4F94D4F4
P 6300 5450
F 0 "U?" H 6300 5500 60  0000 C CNN
F 1 "FIL-DEA" H 6300 5400 60  0000 C CNN
F 4 "TDK" H 6300 5450 60  0001 C CNN "Manufacturer"
F 5 "DEA162400HT-8004B1" H 6300 5450 60  0001 C CNN "Part Number"
F 6 "FILTER HIGHPASS WLAN&BLUETOOTH" H 6300 5450 60  0001 C CNN "Description"
	1    6300 5450
	1    0    0    -1  
$EndComp
Text Notes 10200 5950 1    400  ~ 0
mixer
Wire Bus Line
	10500 3700 9550 3700
Wire Bus Line
	10500 3700 10500 6550
Wire Bus Line
	10500 6550 9550 6550
Wire Bus Line
	9550 6550 9550 3700
Wire Wire Line
	2300 3300 2700 3300
Wire Wire Line
	4300 5800 4000 5800
Wire Wire Line
	4000 5800 4000 5150
Wire Wire Line
	4000 5150 3900 5150
Wire Wire Line
	4300 6100 4200 6100
Wire Wire Line
	4200 6100 4200 6900
Wire Wire Line
	4200 6900 7200 6900
Wire Wire Line
	5400 5800 5500 5800
Wire Wire Line
	5500 5800 5500 5600
Wire Wire Line
	5500 5600 5700 5600
Wire Wire Line
	5400 3400 5500 3400
Wire Wire Line
	5500 3400 5500 2900
Wire Wire Line
	5500 2900 7200 2900
Wire Wire Line
	6900 6100 7200 6100
Wire Wire Line
	6900 4500 7200 4500
Wire Wire Line
	8000 3700 8200 3700
Wire Wire Line
	8200 3700 8200 3900
Wire Wire Line
	8200 3900 8300 3900
Wire Wire Line
	8300 4500 8000 4500
Wire Wire Line
	8000 5300 8200 5300
Wire Wire Line
	8200 5300 8200 5500
Wire Wire Line
	8200 5500 8300 5500
Wire Wire Line
	8000 6500 8200 6500
Wire Wire Line
	8200 6500 8200 6300
Wire Wire Line
	8200 6300 8300 6300
Wire Wire Line
	8000 7300 8100 7300
Wire Wire Line
	8100 7300 8100 7000
Wire Wire Line
	10550 6300 11800 6300
Wire Wire Line
	11800 6300 11800 6800
Wire Wire Line
	11800 6800 11900 6800
Wire Wire Line
	8000 6900 11700 6900
Wire Wire Line
	11700 6900 11700 6400
Wire Wire Line
	11700 6400 11900 6400
Wire Wire Line
	13300 6900 13100 6900
Wire Wire Line
	13100 6300 13300 6300
Wire Wire Line
	11900 6200 10550 6200
Wire Wire Line
	8100 7000 11900 7000
Wire Wire Line
	11900 4000 10550 4000
Wire Wire Line
	11900 3200 8100 3200
Wire Wire Line
	13100 3300 13300 3300
Wire Wire Line
	13300 3900 13100 3900
Wire Wire Line
	11900 3400 11700 3400
Wire Wire Line
	11700 3400 11700 3900
Wire Wire Line
	11700 3900 10550 3900
Wire Wire Line
	11900 3800 11800 3800
Wire Wire Line
	11800 3800 11800 3300
Wire Wire Line
	11800 3300 8000 3300
Wire Wire Line
	8100 3200 8100 2900
Wire Wire Line
	8100 2900 8000 2900
Wire Wire Line
	8300 6100 8000 6100
Wire Wire Line
	8300 5700 8000 5700
Wire Wire Line
	8300 4700 8200 4700
Wire Wire Line
	8200 4700 8200 4900
Wire Wire Line
	8200 4900 8000 4900
Wire Wire Line
	8300 4100 8000 4100
Wire Wire Line
	6900 3700 7200 3700
Wire Wire Line
	6900 5300 7200 5300
Wire Wire Line
	5700 3700 5400 3700
Wire Wire Line
	5700 4800 4200 4800
Wire Wire Line
	4200 4800 4200 3700
Wire Wire Line
	4200 3700 4300 3700
Wire Wire Line
	5700 6100 5400 6100
Wire Wire Line
	4300 3400 3900 3400
Wire Wire Line
	2700 3500 2600 3500
Wire Wire Line
	2600 3500 2600 4250
Wire Wire Line
	2600 4250 4000 4250
Wire Wire Line
	4000 4250 4000 4950
Wire Wire Line
	4000 4950 3900 4950
Wire Wire Line
	2300 5050 2700 5050
$Comp
L GSG-SMA-CONN P?
U 1 1 4F94D0F2
P 2000 5050
F 0 "P?" H 2000 5200 60  0000 C CNN
F 1 "GSG-SMA-CONN" H 2000 4900 60  0000 C CNN
	1    2000 5050
	1    0    0    -1  
$EndComp
$Comp
L GSG-SMA-CONN P?
U 1 1 4F94D0E4
P 2000 3300
F 0 "P?" H 2000 3450 60  0000 C CNN
F 1 "GSG-SMA-CONN" H 2000 3150 60  0000 C CNN
	1    2000 3300
	1    0    0    -1  
$EndComp
$Comp
L SKY13351 U?
U 1 1 4F94D08D
P 3300 5050
F 0 "U?" H 3300 5250 60  0000 C CNN
F 1 "SKY13351" H 3300 4850 60  0000 C CNN
	1    3300 5050
	-1   0    0    1   
$EndComp
$Comp
L SKY13351 U?
U 1 1 4F94D06D
P 3300 3400
F 0 "U?" H 3300 3600 60  0000 C CNN
F 1 "SKY13351" H 3300 3200 60  0000 C CNN
	1    3300 3400
	1    0    0    -1  
$EndComp
$Comp
L SKY13317 U?
U 1 1 4F94CFA9
P 4850 3550
F 0 "U?" H 4850 3550 60  0000 C CNN
F 1 "SKY13317" H 4850 3800 60  0000 C CNN
	1    4850 3550
	1    0    0    -1  
$EndComp
$Comp
L FIL-LP0603 U?
U 1 1 4F94CF46
P 6300 3850
F 0 "U?" H 6300 3900 60  0000 C CNN
F 1 "FIL-LP0603" H 6300 3800 60  0000 C CNN
F 4 "AVX" H 6300 3850 60  0001 C CNN "Manufacturer"
F 5 "LP0603A1880ANTR" H 6300 3850 60  0001 C CNN "Part Number"
F 6 "FILTER LOW PASS 1880MHZ 0603 SMD" H 6300 3850 60  0001 C CNN "Description"
	1    6300 3850
	1    0    0    -1  
$EndComp
$Comp
L FIL-DEA U?
U 1 1 4F94CF02
P 6300 4650
F 0 "U?" H 6300 4700 60  0000 C CNN
F 1 "FIL-DEA" H 6300 4600 60  0000 C CNN
F 4 "TDK" H 6300 4650 60  0001 C CNN "Manufacturer"
F 5 "DEA162400HT-8004B1" H 6300 4650 60  0001 C CNN "Part Number"
F 6 "FILTER HIGHPASS WLAN&BLUETOOTH" H 6300 4650 60  0001 C CNN "Description"
	1    6300 4650
	1    0    0    -1  
$EndComp
$Comp
L SKY13351 U?
U 1 1 4F94CD1A
P 8900 6200
F 0 "U?" H 8900 6400 60  0000 C CNN
F 1 "SKY13351" H 8900 6000 60  0000 C CNN
	1    8900 6200
	1    0    0    -1  
$EndComp
$Comp
L SKY13351 U?
U 1 1 4F94CD19
P 8900 5600
F 0 "U?" H 8900 5800 60  0000 C CNN
F 1 "SKY13351" H 8900 5400 60  0000 C CNN
	1    8900 5600
	1    0    0    -1  
$EndComp
$Comp
L SKY13351 U?
U 1 1 4F94CCC3
P 8900 4000
F 0 "U?" H 8900 4200 60  0000 C CNN
F 1 "SKY13351" H 8900 3800 60  0000 C CNN
	1    8900 4000
	1    0    0    -1  
$EndComp
$Comp
L SKY13351 U?
U 1 1 4F94CCC2
P 8900 4600
F 0 "U?" H 8900 4800 60  0000 C CNN
F 1 "SKY13351" H 8900 4400 60  0000 C CNN
	1    8900 4600
	1    0    0    -1  
$EndComp
$Comp
L BALUN T?
U 1 1 4F94CC6F
P 7600 4700
F 0 "T?" H 7600 4950 70  0000 C CNN
F 1 "BALUN" H 7600 4400 70  0000 C CNN
F 4 "TDK" H 7600 4700 60  0001 C CNN "Manufacturer"
F 5 "HHM1595A1" H 7600 4700 60  0001 C CNN "Part Number"
F 6 "XFMR BALUN RF 3.0-8.0GHZ UWB SMD" H 7600 4700 60  0001 C CNN "Description"
	1    7600 4700
	-1   0    0    1   
$EndComp
$Comp
L BALUN T?
U 1 1 4F94CC5E
P 7600 3900
F 0 "T?" H 7600 4150 70  0000 C CNN
F 1 "BALUN" H 7600 3600 70  0000 C CNN
F 4 "Pulse Electronics" H 7600 3900 60  0001 C CNN "Manufacturer"
F 5 "CX2156NL" H 7600 3900 60  0001 C CNN "Part Number"
F 6 "TRANSFORMR BALUN 2.3-2700MHZ SMD" H 7600 3900 60  0001 C CNN "Description"
	1    7600 3900
	-1   0    0    1   
$EndComp
$Comp
L BALUN T?
U 1 1 4F94CC10
P 7600 3100
F 0 "T?" H 7600 3350 70  0000 C CNN
F 1 "BALUN" H 7600 2800 70  0000 C CNN
F 4 "Johanson Technology" H 7600 3100 60  0001 C CNN "Manufacturer"
F 5 "2500BL14M100T" H 7600 3100 60  0001 C CNN "Part Number"
F 6 "BALUN CERAMIC CHIP WIMAX 2.5GHZ" H 7600 3100 60  0001 C CNN "Description"
	1    7600 3100
	-1   0    0    1   
$EndComp
Text Label 13300 6300 2    40   ~ 0
TXRF+
$Comp
L SKY13351 U?
U 1 1 4F94CBE3
P 12500 6300
F 0 "U?" H 12500 6500 60  0000 C CNN
F 1 "SKY13351" H 12500 6100 60  0000 C CNN
	1    12500 6300
	1    0    0    -1  
$EndComp
$Comp
L SKY13351 U?
U 1 1 4F94CBE2
P 12500 6900
F 0 "U?" H 12500 7100 60  0000 C CNN
F 1 "SKY13351" H 12500 6700 60  0000 C CNN
	1    12500 6900
	1    0    0    -1  
$EndComp
Text Label 13300 6900 2    40   ~ 0
TXRF-
Text Label 13300 3900 2    40   ~ 0
RXRF-
$Comp
L SKY13351 U?
U 1 1 4F94CBAF
P 12500 3900
F 0 "U?" H 12500 4100 60  0000 C CNN
F 1 "SKY13351" H 12500 3700 60  0000 C CNN
	1    12500 3900
	1    0    0    -1  
$EndComp
$Comp
L SKY13351 U?
U 1 1 4F94CB86
P 12500 3300
F 0 "U?" H 12500 3500 60  0000 C CNN
F 1 "SKY13351" H 12500 3100 60  0000 C CNN
	1    12500 3300
	1    0    0    -1  
$EndComp
$Comp
L SKY13317 U?
U 1 1 4F94CB80
P 4850 5950
F 0 "U?" H 4850 5950 60  0000 C CNN
F 1 "SKY13317" H 4850 6200 60  0000 C CNN
	1    4850 5950
	1    0    0    -1  
$EndComp
Text Label 13300 3300 2    40   ~ 0
RXRF+
$EndSCHEMATC