
#include "exti.h"

static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate);
static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate);

void uart3_tx_init(void);
void uart3_write(int ch);
void UART_Write_String(char *p);
//int __io_putchar(int ch);



//interrupt function........................................
void pc13_exit_init(void)
{
	//disable global interrupts
	__disable_irq();
	//enable the rcc clock 
	RCC->AHB1ENR |= GPIOCEN;
	//set PC13 as input 
	// GPIOC->MODER  as input is 00
	
	// enable clock acess of SYSCFGEN
	RCC->APB2ENR |= SYSCFGEN;
	
	//select portc as EXTI13
	SYSCFG->EXTICR[3] |= (1<<5);  //PC 0010 
	
	//unmask EXTI
	EXTI->IMR |= (1<<13);
	
	//select falling edge triger 
	EXTI->FTSR |= (1<<13);
	
	//enable EXTI13 line in NVIC 
	NVIC_EnableIRQ(EXTI15_10_IRQn);  //External Line[15:10] Interrupts = 40
	
	//enable global interrupt 
	__enable_irq();
}

void EXTI15_10_IRQHandler(void)  // inside the vector table naming //interrupt req handler
{
	if ((EXTI->PR & LINE13)!=0)  //when iturrpt occured 
	{
		//clear the PR flag 
		EXTI->PR |= LINE13;
		//print the dialogue
		UART_Write_String("Interrupt executed....\r\n");
		
	}
}
//interrupt function......................................


//uart send 

void uart3_tx_init(void)
{

	//confiure uart gpio pin
	RCC->AHB1ENR  |= GPIOCEN;  // enable gpio clk
	GPIOC->MODER  |= (1<<21);  // alternet mode of gpio pin
	GPIOC->MODER  |= (1<<23);
	
	GPIOC->AFR[1] |= (1<<8); // PC12 alternet func AFRL2 type to uart_tx AF7
	GPIOC->AFR[1] |= (1<<9);
	GPIOC->AFR[1] |= (1<<10);
	
	GPIOC->AFR[1] |= (1<<12); // PA2 alternet func AFRL2 type to uart_tx AF7
	GPIOC->AFR[1] |= (1<<13);
	GPIOC->AFR[1] |= (1<<14);
	
	
	//GPIOA->AFR[1] |= (1<<6);
	
	//configure uart module
	RCC->APB1ENR |= UART3EN;  //enable clk to uart2
	uart_set_bd(USART3, SYS_FREQ, UART_BR); // config baudrate
	USART3->CR1 = CR1_TE; //config transfer direction by usart control reg   // M=0, PS=0 not need to code
	USART3->CR1 |=  CR1_UE;  // enable uart   |= or operator  for different from previous        
}

void uart3_write(int ch)
{
	while(!(USART3->SR & SR_TXE)){}     // make sure data reg is empty 
	USART3->DR  =  (ch & 0xFF);  // write to transmit data register
}

void UART_Write_String(char *p)
	{
	while(*p!='\0')
{
uart3_write(*p);
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
