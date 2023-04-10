#include "usart.h"

static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate);
static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate);

void uart_tx_init(void);
void uart_write(int ch);
void UART_Write_String(char *p);
void UART_Read_String(char *p_data);

void uart_tx_init(void)
{
//PA10 - rx, PA9 - tx
	//confiure uart gpio pin
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;  // enable gpio clk
	GPIOA->MODER  |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1; // alternet mode of 2 gpio pin 
	USART1->CR1 &= ~USART_CR1_M; //define the word length 8
	USART1->CR2 &= ~USART_CR2_STOP; // stop bit 1
	GPIOA->AFR[1] |= 0x770;
	//configure uart module
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;  //enable clk to uart2
	uart_set_bd(USART1, SYS_FREQ, UART_BR); // config baudrate
	USART1->CR1 = USART_CR1_TE|USART_CR1_RE; //config transfer direction by usart control reg   // M=0, PS=0 not need to code
	USART1->CR1 |=  USART_CR1_UE;  // enable uart   |= or operator  for different from previous        
}

char uart_read(void)
{
	while(!(USART1->SR & USART_SR_RXNE)){}                    // make sure data reg is not empty 
	return USART1->DR ;                                  // write to transmit data register
}


void uart_write(int ch)
{
	while(!(USART1->SR & USART_SR_TXE)){}     // make sure data reg is empty 
	USART1->DR  =  (ch & 0xFF);  // write to transmit data register
}

void UART_Write_String(char *p)
{
	//while(*p!='\0')
	while(*p!='0')
	{
		uart_write(*p);
		p++;
	}
	p=0;
}

void UART_Read_String(char *p_data)
{
	while(*p_data!='0')
	{
		*p_data = uart_read();
		p_data++;
	}
	p_data=0;
}




static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate)   // baudrate calculation
{
	USARTx->BRR = compute_uart_bd(PeriphCLK, BaudRate);
}

static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate)
{
	return ((PeriphCLK + (BaudRate/2))/BaudRate);
}	
