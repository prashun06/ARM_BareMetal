

#include <stdint.h>
#include "stm32f407xx.h"


#define GPIODEN      (1 << 3)
#define Pin12        (1 << 11)



int main()
{
	
	RCC->AHB1ENR |=GPIODEN; //enable the clock
	
	//GPIOD->MODER |=  GPIODMode;  //input mode
	GPIOD->MODER  |= ( 1 << 22);
	
	
	while(1)
	{
		GPIOD->ODR ^= Pin12 ;
		for(uint32_t i=0; i<(168000000); i++){}

	}
}


	
