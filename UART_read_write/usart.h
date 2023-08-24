#include <stdio.h>
#include <stdint.h>
//#include "stm32f446xx.h"
#include "stm32f4xx.h"                  // Device header


#define SYS_FREQ     16000000
#define UART_BR      9600

void uart_tx_init(void);
void uart_write(int ch);
char uart_read(void);
void UART_Write_String(char *p);
void UART_read_string(char *p_read, uint16_t num_bytes);
void test_setup(void);
