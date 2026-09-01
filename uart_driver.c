#include<lpc21xx.h>
#include<stdlib.h>
#include "header.h"
void uart0_init(unsigned int baud)
{
 int pclk,result=0;
 if(VPBDIV==0x0)
 pclk=15000000;
 else if(VPBDIV==0x1)
 pclk=60000000;
 else if(VPBDIV==0x2)
 pclk=30000000;
		result=pclk/(16*baud);
		PINSEL0|=0X5;
U0LCR=0X83;
U0DLL=result&0xff;
 U0DLM=(result>>8)&0xff;
 U0LCR=0X03;
}

//TRANSMIT IBYTE DATA

#define THRE ((U0LSR>>5)&1)
void uart0_tx(unsigned char data)
{
 U0THR=data;
 while(THRE==0);
}

//receiver function

#define RDR (U0LSR&1)
unsigned char uart0_rx(void)
{
 while(RDR==0);
 return U0RBR;
}

//UART TRANSMIT STRING FUNCTION

void uart0_tx_string(char *ptr)
{
 while(*ptr!=0)
 {
  uart0_tx(*ptr);
  ptr++;
 }
}

// UART RECEIVER STRINF FUNCTION

void uart0_rx_string(char *p,int n)
{
 int i=0;
 char ch;
 for(i=0;i<n;i++)
 {
 while(RDR==0);
 ch=U0RBR;
 uart0_tx(ch);
 p[i]=ch;
 if(p[i]=='\r')  
 break;
 
}
p[i]='\0';
} 

 
 // UART INTEGER FUNCTION

 void uart0_integer(int num)
 {
  int a[10],i=0;
  if (num==0)
  uart0_tx('0');

  if(num<0)
  {
   num=-num;
   uart0_tx('-');
  }

  while(num>0)
  {
   a[i]=(num%10)+48;
   num=num/10;
   i++;
  }
  for(i=i-1;i>=0;i--)
  uart0_tx(a[i]);
}


 //uart transmit float

 void uart0_tx_float(float f)
 {
	int num;
	if(f<0)
	{
	 uart0_tx('-');
	 f=-f;

	}
	if(f==0)
	{
	 uart0_tx_string("0.0");
	 return;

	}
	num=f;
	uart0_integer(num);
	uart0_tx('.');
	num=(f-num)*1000000;
	uart0_integer(num);
 }


		

