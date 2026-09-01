Smart Automatic Water Tank Management and IoT Monitoring System

## Overview

The Smart Automatic Water Tank Management and IoT Monitoring System is an embedded and IoT-based project developed using the LPC2129 ARM7 Microcontroller.

The system continuously monitors the water level and temperature, automatically controls the water pump and valve, displays real-time information on an LCD, stores important data, and provides remote monitoring through the ESP-01 Wi-Fi Module and ThingSpeak cloud platform.

## Key Features

- Automatic water-level monitoring
- Automatic water pump ON/OFF control
- Ultrasonic-based water-level measurement
- Temperature monitoring using LM35
- Automatic water valve control using Servo Motor
- Real-time 16×2 LCD display
- UART-based monitoring and debugging
- EEPROM-based data storage
- SD card-based operational data logging
- IoT monitoring using ESP-01 Wi-Fi Module
- ThingSpeak cloud connectivity
- DS1307 RTC-based date and time management
- LED and Buzzer alerts
- CAN communication support for future expansion

## Hardware Components

- LPC2129 ARM7 Microcontroller
- Water Level Sensor
- Ultrasonic Sensor
- LM35 Temperature Sensor
- ESP-01 Wi-Fi Module
- DS1307 RTC
- AT24C256 EEPROM
- SD Card Module
- MCP2551 CAN Transceiver
- Relay Module
- Water Pump
- Servo Motor
- 16×2 LCD
- LEDs
- Buzzer
- User Switches

The project uses UART, I²C, SPI, CAN and GPIO communication interfaces for connecting different peripherals and modules.

## Working Principle

The LPC2129 receives data from the water-level sensor, ultrasonic sensor, LM35, user switches and RTC. Based on the sensor readings, it controls the relay-driven water pump and servo-controlled valve.

When the water level becomes low, the system can automatically turn the pump ON. When the tank reaches the required maximum level, the pump is turned OFF to prevent overflow and reduce unnecessary power consumption.

The system status is displayed locally through the LCD and transmitted through UART. Important information is also stored in memory and uploaded to ThingSpeak through the ESP-01 Wi-Fi module for remote monitoring.

## IoT Monitoring

The ESP-01 Wi-Fi Module communicates with the LPC2129 through UART and sends information such as:

- Water Level
- Temperature
- Pump Status
- Valve Status
- Alarm Conditions

The data is uploaded to ThingSpeak, allowing the system to be monitored remotely over the Internet.

## Applications

This system can be adapted for:

- Apartments
- Hospitals
- Industries
- Educational Institutions
- Commercial Buildings
- Smart Water Management Systems

## Future Scope

The CAN communication capability allows multiple controllers or water tanks to communicate with each other, making the system suitable for future distributed and industrial water-management applications.

## Technologies

Microcontroller: LPC2129 ARM7
Programming: Embedded C
Communication: UART, I²C, SPI, CAN
IoT: ESP-01 + ThingSpeak
Sensors: Water Level, Ultrasonic, LM35
Storage: AT24C256 EEPROM + SD Card

## Project Goal

The main goal of this project is to reduce water wastage, unnecessary electricity consumption, and manual effort by combining automatic control, real-time monitoring, data logging, and IoT connectivity into a single embedded system.
