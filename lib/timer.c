#include "stm32f103xb.h"
#include "timer.h"
int volatile ovf=0; // Variable para sumar los overflows acumulados.

void timer_init(){
    RCC->APB1ENR|=RCC_APB1ENR_TIM2EN; //Habilita clock del timer.
    TIM2->CR1&~(TIM_CR1_CEN); //Detiene el contador para configurarlo.
    TIM2->PSC=7; //Carga PSC para lograr 1 MHz.
    TIM2->ARR=0XFFFF; //Valor máximo antes de overflow.
    TIM2->CNT=0; //Reinicia CNT.
    TIM2->DIER|=TIM_DIER_UIE; //Habilita interrupción por update.
    TIM2->EGR|=TIM_EGR_UG; //Genera UG.
    TIM2->SR|=TIM_SR_UIF; //Limpia UIF.
    NVIC_EnableIRQ(TIM2_IRQn); //Habilitar la interrupcion en NVIC.
    TIM2->CR1|=TIM_CR1_CEN; //Arranca el contador con CEN.
}
void delay_init(){
    RCC->APB1ENR|=RCC_APB1ENR_TIM2EN; //Habilita clock del timer.
    TIM2->CR1&~(TIM_CR1_CEN); //Detiene el contador para configurarlo.
    TIM2->PSC=7; //Carga PSC para lograr 1 MHz.
    TIM2->ARR=0XFFFF; //Valor máximo antes de overflow.
    TIM2->CNT=0; //Reinicia CNT.
    TIM2->EGR|=TIM_EGR_UG; //Genera UG.
    TIM2->CR1|=TIM_CR1_CEN; //Arranca el contador con CEN.
}
uint32_t timer_millis(){
    RCC->APB1ENR|=RCC_APB1ENR_TIM2EN; //Habilita clock del timer.    
    TIM2->CR1&~(TIM_CR1_CEN); //Detiene el contador para configurarlo.
    TIM2->PSC=7; //Carga PSC para lograr 1 MHz.
    TIM2->ARR=0XFFFF; //Valor máximo antes de overflow.
    TIM2->CNT=0; //Reinicia CNT.
    return((TIM2->CNT+ovf)/1000); //Devuelve el valor. 
}
void TIM2_IRQhandler(){
        if(TIM2->SR&TIM_SR_UIF){ // Si se llego al overflow:
            TIM2->SR&=~TIM_SR_UIF; 
            ovf+=(0XFFFF+1); // La variable ovf toma la vuelta del overflow y lo devuelve con el mismo valor pero con uno mas.
        }
}
void delay_us(uint32_t us){
    RCC->APB1ENR|=RCC_APB1ENR_TIM2EN; //Habilita clock del timer.
    TIM2->CR1&~(TIM_CR1_CEN); //Detiene el contador para configurarlo.
    TIM2->CNT=0; //Se reinicia el contador.
    TIM2->CR1|=TIM_CR1_CEN; //Arranca el contador.
    while(TIM2->CNT<us){
    }
}
void delay_ms(uint32_t ms){
    RCC->APB1ENR|=RCC_APB1ENR_TIM2EN; //Habilita clock del timer.
    TIM2->CR1&~(TIM_CR1_CEN); //Detiene el contador para configurarlo.
    TIM2->CNT=0; //Se reinicia el contador.
    TIM2->CR1|=TIM_CR1_CEN; //Arranca el contador.
    while(TIM2->CNT<ms){
        int p;
        delay(1000);
        p++;    
    }
}
void pwm_init(uint8_t canal, uint32_t frec){
    RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;
    switch(canal){

    case 1:
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN; // Hablito los clocks del puerto A.
    GPIOA->CRL&=~(0xF<<(((canal))*4)); //Iniciamos la configuracion del pin del puerto A.
    GPIOA->CRL|=(0xB<<(((canal))*4)); //Configuro el pin del puerto A como salida.
    TIM3->CCMR1&=~(0B111<<4); 
    TIM3->CCMR1&=(0B110<<4); 
    TIM3->CCER|=TIM_CCER_CC1E; //Habilita la salida.
    break;

    case 2:
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN; // Hablito los clocks del puerto A.
    GPIOA->CRL&=~(0xF<<(((canal))*4)); //Iniciamos la configuracion del pin del puerto A.
    GPIOA->CRL|=(0xB<<(((canal))*4)); //Configuro el pin del puerto A como salida.
    TIM3->CCMR1&=~(0B111<<12); 
    TIM3->CCMR1&=(0B110<<12); 
    TIM3->CCER|=TIM_CCER_CC2E; //Habilita la salida.
    break;

    case 3:
    RCC->APB2ENR|=RCC_APB2ENR_IOPBEN; // Hablito los clocks del puerto B.
    GPIOB->CRL&=~(0xF<<(((canal))*4)); //Iniciamos la configuracion del pin del puerto B.
    GPIOB->CRL|=(0xB<<(((canal))*4)); //Configuro el pin del puerto B como salida.
    TIM3->CCMR1&=~(0B111<<4); 
    TIM3->CCMR1&=(0B110<<4); 
    TIM3->CCER|=TIM_CCER_CC3E; //Habilita la salida.
    break;

    case 4:
    RCC->APB2ENR|=RCC_APB2ENR_IOPBEN; // Hablito los clocks del puerto B.
    GPIOB->CRL&=~(0xF<<(((canal))*4)); //Iniciamos la configuracion del pin del puerto B.
    GPIOB->CRL|=(0xB<<(((canal))*4)); //Configuro el pin del puerto B como salida.
    TIM3->CCMR1&=~(0B111<<12); 
    TIM3->CCMR1&=(0B110<<12); 
    TIM3->CCER|=TIM_CCER_CC4E; //Habilita la salida.
    break;
    default:
    break;

    TIM3->PSC=7; //Ajustamos el Prescaler a 7 para tener una frecuencia de 1MHz.
    TIM3->ARR=((1000000/frec)-1); 
    TIM3->EGR|=TIM_EGR_UG; //Aplicar la configuración nueva del timer antes de arrancar.
    TIM3->CR1|=TIM_CR1_CEN; //Arranca el contador con CEN.
    }
}
void pwm(uint8_t canal , uint8_t duty){
    if(duty>100) duty = 100;
        if(canal == 1)
            TIM3 -> CCR1 = ((TIM3 -> ARR + 1)*duty/100);
            else if(canal==2) TIM3 -> CCR2 = ((TIM3 -> ARR + 1)*duty/100);
            else if(canal==3) TIM3 -> CCR3 = ((TIM3 -> ARR + 1)*duty/100);
            else if(canal==4) TIM3 -> CCR4 = ((TIM3 -> ARR + 1))*duty/100;
}
