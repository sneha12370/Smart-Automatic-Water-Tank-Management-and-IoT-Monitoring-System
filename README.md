# Smart Water Tank Management System

An embedded C project for the LPC2129 ARM7 microcontroller that automates water tank monitoring and pump control using ultrasonic distance sensing, a water level sensor, a temperature sensor, real-time clock (RTC), EEPROM logging, a 4-bit LCD, and optional ESP8266 Wi-Fi IoT upload to ThingSpeak.

## Features

- Auto mode: pump/valve controlled automatically based on water level thresholds
- Manual mode: toggle pump on/off via push button (SW1), switchable at runtime via EINT0
- Ultrasonic sensor measures tank distance in cm (HC-SR04 style)
- Water level sensor reads 0-100% via ADC
- LM35 temperature sensor via ADC
- DS1307 RTC for real-time date and time over I2C
- AT24C EEPROM stores last water level and temperature readings over I2C
- 3-color LED (Red/Orange/Green) + Buzzer for visual/audio alerts
- Servo motor acts as inlet valve (0 deg = closed, 180 deg = open)
- 4-bit LCD for local display
- UART0 serial terminal at 9600 baud for full debug output
- ESP8266 Wi-Fi module (UART1) uploads sensor data to ThingSpeak 
- CAN1 peripheral driver included 


## Auto Mode Logic

| Water Level | State  | Servo  | LED    | Buzzer               | Warning            |
|-------------|--------|--------|--------|----------------------|--------------------|
| < 35%       | LOW    | Open   | Red    | Off                  | LOW WATER LEVEL    |
| 35% - 70%   | NORMAL | Open   | Green  | Off                  | None               |
| > 70%       | HIGH   | Closed | Orange | On (2 sec per cycle) | HIGH WATER LEVEL   |


## Hardware

- Controller: LPC2129 ARM7 
- Sensors: HC-SR04 ultrasonic,  water level sensor, LM35 temperature sensor
- Actuators: SG90/MG90 servo motor, relay/pump
- Display: 16x2 LCD (4-bit mode)
- Communication: DS1307 RTC and AT24C EEPROM over I2C, ESP8266 over UART1
- Debug: UART0 serial terminal 


## Pin Mapping

| Pin   | Function                        |
|-------|---------------------------------|
| P0.0  | TXD0 (UART0 debug)              |
| P0.1  | RXD0 (UART0 debug)              |
| P0.2  | SCL (I2C)                       |
| P0.3  | SDA (I2C)                       |
| P0.7  | PWM2 (Servo motor)              |
| P0.8  | TXD1 (UART1 / ESP8266)          |
| P0.9  | RXD1 (UART1 / ESP8266)          |
| P0.10 | TRIG (Ultrasonic)               |
| P0.11 | ECHO (Ultrasonic)               |
| P0.14 | SW1                             |
| P0.17 | Red LED (LOW water level)       |
| P0.18 | Orange LED (HIGH water level)   |
| P0.19 | Green LED (NORMAL water level)  |
| P0.21 | Buzzer                          |
| AD0.0 | Water level sensor (ADC CH0)    |
| AD0.2 | Temperature sensor LM35 (ADC CH2)|

### LCD Pin Mapping (4-bit mode, Port 1)

| LCD Signal | LPC2129 Pin |
|------------|-------------|
| RS         | P1.17       |
| RW         | P1.18       |
| E          | P1.19       |
| D4         | P1.20       |
| D5         | P1.21       |
| D6         | P1.22       |
| D7         | P1.23       |


## File Structure

smart_tank_project

1.main (4).c          - Main loop: sensor reads, auto/manual logic, RTC, EEPROM, UART output
2.header (1).h        - All extern function declarations and typedefs
3. adc_driver.c        - ADC init and single-channel read (10-bit result) 
4. waterlevel.c        - Water sensor: ADC CH0, scales to 0-100% using (adc*100)/1023
5.temperature.c       - LM35: ADC CH2, formula (3.3/1024)*adc*100 gives degrees C
6.ultrasonic.c        - HC-SR04: TRIG on P0.10, ECHO on P0.11, returns cm (count/58)
7. servo_motor.c       - PWM2 servo: 0 deg=1000us, 90 deg=1500us, 180 deg=2000us, period=20ms
8. lcd_4bit_driver.c   - 4-bit LCD driver (cmd, data, string, integer, float)
9. uart_driver.c       - UART0: init, tx byte/string/int/float, rx byte/string
10. i2c.c               - I2C master: init (100 kHz), write and read with error reporting
11. eeprom1.c           - Reads stored water level (0x07) and temperature (0x09) from AT24C
12. eint0 (1).c         - EINT0 ISR: falling-edge interrupt, 50ms debounce, toggles auto/manual flag
13. delay.c             - Timer0-based delays: delay_ms, delay_sec, ultra_delay (microseconds)
14. wifi (2).c          - ESP8266 AT-command driver, uploads wl+temp to ThingSpeak via HTTP GET
15. can_rx.c            - CAN1 receive driver 
16. can_tx.c            - CAN1 transmit driver 


## I2C Device Addresses

| Device       | Write Addr | Read Addr | Registers Used                                                               |
|--------------|------------|-----------|------------------------------------------------------------------------------|
| DS1307 RTC   | 0xD0       | 0xD1      | Seconds(0x00), Minutes(0x01), Hours(0x02), Date(0x04), Month(0x05), Year(0x06) |
| AT24C EEPROM | 0xA0       | 0xA1      | Water level low byte(0x07), high byte(0x08), Temp low byte(0x09), high byte(0x0A) |



## UART Debug Output (9600 baud)

On every loop iteration the terminal prints:

==============================================================================
************************SMART WATER TANK MANAGEMENT SYSTEM*********************
===============================================================================
CONTROLLER  :LPC2129 ARM7
PROJECT STATUS  :RUNNING
MODE:AUTO MODE
WATER LEVEL: 45
DISTANCE= 12 cm
TEMPERATURE= 28
water level:NORMAL
Pump Status:ON
Relay Status:ON
Valve Status:OPEN
Date :01:09:2026
Time :10:30:45 AM
 waterlevel value stored in EEPROM:45
 temperature value stored in EEPROM:28
EEPROM:DATA SAVED
***********************************************************************



