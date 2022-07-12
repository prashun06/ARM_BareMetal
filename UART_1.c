

#include <stdint.h>
#include "stm32f407xx.h"


#define GPIODEN      (1 << 3)
#define UART2EN      (1 << 17)

#define CR1_TE       (1<<3)
#define CR1_UE       (1<<13)
#define SR_TXE       (1<<7)

#define SYS_FREQ     16000000
#define UART_BR      115200


static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate);
static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate);
void uart2_tx_init(void);
void uart2_write(int ch);


int main()
{
	uart2_tx_init();
	while(1)
	{
		uart2_write('Y');
	}
}


void uart2_tx_init(void)
{
	//confiure uart gpio pin
	RCC->AHB1ENR  |= GPIODEN;  // enable gpio clk
	GPIOA->MODER  |= (1<<5);  // alternet mode of gpio pin
	
	GPIOA->AFR[0] |= (1<<8); // PA2 alternet func AFRL2 type to uart_tx AF7
	GPIOA->AFR[0] |= (1<<9);
	GPIOA->AFR[0] |= (1<<10);
	
	//configure uart module
	RCC->APB1ENR |= UART2EN;  //enable clk to uart2
	uart_set_bd(USART2, SYS_FREQ, UART_BR); // config baudrate
	USART1->CR1 = CR1_TE; //config transfer direction by usart control reg   // M=0, PS=0 not need to code
	USART1->CR1 |=  CR1_UE;  // enable uart   |= or operator  for different from previous        
}

void uart2_write(int ch)
{
	while(!(USART2->SR & SR_TXE)){}     // make sure data reg is empty 
	USART2->DR  =  (ch & 0xFF);  // write to transmit data register
}

static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate)   // baudrate calculation
{
	USARTx->BRR = compute_uart_bd(PeriphCLK, BaudRate);
}

static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate)
{
	return ((PeriphCLK + (BaudRate/2U))/BaudRate);
}	
	 
