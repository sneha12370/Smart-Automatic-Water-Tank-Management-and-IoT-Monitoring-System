#include<lpc21xx.h>
#include "header.h"

#define TRIG_PIN (1<<10)	  //12
#define ECHO_PIN (1<<11)		//13
// ulrasonoc sensor

//unsigned int ultra_init(void){
void ultra_init(void){
   IODIR0|= TRIG_PIN;  
   IODIR0&= ~ECHO_PIN; 
 }  
unsigned int ultra_dist(void){

unsigned int c=0;
IOCLR0=TRIG_PIN; 
ultra_delay(2);	 
IOSET0=TRIG_PIN; 
ultra_delay(10);   
IOCLR0=TRIG_PIN;	

while(!(IOPIN0 & ECHO_PIN)); 

while(IOPIN0 & ECHO_PIN)  
{
c++;
 ultra_delay(1);
}
 // uart0_tx(c/58);
return (c/58);

}
  
		
	
	
	
	
