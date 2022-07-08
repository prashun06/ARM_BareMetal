

#include <stdint.h>
#include "stm32f407xx.h"

#define RCC_AHB1ENR_OFFSET     0x30

#define RCC_BASE_ADDR         (0x40023800)


#define RCC_AHB1ENR_ADDR   *((volatile unsigned long *)(RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET) )

#define GPIOD_MODER_OFFSET    0x00

#define BASE_ADDR_GPIOD     (0x40020C00)

#define GPIOD_MODER   *((volatile unsigned long *)(BASE_ADDR_GPIOD + GPIOD_MODER_OFFSET) )

#define DATA_OUT_OFFSET      0x14

#define  GPIOD_ODR  *((volatile unsigned long *)(BASE_ADDR_GPIOD + DATA_OUT_OFFSET))

int main (void)
{
	//enable the clk of tha pin
	RCC_AHB1ENR_ADDR |= (1<<3);  //bit 0 enable the clk for gpio A
	
	//configure the gpio pin to output mode using mode reg
	GPIOD_MODER  |= (1<< (12 * 2)); //set 12 and 13th pin to 0x01 to 6th pin output
	
	//use data reg of GPIO port to write or read
	GPIOD_ODR  |=(1<<12);  //6TH Pin data 1 
	
	
	
	return 0;
}	
