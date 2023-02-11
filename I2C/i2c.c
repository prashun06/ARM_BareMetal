
#include "i2c.h"
 
 /*
 PB8-------SCL
 PB9-------SDA
 */
 
 void I2C1_init(void){
	 /*Enable clock access to GPIOB */
	 RCC->AHB1ENR |= GPIOBEN;
	 /*Set PB8 and PB9 mode to alternate function */
	 GPIOB-> MODER &=~ (1U<<16);
	 GPIOB-> MODER |= (1U<<17);
	 
	 GPIOB-> MODER &=~ (1U<<18);
	 GPIOB-> MODER |= (1U<<19);
	 /*Set PB8 and PB9 output type to open drain */
	 GPIOB-> OTYPER |= (1U<<8);
	 GPIOB-> OTYPER |= (1U<<9);
	 /*Enable pullup for PB8 and PB9 */
	 GPIOB-> PUPDR |= (1U<<16);
	 GPIOB-> PUPDR |= (1U<<17);
	 
	 GPIOB-> PUPDR |= (1U<<18);
	 GPIOB-> PUPDR |= (1U<<19);
	 
	 /*alternet function mode*/
	 GPIOB->AFR[1] |= (1<<2);
	 GPIOB->AFR[1] |= (1<<6);
	 
	 
	 /*Enable clock access to I2C1*/
	 RCC->APB1ENR |= I2C1EN;
	 /*Enter reset mode */
	 I2C1->CR1 |=(1U<<15);
	 /*Come out of reset mode */
	 I2C1->CR1 &=~ (1U<<15);
	 /*Set Peripheral clock frequency */
	 I2C1->CR2 =(1U<<4);
	 /*Set I2C to standard mode, 100KHz clock */
	 I2C1->CCR = I2C_100KHZ;
	 /*Set rise time */
	 I2C1->TRISE = SD_MODE_MAX_RISE_TIME;
	 /*Enable i2c1 module */
	 I2C1->CR1 |= CR1_PE;  
			
 }
 void I2C1_byteRead(uint32_t saddr, uint32_t maddr, uint32_t* data){
	 volatile uint32_t tmp;
	 /*Wait until bus not busy*/
	 while(I2C1->SR2 &(SR2_BUSY)){}
	 /* Generate start*/
	 I2C1->CR1 |= CR1_START;
	 /*Wait until start flag is set*/
		while(!(I2C1->SR1 &(SR1_SB))){}
	 /*Transmit slave address+Write*/
		I2C1->DR = saddr << 1;
	 /*Wait until addr flag is set*/
		while(!(I2C1->SR1 &(SR1_ADDR))){}
	 /*Clear addr flag*/
		tmp = I2C1->SR2;
	 /*Send memory address*/
		I2C1->DR= maddr;
	 /*Wait until transmitter empty*/
		while(!(I2C1->SR1 & SR1_TXE)){}
	 /*Generate restart*/
		I2C1->CR1 |= CR1_START;
	 /*Wait until start flag is set*/
		while(!(I2C1->SR1 & SR1_SB)){}
	 /*Transmit slave address + Read*/
		I2C1->DR=saddr<< 1|1;
	 /*Wait until the address flag is set*/
		while(!(I2C1->SR1 & (SR1_ADDR))){}
	 /*Disable Acknowledge*/
		I2C1->CR1 &=~ CR1_ACK;
	/*Clear address flag*/
		tmp=I2C1->SR2;
	/*Generate stop after data received*/
		I2C1->CR1 |= CR1_STOP;
	/*Wait until RXNE flag is set*/
		while(!(I2C1->SR1 & SR1_RXNE)){}
	/*Read data from DR*/
		*data++ = I2C1->DR;
	 
 }
 
 void I2C1_burstRead(uint32_t	 saddr, uint32_t maddr, uint32_t n, uint32_t* data){
	 volatile uint32_t tmp;
	 /*Wait until bus not busy*/
	 while(I2C1->SR2 &(SR2_BUSY)){}
	 /* Generate start*/
	 I2C1->CR1 |= CR1_START;
	 /*Wait until start flag is set*/
		while(!(I2C1->SR1 & SR1_SB)){}
	 /*Transmit slave address+Write*/
		I2C1->DR = saddr << 1;
	 /*Wait until addr flag is set*/
		while(!(I2C1->SR1 & SR1_ADDR)){}
	 /*Clear addr flag*/
		tmp = I2C1-> SR2;
	 /*Send memory address*/
		I2C1->DR=maddr;
	 /*Wait until transmitter empty*/
		while(!(I2C1->SR1 & SR1_TXE)){}
	 /*Generate restart*/
		I2C1->CR1 |= CR1_START;
			
	 /*Wait until start flag is set*/
		while(!(I2C1->SR1 & SR1_SB)){}
	 /*Transmit slave address + Read*/
		I2C1->DR=saddr<< 1|1;
			 
	 /*Wait until the address flag is set*/
		while(!(I2C1->SR1 & SR1_ADDR)){}
	 /*Clear address flag*/
		tmp = I2C1->SR2;
	 /*Disable Acknowledge*/
		I2C1->CR1 &=~ CR1_ACK;
	/*Clear address flag*/
		tmp=I2C1->SR2;
	 /*Enable Acknowledge*/
		I2C1->CR1 |= CR1_ACK;
		while(n > 0U)
		{
			if(n == 1U)
			{
				/*Disable Acknowlede*/
				I2C1->CR1 &=~ CR1_ACK; 
				/*Generate stop*/
				I2C1->CR1 |= CR1_STOP;
				/*Wait for RXNE flag set*/
				while(!(I2C1->SR1 & SR1_RXNE)){}
				/*Read data from DR*/
				*data++ = I2C1->DR;
				break;
			}
			else{
				/*Wait until RXNE Flag is set*/
				while(!(I2C1->SR1 & SR1_RXNE)){}
	     /*read data from DR*/
					(*data++)=I2C1->DR;
					n--;
			}
		}
 }
 
void I2C1_burstWrite(uint32_t saddr, uint32_t maddr, uint32_t n, uint32_t* data){
	uint32_t i;
	volatile uint32_t tmp;
	/*Wait until bus not busy*/
	while(I2C1->SR2 & (SR2_BUSY)){}
		
	/*Generate start*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
		while(!(I2C1->SR1 & (SR1_SB))){}
		
	/*Transmit slave address*/
		I2C1->DR = saddr<<1;
	/*Wait until address flag is set*/
		while(!(I2C1->SR1 & (SR1_ADDR))){}
	/*Clear address flag*/
		tmp = I2C1->SR2;
	/*Wait until data register empty*/
		while(!(I2C1->SR1 & (SR1_TXE))){}
	/*Send memory address*/
		I2C1->DR=maddr;
			
	for(i=0; i<n ;i++){
		/*Wait until data register empty*/
		while(!(I2C1->SR1 & (SR1_TXE))){}
		/*Transmit memory address*/
		I2C1->DR=*data++;
	}
	/*Wait until transfer finished*/
	while(!(I2C1->SR1 & (SR1_BTF))){}
	/*Generate stop*/
	I2C1-> CR1 |= CR1_STOP;
}

