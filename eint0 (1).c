#include<lpc21xx.h>
#include "header.h"
extern volatile char flag;

void config_vic_for_eint0(void)
{
	PINSEL1|=1;
	EXTMODE=1;
	EXTPOLAR=0;
	VICIntSelect=0;
	VICVectCntl0=14|(1<<5);
	VICVectAddr0=(unsigned int) EINT0_Handler;
	VICIntEnable=(1<<14);
}

void EINT0_Handler(void) __irq
{
	EXTINT=1;       // clear the triggering edge
	delay_ms(50);   // wait for mechanical bouncing to settle
	EXTINT=1;       // clear any bounce edges latched during the delay
	flag^=1;
	VICVectAddr=0;
}
