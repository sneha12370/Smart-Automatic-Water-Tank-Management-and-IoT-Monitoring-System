
// temperature sensor

#include<lpc21xx.h>
#include "header.h"

	int temp_sensor(void){
    unsigned int adc_out;
    int temp;
    adc_out=adc_read(2);	            //adc channel 2
    temp=(3.3/1024)*adc_out*100;
		//vout=(adc_out*3.3)/1023;
    //temp=vout*100;
     
   return temp;

}
