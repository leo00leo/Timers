#include "stm32f103xb.h"
#include "timer.h"
#include "adc.h"
int pmt=0;
int led=3;
int conversion=0;
int val=0;

int main(){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    adc_init();
    pwm_init(1,1000);
    while(1){

        conversion = adc_read(pmt);

        val=(conversion*100)/4095;

        pwm(1,val);
        
    }
}