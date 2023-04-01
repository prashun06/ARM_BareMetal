
#include "i2c.h"
//#include "stm32f4xx_hal.h"

uint32_t value=0;
uint32_t *va2;
uint8_t readyflag=1;
void I2C2_read(uint8_t slaveAddress, uint8_t Mem_Address, uint32_t *p_read, uint16_t num_bytes);
void Configure_I2C(void);
void I2C2_write_8b(uint8_t slaveAddress, uint8_t Mem_Address, uint32_t *p_read);
void I2C2_write(uint8_t slaveAddress, uint8_t Mem_Address, uint32_t *p_read, uint16_t num_bytes);
uint8_t I2C2_read_8b(uint8_t slaveAddress, uint8_t Mem_Address);
//void I2C2_write(uint8_t address, uint8_t reg_address, uint8_t data);
void I2C2_read_HAL(uint8_t slaveAddress, uint8_t Mem_Address, uint32_t *p_read, uint16_t num_bytes);

void Configure_I2C(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable clock for GPIOB peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable clock for GPIOC peripheral
	
	GPIOB->MODER |= GPIO_MODER_MODER10_1;
	GPIOB->OTYPER &= ~(1<<10);      
	GPIOB->OTYPER |= (1<<10);             
	GPIOB->PUPDR |=  GPIO_PUPDR_PUPD10_0;  
	GPIOB->OSPEEDR = GPIO_OSPEEDR_OSPEED10_0;

	//configure gpio c
	GPIOC->MODER |=  GPIO_MODER_MODER12_1;    //AF
	GPIOC->OTYPER &= ~(1<<12);                //pushpull
	GPIOC->OTYPER |= (1<<12);                //pushpull
	GPIOC->PUPDR |= GPIO_PUPDR_PUPD12_0;      // pull up
	GPIOC->OSPEEDR= GPIO_OSPEEDR_OSPEED12_0 ;   //Medium speed

	//configure alternet function mode
	GPIOB->AFR[1] |=   GPIO_AFRH_AFRH2_2; //0100:AF4 for PB10
	GPIOC->AFR[1] |=  GPIO_AFRH_AFRH4_2;  //PC12

	//I2C configuration........................................
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

	// reset and clear reg
	I2C2->CR1 |=  I2C_CR1_SWRST;  //software reset
	I2C2->CR1 &= ~I2C_CR1_SWRST;  //software reset
	I2C2->CR1 &= ~I2C_CR1_NOSTRETCH; //disable stretch 
	I2C2->CR1 &= ~I2C_CR1_ENGC;
	I2C2->CR2 |= 16 ;   //0x10=16mhz 
	//I2C standerd mode, 100kZ
	I2C2->CCR =80;      // 0x50; based on cal
	I2C2->TRISE = 17; //0x11 
	I2C2->CR1 |= I2C_CR1_PE; // enable i2c

}

void I2C2_write(uint8_t slaveAddress, uint8_t Mem_Address, uint32_t *p_read, uint16_t num_bytes)
{
	I2C2->CR1 |= I2C_CR1_PE;
	uint32_t tmpreg = 0x00U;
	//for (int i=0;i<10000;i++)
	while(I2C2->SR2 & I2C_SR2_BUSY){}	  // Wait until I2C2 peripheral is not busy
  
	I2C2->CR1 |= I2C_CR1_START;  // Generate start condition
	while (!(I2C2->SR1 & I2C_SR1_SB)){}  // Wait for start bit to be set
	//tmpreg = I2C2->SR1;
		
	I2C2->DR = ((uint8_t)((slaveAddress) & (uint8_t)(~I2C_OAR1_ADD0))); 				
	
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){}  // Wait for address sent and ACK received
	tmpreg = I2C2->SR1;
	tmpreg = I2C2->SR2;
	while (!(I2C2->SR1 & I2C_SR1_TXE)){}	

	//I2C2->CR1 &= ~I2C_CR1_ACK; //disable ack
	I2C2->DR = ((uint8_t)((uint16_t)((Mem_Address) & (uint16_t)0x00FF)));
	//end..................................

	while (!(I2C2->SR1 & I2C_SR1_BTF)){}
	
	while(num_bytes > 0){
		I2C2->CR1 |= I2C_CR1_ACK;
		while (!(I2C2->SR1 & I2C_SR1_TXE)){}	
		I2C2->DR = *p_read;  // Send data
		p_read++;
		num_bytes--;
		while (!(I2C2->SR1 & I2C_SR1_BTF)){}
	}
	I2C2->CR1 |= I2C_CR1_STOP;
	while(I2C2->SR2 & I2C_SR2_BUSY){}	
}

