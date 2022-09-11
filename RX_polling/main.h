#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

extern void uart3_write (int ch);
extern void uart3_tx_init(void);
extern void UART_Write_String(char *p);
