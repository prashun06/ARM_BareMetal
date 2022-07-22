#include <stdio.h>
#include <stdint.h>
#include "stm32f407xx.h"


#define GPIOAEN      (1 << 0)
#define UART1EN      (1 << 4)

#define CR1_TE       (1<<3)
#define CR1_RE       (1<<2)

#define CR1_UE       (1<<13)

#define SR_TXE       (1<<7)
#define SR_RXNE       (1<<5)

#define SYS_FREQ     16000000
#define UART_BR      115200



static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate);
static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate);

void uart1_rxtx_init(void);

char uart1_read(void);



int main()
{
	
	char led;
	GPIOA->MODER  |= (1<<10);
	
	uart1_rxtx_init();
	
	while(1)
	{
		
		led = uart1_read();
		if (led == 'k'){
			GPIOA->ODR |= (0<<5);
		}
		else{
			GPIOA->ODR |= (1<<5);
		}
	}
}


void uart1_rxtx_init(void)
{
	//confiure uart gpio pin
	RCC->AHB1ENR  |= GPIOAEN;                   // enable gpio clk
	GPIOA->MODER  |= (1<<19);                  // alternet mode of gpio pin
	
	GPIOA->AFR[1] |= (1<<4);                 // PA2 alternet func AFRL2 type to uart_tx AF7
	GPIOA->AFR[1] |= (1<<5);
	GPIOA->AFR[1] |= (1<<6);
	
	//configure uart module
	RCC->APB2ENR |= UART1EN;                    //enable clk to uart2
	uart_set_bd(USART1, SYS_FREQ, UART_BR);      // config baudrate
	USART1->CR1 = (CR1_TE | CR1_RE);            //config transfer direction by usart control reg   // M=0, PS=0 not need to code
	USART1->CR1 |=  CR1_UE;                    // enable uart   |= or operator  for different from previous        
	
	
	
	//confiure uart gpio pin
	RCC->AHB1ENR  |= GPIOAEN;                  // enable gpioa clk
	GPIOA->MODER  |= (1<<21);                 // alternet mode of gpio pin2
	
	GPIOA->AFR[1] |= (1<<4);                // PA2 alternet func AFRL2 type to uart_tx AF7 0111
	GPIOA->AFR[1] |= (1<<5);
	GPIOA->AFR[1] |= (1<<6);
}



char uart1_read(void)
{
	while(!(USART1->SR & SR_RXNE)){}                    // make sure data reg is not empty 
	return USART1->DR;                                  // write to transmit data register
}


static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate)   // baudrate calculation
{
	USARTx->BRR = compute_uart_bd(PeriphCLK, BaudRate);
}

static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate)
{
	return ((PeriphCLK + (BaudRate/2))/BaudRate);
}	
	 
