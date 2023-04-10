#include <stdio.h>
#include <stdint.h>

extern void UART_Write_String(char *p);
extern void uart_tx_init(void);

extern char uart_read(void);
extern void UART_Read_String(char *p_data);
