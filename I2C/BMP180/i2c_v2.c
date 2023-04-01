
#include "i2c.h"
//#include "stm32f4xx_hal.h"

//uint16_t value=0;

/*
void I2C2_write_v2(uint8_t DevAddress, uint8_t Mem_Address, uint8_t data)
{
	uint32_t tmpreg = 0x00U;
	  for(int i = 0; i<100; i++){}
		while(I2C2->SR2 & I2C_SR2_BUSY){}	  // Wait until I2C2 peripheral is not busy
	  I2C2->CR1 &= ~I2C_CR1_POS;
			
		//send dev address 
		I2C2->CR1 &= ~I2C_CR1_ACK;
		I2C2->CR1 |= I2C_CR1_START;  // Generate start condition
		while (!(I2C2->SR1 & I2C_SR1_SB)){}  // Wait for start bit to be set

			
    //I2C2->DR = DevAddress<<1; //DevAddress<<1;  // Send address
		I2C2->DR = ((uint8_t)((DevAddress) & (uint8_t)(~I2C_OAR1_ADD0)));
			value = I2C2->DR;	
    while (!(I2C2->SR1 & I2C_SR1_ADDR)){}  // Wait for address sent and ACK received
    while (!(I2C2->SR1 & I2C_SR1_TXE)){}
		
			//tmp= I2C2->SR2;  // Read SR2 to clear ADDR flag
		tmpreg = I2C2->SR1;
			tmpreg = I2C2->SR2;
			while (!(I2C2->SR1 & I2C_SR1_TXE)){}	
			
				//memory address send
    //I2C2->DR = mem_address;  // Send register address
    I2C2->DR = ((uint8_t)((uint16_t)((Mem_Address) & (uint16_t)0x00FF)));	
		while (!(I2C2->SR1 & I2C_SR1_TXE)){}  // Wait for transmit buffer to be empty
		//while (!(I2C2->SR1 & I2C_SR1_BTF)){};
			
			value = 0;
	  value = data;
    I2C2->DR = (uint8_t)data;  // Send data
		while (!(I2C2->SR1 & I2C_SR1_BTF)){}
    //while (!(I2C2->SR1 & I2C_SR1_TXE)){}  // Wait for transmit buffer to be empty
    value = 0;
			value = I2C2->DR;
    I2C2->CR1 |= I2C_CR1_STOP;  // Generate stop condition
}
*/