void I2C2_write_8b(uint8_t slaveAddress, uint8_t Mem_Address, uint32_t *p_read)
{
	I2C2->CR1 |= I2C_CR1_PE;
	uint32_t tmpreg = 0x00U;
	//for (int i=0;i<10000;i++)
	while(I2C2->SR2 & I2C_SR2_BUSY){}	  // Wait until I2C2 peripheral is not busy
  
	I2C2->CR1 |= I2C_CR1_START;  // Generate start condition
	while (!(I2C2->SR1 & I2C_SR1_SB)){}  // Wait for start bit to be set
	//tmpreg = I2C2->SR1;
		
	I2C2->DR = ((uint8_t)((slaveAddress) & (uint8_t)(~I2C_OAR1_ADD0))); 				
	
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){}  // Wait for address sent and ACK received
	tmpreg = I2C2->SR1;
	tmpreg = I2C2->SR2;
	while (!(I2C2->SR1 & I2C_SR1_TXE)){}	

	I2C2->CR1 &= ~I2C_CR1_ACK; //disable ack
	I2C2->DR = ((uint8_t)((uint16_t)((Mem_Address) & (uint16_t)0x00FF)));
	//end..................................
		
	while (!(I2C2->SR1 & I2C_SR1_BTF)){}
		
	I2C2->DR = *p_read;  // Send data
	while (!(I2C2->SR1 & I2C_SR1_TXE)){}
	tmpreg = I2C2->SR1;
	tmpreg = I2C2->SR2;
	I2C2->CR1 |= I2C_CR1_STOP;
}


void I2C2_read(uint8_t slaveAddress, uint8_t Mem_Address, uint32_t *p_read, uint16_t num_bytes)
{	
	I2C2->CR1 |= I2C_CR1_PE;
	uint32_t tmpreg = 0x00U;
	//for (int i=0;i<10000;i++)
	while(I2C2->SR2 & I2C_SR2_BUSY){}	  // Wait until I2C2 peripheral is not busy
//I2C_CR1_POS
	I2C2->CR1 |= I2C_CR1_START;  // Generate start condition
	while (!(I2C2->SR1 & I2C_SR1_SB)){}  // Wait for start bit to be set
	tmpreg = I2C2->SR1;
		
	I2C2->DR = ((uint8_t)((slaveAddress) & (uint8_t)(~I2C_OAR1_ADD0))); 				
	
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){}  // Wait for address sent and ACK received
	tmpreg = I2C2->SR1;
	tmpreg = I2C2->SR2;
	while (!(I2C2->SR1 & I2C_SR1_TXE)){}	

	if(num_bytes >=2){
		I2C2->CR1 |= I2C_CR1_ACK; //enable ack
		I2C2->DR = ((uint8_t)((uint16_t)((Mem_Address) & (uint16_t)0x00FF)));	
	}
	else
	{
		I2C2->CR1 &= ~I2C_CR1_ACK; //disable ack
		I2C2->DR = ((uint8_t)((uint16_t)((Mem_Address) & (uint16_t)0x00FF)));
	}
	
	while (!(I2C2->SR1 & I2C_SR1_BTF)){}
		
	I2C2->CR1 |= I2C_CR1_START;  // Generate restart condition
	while (!(I2C2->SR1 & I2C_SR1_SB)){}  // Wait for restart bit to be set
	
	tmpreg = I2C2->SR1;
	I2C2->DR =  ((uint8_t)((slaveAddress) | I2C_OAR1_ADD0));
		
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){}  // Wait for address sent and ACK received
	//.....................end 
	/*
	if(num_bytes >=2){
		I2C2->CR1 |= I2C_CR1_ACK; //enable ack
		I2C2->DR = Mem_Address;	
	}
	else
	{
		I2C2->CR1 &= ~I2C_CR1_ACK; //disable ack
		I2C2->DR = Mem_Address;
	}
	tmpreg = I2C2->SR1;
	tmpreg = I2C2->SR2;*/

		tmpreg = I2C2->SR1;
	tmpreg = I2C2->SR2;
		
		
	while(num_bytes >0){
		if(num_bytes >=2){
			I2C2->CR1 |= I2C_CR1_ACK;
			while(!(I2C2->SR1 & I2C_SR1_RXNE)){}
			*p_read = I2C2->DR;
			p_read++;
			num_bytes--;

		}
		else{
			I2C2->CR1 &= ~I2C_CR1_ACK;
			while(!(I2C2->SR1 & I2C_SR1_RXNE)){}
			*p_read = I2C2->DR;
			//p_read++;
			num_bytes--;
			I2C2->CR1 |= I2C_CR1_STOP;
				
		}
	}
	//I2C2->CR1 &= ~I2C_CR1_PE;
}

