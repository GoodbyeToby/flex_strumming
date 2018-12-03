/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

void nanowait(int);

void adc_init(void) {

  //PB01, IN9, 12bit res, right align, 14MHz clk.

  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  GPIOB->MODER |= 0xC;
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->CR2 |= RCC_CR2_HSI14ON;

  while(!(RCC->CR2 & RCC_CR2_HSI14RDY));
  ADC1->CR |= ADC_CR_ADEN;
  while(!(ADC1->ISR & ADC_ISR_ADRDY));
  //while((ADC1->CR & ADC_CR_ADSTART));

}

int adc_read(void) {

  while(1){

      ADC1->CHSELR = 0;
      ADC1->CHSELR |= 1 << 9; //IN9

      while(!(ADC1->ISR & ADC_ISR_ADRDY));
      ADC1->CR |= ADC_CR_ADSTART;
      while(!(ADC1->ISR & ADC_ISR_EOC));

      return ADC1->DR;
  }

}

int sensor()
{
  int strum;
  int old;
  int new;


    strum = 0;


     old = adc_read();
     nanowait(10000);
     new = adc_read();

     if((0x0223<old) && (old<0x02EB)){ //Idle Range
         if(!((0x0223<new) && (new<0x02EB))){
             strum = 1;
         }
/*         if ((0 < new) && (new<0x0223) ){ //Low range
             strum = 1;
         }
         else if ((0x02EB<new) && (new<0x01000)){
             strum = 1;
         }
         else {
           strum = 0;
         }
*/     }
     else{
         strum = 0;
     }

  return strum;
}

int main(void)
{

  //Used by LCD = pa5-12, pb10-12, pc4,5
  //Flex Sensor = PB01
  //PushButtons =

  int x;


  adc_init();

  RCC->AHBENR |= RCC_AHBENR_GPIOBEN; //LED to check strum
  GPIOB->MODER |= 0x100;
  GPIOB->ODR &= ~(0x11);

  //adc_read();

  for(;;){

      x = sensor();

      if (x){
          GPIOB->ODR |= 0x10;
          nanowait(100000000);
      }
      else{
          GPIOB->ODR &= ~0x10;
      }
  }

}
