#include <stdio.h>
#include <stdint.h>
#include "stm32f407xx.h"


#define GPIOAEN      (1 << 0)
#define UART1EN      (1 << 4)

#define CR1_TE       (1<<3)
#define CR1_UE       (1<<13)
#define SR_TXE       (1<<7)

#define SYS_FREQ     16000000
#define UART_BR      115200


static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate);
static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate);

void uart1_tx_init(void);
void uart1_write(int ch);
void UART_Write_String(char *p);
//int __io_putchar(int ch);



int main()
{
	uart1_tx_init();
	while(1)
	{
		//printf("Hello Prashun.........\r\n");
		UART_Write_String("Hello Prashun....\r\n");
	}
}


void uart1_tx_init(void)
{
	//confiure uart gpio pin
	RCC->AHB1ENR  |= GPIOAEN;  // enable gpio clk
	GPIOA->MODER  |= (1<<19);  // alternet mode of gpio pin
	
	GPIOA->AFR[1] |= (1<<4); // PA2 alternet func AFRL2 type to uart_tx AF7
	GPIOA->AFR[1] |= (1<<5);
	GPIOA->AFR[1] |= (1<<6);
	
	//configure uart module
	RCC->APB2ENR |= UART1EN;  //enable clk to uart2
	uart_set_bd(USART1, SYS_FREQ, UART_BR); // config baudrate
	USART1->CR1 = CR1_TE; //config transfer direction by usart control reg   // M=0, PS=0 not need to code
	USART1->CR1 |=  CR1_UE;  // enable uart   |= or operator  for different from previous        
}

void uart1_write(int ch)
{
	while(!(USART1->SR & SR_TXE)){}     // make sure data reg is empty 
	USART1->DR  =  (ch & 0xFF);  // write to transmit data register
}

void UART_Write_String(char *p)
	{
	while(*p!='\0')
{
uart1_write(*p);
p++;
}
}

/*int __io_putchar(int ch)
{
	uart1_write(ch);
	return ch;
}*/

static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate)   // baudrate calculation
{
	USARTx->BRR = compute_uart_bd(PeriphCLK, BaudRate);
}

static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate)
{
	return ((PeriphCLK + (BaudRate/2))/BaudRate);
}	
	 
