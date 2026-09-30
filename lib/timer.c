#include "stm32f103xb.h"
#include "timer.h"


void timer_init(){
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN; //Habilita clock del timer.
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
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN; //Habilita clock del timer.
    TIM2->CR1&~(TIM_CR1_CEN); //Detiene el contador para configurarlo.
    TIM2->PSC=7; //Carga PSC para lograr 1 MHz.
    TIM2->ARR=0XFFFF; //Valor máximo antes de overflow.
    TIM2->CNT=0; //Reinicia CNT.
    TIM2->EGR|=TIM_EGR_UG; //Genera UG.
    TIM2->CR1|=TIM_CR1_CEN; //Arranca el contador con CEN.
}
uint32_t timer_millis(){
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN; //Habilita clock del timer.
    TIM2->CR1&~(TIM_CR1_CEN); //Detiene el contador para configurarlo.
    TIM2->PSC=7; //Carga PSC para lograr 1 MHz.
    TIM2->ARR=0XFFFF; //Valor máximo antes de overflow.
    TIM2->CNT=0; //Reinicia CNT.
}
void TIM2_IRQhandler(){
    volatile int ovf;
    if(TIM2->SR&TIM_SR_UIF){
        TIM->SR&=~TIM_SR_UIF;
        ovf+=(0XFFFF+1);
    }
}
    TIM2->CNT+ovf;
void delay_us(uint32_t us);

    