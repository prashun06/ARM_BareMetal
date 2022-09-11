#include "rx3.h"

static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate);
static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate);

char key;
void uart3_tx_init(void);

void uart3_tx_init(void)
{
	RCC->AHB1ENR  |=  GPIOAEN;
	GPIOA->MODER  |= (1<<10);  //PA5 pin 

	//confiure uart gpio pin
	RCC->AHB1ENR  |= GPIOCEN ;  // enable gpio clk
	GPIOC->MODER  |= (1<<21);  // alternet mode of gpio pin
	GPIOC->MODER  |= (1<<23);
	
	GPIOC->AFR[1] |= (1<<8); // PC10 alternet func AFRL2 type to uart_rx AF7
	GPIOC->AFR[1] |= (1<<9);
	GPIOC->AFR[1] |= (1<<10);
	
	GPIOC->AFR[1] |= (1<<12); // PC11 alternet func AFRL2 type to uart_tx AF7
	GPIOC->AFR[1] |= (1<<13);
	GPIOC->AFR[1] |= (1<<14);
	
	
	//configure uart module
	RCC->APB1ENR |= UART3EN;  //enable clk to uart2
	uart_set_bd(USART3, SYS_FREQ, UART_BR); // config baudrate
	USART3->CR1 = (CR1_TE | CR1_RE); //config transfer direction by usart control reg   // M=0, PS=0 not need to code
	USART3->CR1 |= CR1_RXNEIE; // enable the receive inturrupt reg 
	NVIC_EnableIRQ(USART3_IRQn);  // NVIC reg enable ffor USART3 = 39
	USART3->CR1 |=  CR1_UE;  // enable uart   |= or operator  for different from previous    
	
}

void USART3_IRQHandler(void){
	if(USART3->SR & SR_RXNE){  //IRQ handler processe 
		key = USART3->DR;
		if (key == '1'){
			GPIOA->BSRR |= (1<<5);
		}
		else{
			GPIOA->BSRR |= (1<<21);
		}
		
	} // save the data in key variable 
}

static void uart_set_bd(USART_TypeDef *USARTx, uint32_t PeriphCLK, uint32_t BaudRate)   // baudrate calculation
{
	USARTx->BRR = compute_uart_bd(PeriphCLK, BaudRate);
}

static uint32_t compute_uart_bd (uint32_t PeriphCLK, uint32_t BaudRate)
{
	return ((PeriphCLK + (BaudRate/2))/BaudRate);
}	
