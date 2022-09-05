#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

#define LINE13      (1<< 13)

extern void pc13_exit_init(void);
extern void uart3_tx_init(void);
extern void UART_Write_String(char *p);
extern void EXTI15_10_IRQHandler(void);