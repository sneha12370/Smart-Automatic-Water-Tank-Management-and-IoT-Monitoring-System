#include<lpc21xx.h>
#include "header.h"

void lcd_init()
{
 IODIR1=0XFE<<16;
 IOCLR1=1<<19;
 lcd_cmd(0x02);
 lcd_cmd(0x03);
 lcd_cmd(0x28);
 lcd_cmd(0x0e);
 lcd_cmd(0x01);
}


void lcd_data(unsigned char data)
{
//HIGHER NIBBLE
 unsigned int temp;
 IOCLR1=0XFE<<16;
 temp=(data&0xf0)<<16;
 IOSET1=temp;
 IOSET1=1<<17;
 IOCLR1=1<<18;
 IOSET1=1<<19;
 delay_ms(2);
 IOCLR1=1<<19;

   //LOWER NIBBLE
 
 IOCLR1=0XFE<<16;
 temp=(data&0x0F)<<20;
 IOSET1=temp;
 IOSET1=1<<17;
 IOCLR1=1<<18;
 IOSET1=1<<19;
 delay_ms(2);
 IOCLR1=1<<19;
}


void lcd_cmd(unsigned char cmd)
{
//HIGHER NIBBLE
 unsigned int temp;
 IOCLR1=0XFE<<16;
 temp=(cmd&0xf0)<<16;
 IOSET1=temp;
 IOCLR1=1<<17;
 IOCLR1=1<<18;
 IOSET1=1<<19;
 delay_ms(2);
 IOCLR1=1<<19;

   //LOWER NIBBLE
 
 IOCLR1=0XFE<<16;
 temp=(cmd&0x0F)<<20;
 IOSET1=temp;
 IOCLR1=1<<17;
 IOCLR1=1<<18;
 IOSET1=1<<19;
 delay_ms(2);
 IOCLR1=1<<19;
}


void lcd_string(char *ptr)
{
 while(*ptr!=0)
 {
  lcd_data(*ptr);
  ptr++;

 }
}

void lcd_integer(int num)
{
int a[10],i=0;
   if(num==0)
   {
   lcd_data('0');
   }

   if(num<0)			  
   {
   num=-num;
   lcd_data('-');
   }
   while(num>0)
   {
	a[i]=(num%10)+48;
	num=num/10;
	i++;
   }
 for(i=i-1;i>=0;i--)
 {
 lcd_data(a[i]);
 }
}

void lcd_float(float num)
{
 int n;
 n=(int)num;
 lcd_integer(n);
 lcd_data('.');
 //num=num-n;
 n=(int)((num-n)*1000000);
 lcd_integer(n);
}