uint8_t I2C2_read_8b(uint8_t slaveAddress, uint8_t Mem_Address)
{	
	uint32_t tmpreg = 0x00U;
	//for (int i=0;i<10000;i++)
	while(I2C2->SR2 & I2C_SR2_BUSY){}	  // Wait until I2C2 peripheral is not busy

	I2C2->CR1 |= I2C_CR1_START;  // Generate start condition
	while (!(I2C2->SR1 & I2C_SR1_SB)){}  // Wait for start bit to be set
	tmpreg = I2C2->SR1;
		
	I2C2->DR = ((uint8_t)((slaveAddress) & (uint8_t)(~I2C_OAR1_ADD0))); 				
	
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){}  // Wait for address sent and ACK received
	tmpreg = I2C2->SR1;
	tmpreg = I2C2->SR2;
	while (!(I2C2->SR1 & I2C_SR1_TXE)){}	

	//I2C2->CR1 &= ~I2C_CR1_ACK; //disable ack
	I2C2->DR = Mem_Address;
	
	
	while (!(I2C2->SR1 & I2C_SR1_BTF)){}
		
	I2C2->CR1 |= I2C_CR1_START;  // Generate restart condition
	while (!(I2C2->SR1 & I2C_SR1_SB)){}  // Wait for restart bit to be set
	
	tmpreg = I2C2->SR1;
	I2C2->DR =  ((uint8_t)((slaveAddress) | I2C_OAR1_ADD0));
		
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){}  // Wait for address sent and ACK received
	tmpreg = I2C2->SR1;
	tmpreg = I2C2->SR2;
		I2C2->CR1 &= ~I2C_CR1_ACK;
	while(!(I2C2->SR1 & I2C_SR1_RXNE)){}
	uint8_t p_data = I2C2->DR;
	I2C2->CR1 |= I2C_CR1_STOP;

	return p_data;
}

