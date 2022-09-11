#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h" 

#define GPIOAEN      (1 << 0)
#define GPIOCEN      (1 << 2)

#define UART3EN      (1 << 18)

#define CR1_TE       (1<<3)
#define CR1_RE       (1<<2)
#define SR_RXNE       (1<<5)

#define CR1_UE       (1<<13)


#define CR1_RXNEIE   (1<< 5)

#define SYS_FREQ     16000000
#define UART_BR      115200
