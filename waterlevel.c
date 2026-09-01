			
			// water level sensor

#include<lpc21xx.h>
#include "header.h"

unsigned int water_sensor(void){
    unsigned int adc_wl;
		int wat;	
    adc_wl=adc_read(0);          //adc channel 0 
		wat=(adc_wl*100)/1023;
		return wat;

}

