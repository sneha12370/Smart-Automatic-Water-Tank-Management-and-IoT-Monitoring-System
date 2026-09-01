#include <lpc21xx.h>
#include "header.h"
 void adc_init(void)
 {
 PINSEL1|=0X15400000;
 ADCR=0X00200400;
}
unsigned int adc_read(unsigned char ch_num)
{
 unsigned int temp;
 ADCR|=(1<<ch_num);	  //SELECT CHANNEL NUM
 ADCR|=(1<<24);		  //START ADC
 while(((ADDR>>31)&1)==0);
 ADCR^=(1<<24);
 ADCR^=(1<<ch_num);
 temp=(ADDR>>6)&0x3ff;
 return temp;
}

