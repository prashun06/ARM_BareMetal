

#include <stdint.h>
#include "stm32f407xx.h"


#define GPIODEN      (1 << 3)
#define GPIOEEN      (1 << 4)
#define Pin11        (1 << 11)
#define Pin3        (1 << 3)



int main()
{
  //2 type of clock enable
	RCC->AHB1ENR |=GPIODEN; 
	RCC->AHB1ENR |= GPIOEEN;
  
	//2 type of direction reg
	GPIOD->MODER  |= ( 1 << 22);
	//GPIOE->MODER  &=~ ( 1 << 6);
	//GPIOE->MODER  &=~ ( 1 << 7);
	
	
	while(1)
	{
		if(GPIOE->IDR & Pin3)  // input k1 batton input
		{
			GPIOD->BSRR |= Pin11;
		}
		else{
			GPIOD->BSRR |= (1<<27);  // led bit reset 
		}
		
	}
}


	