uint16_t num_bytes;
void I2C2_read_HAL(uint8_t slaveAddress, uint8_t Mem_Address, uint32_t *p_read, uint16_t num_bytes)
{	
	//I2C2->CR1 |= I2C_CR1_PE;
	uint32_t tmpreg = 0x00U;
	//for (int i=0;i<10000;i++)
	
	while(I2C2->SR2 & I2C_SR2_BUSY){}	  // Wait until I2C2 peripheral is not busy
  I2C2->CR1 |= I2C_CR1_POS; 
		
	I2C2->CR1 |= I2C_CR1_ACK; 
	I2C2->CR1 |= I2C_CR1_START;  // Generate start condition
	while (!(I2C2->SR1 & I2C_SR1_SB)){}  // Wait for start bit to be set
	
	I2C2->DR = ((uint8_t)((slaveAddress) & (uint8_t)(~I2C_OAR1_ADD0))); 				
	
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){}  // Wait for address sent and ACK received
	tmpreg = I2C2->SR1;
	tmpreg = I2C2->SR2;
	while (!(I2C2->SR1 & I2C_SR1_TXE)){}	

	I2C2->DR = ((uint8_t)((uint16_t)((Mem_Address) & (uint16_t)0x00FF)));
	I2C2->DR = ((uint8_t)((uint16_t)((Mem_Address) & (uint16_t)0x00FF)));		
	while (!(I2C2->SR1 & I2C_SR1_TXE)){}
		
	//while (!(I2C2->SR1 & I2C_SR1_BTF)){}
		
	I2C2->CR1 |= I2C_CR1_START;  // Generate restart condition
	while (!(I2C2->SR1 & I2C_SR1_SB)){}  // Wait for restart bit to be set
	
	tmpreg = I2C2->SR1;
	tmpreg = I2C2->SR2;
	I2C2->DR =  ((uint8_t)((slaveAddress) | I2C_OAR1_ADD0));
		
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){}  // Wait for address sent and ACK received
//end.................
		
		if(num_bytes==0)
		{
			tmpreg = I2C2->SR1;
			tmpreg = I2C2->SR2;
			I2C2->CR1 |= I2C_CR1_STOP;
		}
		else if(num_bytes==1)
		{
			I2C2->CR1 &= ~I2C_CR1_ACK; 
			tmpreg = I2C2->SR1;
			tmpreg = I2C2->SR2;
			I2C2->CR1 |= I2C_CR1_STOP;
		}
		else if(num_bytes==2)
		{
			I2C2->CR1 &= ~I2C_CR1_ACK; 
			I2C2->CR1 |= I2C_CR1_POS; 
			tmpreg = I2C2->SR1;
			tmpreg = I2C2->SR2;
			I2C2->CR1 |= I2C_CR1_STOP;
		}
		else
		{
			tmpreg = I2C2->SR1;
			tmpreg = I2C2->SR2;
			
		}
	
		while(num_bytes >0){
			if(num_bytes <=3){
				if(num_bytes ==1)  //1 byte
				{
				while(!(I2C2->SR1 & I2C_SR1_RXNE)){}
				*p_read = I2C2->DR;
				p_read++;
				num_bytes--;
					//I2C2->CR1 |= I2C_CR1_STOP;
				}
				else if(num_bytes ==2)  //2 bytes
				{
					while(!(I2C2->SR1 & I2C_SR1_BTF)){}
					I2C2->CR1 |= I2C_CR1_STOP;
					*p_read = I2C2->DR;
					p_read++;
					num_bytes--;
						
					*p_read = I2C2->DR;
					p_read++;
					num_bytes--;
				}
				else   //3 bytes
				{
					while(!(I2C2->SR1 & I2C_SR1_BTF)){}
					I2C2->CR1 &= ~I2C_CR1_ACK; 
					*p_read = I2C2->DR;
					p_read++;
					num_bytes--;
						
					while(!(I2C2->SR1 & I2C_SR1_BTF)){}
					I2C2->CR1 |= I2C_CR1_STOP;
					*p_read = I2C2->DR;
					p_read++;
					num_bytes--;
					*p_read = I2C2->DR;
					p_read++;
					num_bytes--;		
				}
			}
			else{
				while(!(I2C2->SR1 & I2C_SR1_RXNE)){}
				*p_read = I2C2->DR;
				p_read++;
				num_bytes--;	
				while(I2C2->SR1 & I2C_SR1_BTF){
					*p_read = I2C2->DR;
					p_read++;
					num_bytes--;	
					}				
			}
		}
		//readyflag =1;
}