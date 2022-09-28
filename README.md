# Rear-chasis-module-of-Electric-Race-Car
This software pack contains an implementation of the wireless data acquisition and the rear chasis control module of the Formula Student Electric Racing car with CAN network send and receive function.
. 

Some chip functions are implemented based on the source code provided by STMICROELECTRONICS MCD Application Team and ALIENTEK.


# Code Structure
The code is made up of 2 parts:

**Rear Chassis Control**
Main code is under *./rear chasis control/User*, including functions of delay, timer, LED, usart, CAN module etc. In the main program, the controller implements two CAN network signals and controls the corresponding vehicle components, such as LEDs, water pump, buzzer, tail lights, DRS (Drag Reduction System)

**Wireless Data Acquisition**
Main code is under *./Wireless data acquisition/User*. This part accepts the information transmitted by the two CAN networks and processes the data, transfers the data to the host computer via different transmission methods e.g. via wireless transmission, serial port, SD card storage etc. and monitors the vehicle condition in real time or analyses it after a test drive using a customised visualisation interface. The different transmission methods as well as the functions of the real time clock, CAN module etc. are implemented in *./Wireless data acquisition/HARDWARE*.

# Hardware Dependencies
STM32F103, STM32F105 Series Microcontrollers
DS1302 real-time clock
etc.

# Additional information
*./Documents for hardware* contains the hardware component diagrams of the ECU for the two part code integration and the PCB files and a purely hardware-based BSPD(brake-system-plausibility-device) for racing safety.
