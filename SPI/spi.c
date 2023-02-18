
#include "spi.h"

//SPI-SCK = PA5
//SPI-MISO = PA6
//SPI-MOSI = PA7
//SPI-SS = PB6

void spi_gpio_init(void)
{
	//gpio configuration
	RCC->AHB1ENR  |= GPIOAEN;  //PA
	RCC->AHB1ENR  |= GPIOBEN;  //PB
	
	GPIOA->MODER |= (1<<11); //PA5 -10
	GPIOA->MODER |= (1<<13);  //PA6
	GPIOA->MODER |= (1<<15);  //PA7
	GPIOB->MODER |= (1<<12);  //PB6 slave select pin as output pin
	
	GPIOC->AFR[0] |= (1<<20); // PA5 
	GPIOC->AFR[0] |= (1<<22);
	
	GPIOC->AFR[0] |= (1<<24); // PA6
	GPIOC->AFR[0] |= (1<<26);
	
	GPIOC->AFR[0] |= (1<<28); // PA7
	GPIOC->AFR[0] |= (1<<30);
	
}	

void spi1_conf(void)
{
	//enable spi clock 
	RCC->APB2ENR |= SPI1EN;
	
	//baudrate control......peripheral bus clock  
	SPI1->CR1 |= (1<<3); //BR = 001
	
	//Clock phase and  Clock polarity config
	SPI1->CR1 |= (1<<0); 
	SPI1->CR1 |= (1<<1);
	
	//Full duplex mode .. transfer mode
	SPI1->CR1 |= (0<<10);
	
	//Set MSB first 
	SPI1->CR1 |= (0<<7);
	
	//set mode of master
	SPI1->CR1 |= (1<<2);
	
	//data frame formate
	SPI1->CR1 |= (0<<11);
	
	//software slave mode SSM=1, SSI=1
	SPI1->CR1 |= (1<<8);
	SPI1->CR1 |= (1<<9);
	
	//enable spi module 
	SPI1->CR1 |= (1<<6);

}

void spi1_transmit(uint8_t *data, uint32_t size)
{
	uint32_t i = 0; //for while loop
	uint8_t temp; // for read data reg to clear
	//loop for transmit all the data from the register
	while (i<size)
	{
		// wait until TXE is set
		while(SPI1->SR)
		SR_TXE
	}
	
}