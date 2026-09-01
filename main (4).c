4	#include<lpc21xx.h>
	#include "header.h"
	#include<stdio.h>

	#define SW1 ((IOPIN0>>14)&1)			// FOR MANUAL MODE: PUMP ON (OR) OFF
	#define GREEN_LED (1<<19)
	#define ORANGE_LED (1<<18)
	#define RED_LED (1<<17)
	#define BUZ (1<<21)

	volatile char flag;
	int main()
	{
	  char s[50];
		static int pump_state=0;
    int dis,wl,temper=0;
    char day,month,year,hr,min,sec,hr1;

IODIR0|=RED_LED;		
IODIR0|=ORANGE_LED;		
IODIR0|=GREEN_LED;		
IODIR0|=BUZ;		

IOSET0=RED_LED;
IOSET0=ORANGE_LED;
IOSET0=GREEN_LED;
delay_sec(1);

uart0_init(9600);
adc_init();
i2c_init();
ultra_init();
servo_motor_init();
lcd_init();
config_vic_for_eint0();
//uart1_init(115200);
//can1_init();


i2c_write(0xd0,0x02,0x72);	  
i2c_write(0xd0,0x01,0x58);	  
i2c_write(0xd0,0x00,0x55);	  

i2c_write(0xd0,0x04,0x12);
i2c_write(0xd0,0x05,0x08);
i2c_write(0xd0,0x06,0x26);


while(1)
{
	
uart0_tx_string("==============================================================================\r\n");
uart0_tx_string("************************SMART WATER TANK MANAGEMENT SYSTEM*********************\r\n");
uart0_tx_string("===============================================================================\r\n");
uart0_tx_string("CONTROLLER  :LPC2129 ARM7\r\n");
uart0_tx_string("PROJECT STATUS  :RUNNING\r\n");
	
if(flag==1)
{
 uart0_tx_string("MODE:");
 uart0_tx_string("MANUAL MODE\r\n");
}
else
{
uart0_tx_string("MODE:");
uart0_tx_string("AUTO MODE\r\n");
}

delay_ms(500);

// WATER LEVEL SENSOR
wl=water_sensor();
uart0_tx_string("WATER LEVEL: ");
uart0_integer(wl);
uart0_tx_string("\r\n");
delay_ms(500);

	//ULTRASONIC
dis=ultra_dist();
uart0_tx_string("DISTANCE= ");
sprintf(s,"%d cm\r\n",dis);
//uart0_integer(dis);
uart0_tx_string(s);
delay_ms(500);
			
	//TEMPERATURE
temper=temp_sensor();
uart0_tx_string("TEMPERATURE= ");
sprintf(s,"%d\r\n",temper);
//uart0_integer(temper);
uart0_tx_string(s);
delay_ms(500);
	
if(flag==0)
{

	//flag==0 :auto mode
   if(wl<35){
					uart0_tx_string("water level:LOW\r\n");
          uart0_tx_string("Pump Status:ON\r\n");
					uart0_tx_string("Relay Status:ON\r\n");
					uart0_tx_string("Valve Status:OPEN\r\n");
					uart0_tx_string("*************  WARNING:LOW WATER LEVEL  ************\r\n");
		      servo_180deg();
	   	    IOCLR0=RED_LED;	     // red led on
					IOSET0=GREEN_LED;	   // green led off
					IOSET0=ORANGE_LED;	 // 	orange led off
					delay_ms(500);
					}
			else if((wl>35)&&(wl<70))
			{
					uart0_tx_string("water level:NORMAL\r\n");
          uart0_tx_string("Pump Status:ON\r\n");
					uart0_tx_string("Relay Status:ON\r\n");
					uart0_tx_string("Valve Status:OPEN\r\n");
		      servo_180deg();
					IOCLR0=GREEN_LED;	   // green led on
					IOSET0=RED_LED;	    // red led off
					IOSET0=ORANGE_LED;	// 	orange led off
	   	    delay_ms(500);													          
					}
		else	if(wl>70)
		{
		      uart0_tx_string("water level:HIGH\r\n");
		      uart0_tx_string("Pump Status:OFF\r\n");
	        uart0_tx_string("Relay Status:OFF\r\n");
					uart0_tx_string("Valve Status:CLOSED\r\n");
					uart0_tx_string("*************  WARNING:HIGH WATER LEVEL  ************\r\n");
		      servo_0deg();
		      IOSET0=1<<17;
		      IOCLR0=ORANGE_LED;	    // orange led on
					IOSET0=RED_LED;	    // red led off
					IOSET0=GREEN_LED;	 // green	led off

			IOSET0=BUZ;
			delay_sec(2);
			IOCLR0=BUZ;
			delay_ms(500);
		}
		}

else if(flag==1)
{
	//flag==1 : manual mode
    if(((IOPIN0>>14)&1)==0)
      {
      while(((IOPIN0>>14)&1)==0);
				delay_ms(50);
				if(pump_state==0)
				{
					 uart0_tx_string("PUMP ON\r\n");
						servo_180deg();
						pump_state=1;
				}
				else
				{
				uart0_tx_string("PUMP OFF\r\n");
				servo_0deg();	
				pump_state=0;
				}
			}
}	
		
uart0_tx_string("Date :");
day=i2c_read(0xd1,0x04);
month=i2c_read(0xd1,0x05);
year=i2c_read(0xd1,0x06);


uart0_tx(day/16+48);
uart0_tx(day%16+48);
uart0_tx(':');
uart0_tx(month/16+48);
uart0_tx(month%16+48);
uart0_tx(':');
uart0_tx_string("20");
uart0_tx(year/16+48);
uart0_tx(year%16+48);
uart0_tx_string("\r\n");

uart0_tx_string("Time :");

hr=i2c_read(0xd1,0x02);
min=i2c_read(0xd1,0x01);
sec=i2c_read(0xd1,0x00);
hr1=hr&0x0f;
uart0_tx(hr1/16+48);
uart0_tx(hr1%16+48);
uart0_tx(':');
uart0_tx(min/16+48);
uart0_tx(min%16+48);
uart0_tx(':');
uart0_tx(sec/16+48);
uart0_tx(sec%16+48);


if(hr>>5&1){
uart0_tx_string(" PM");
}
else{
uart0_tx_string(" AM");
}
uart0_tx_string("\r\n");
		delay_ms(500);

	//EEPROM- WATER LEVEL SENSOR	VALUES 
	i2c_write(0xa0,0x07,wl&0xff);
	delay_ms(10);
	i2c_write(0xa0,0x08,(wl>>8)&0xff);
	delay_ms(10);
	//EEPROM- TEMPERATURE SENSOR	VALUES 
	i2c_write(0xa0,0x09,temper&0xff);
	delay_ms(10);
	i2c_write(0xa0,0x0A,(temper>>8)&0xff);
	delay_ms(10);

	//uart0_tx_string("EEPROM:DATA SAVED\r\n");
	eeprom();

	uart0_tx_string("***********************************************************************	\r\n");

	//wifi(wl, temper);
	delay_ms(500);
}
}	

