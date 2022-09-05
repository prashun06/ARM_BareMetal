
#include "stm32f446xx.h"
#include <stdio.h>
#include <stdint.h>

#define GPIOCEN      (1 << 2)
#define SYSCFGEN     (1<< 14)
#define LINE13      (1<< 13)

//uart 


#define UART3EN      (1 << 18)

#define CR1_TE       (1<<3)
#define CR1_UE       (1<<13)
#define SR_TXE       (1<<7)

#define SYS_FREQ     16000000
#define UART_BR      115200