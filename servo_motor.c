#include<lpc21xx.h>
#include "header.h"  

#define MOTOR 1<<7		//10
void servo_motor_init(void){

	PINSEL0|=(2<<14);
	PWMPR=14;
	PWMMR0=20000;
	PWMMR2=1500;
			PWMMCR=(1<<1);
			PWMLER=(1<<0)|(1<<1);		    //(1<<0)|(1<<2)
			PWMPCR=(1<<10);	           //(1<<10)|(1<<3)
			PWMTCR=(1<<1);				       	//(1<<1)|(1<<0)
			PWMTCR=(1<<0)|(1<<3);
}

void servo_0deg(void){
	PWMMR2=1000;
	PWMLER=(1<<2);

}

void servo_90deg(void){
	PWMMR2=1500;						//1000
	PWMLER=(1<<2);

}

void servo_180deg(void){
	PWMMR2=2000;						
	PWMLER=(1<<2);

}


