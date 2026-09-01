#include<lpc21xx.h>
#include "header.h"
#include<stdio.h>

void eeprom(void)
{
	unsigned short int wat,tem;
	wat=i2c_read(0xa1,0x07);
	//wat|=(unsigned short int)i2c_read(0xa1,0x08)<<8;
	tem=i2c_read(	0xa1,0x09);
	//tem=(unsigned short int)i2c_read(0xa1,0x0A)<<8;

	
	
	uart0_tx_string(" \r\n waterlevel value stored in EEPROM:");
	uart0_integer(wat);
  uart0_tx_string(" \r\n temperature value stored in EEPROM:");
	uart0_integer(tem);
uart0_tx_string("\r\n");
uart0_tx_string("EEPROM:DATA SAVED\r\n");
}
