#include <stdio.h>
#include <stdint.h>

extern void UART_Write_String(char *p);
extern void UART_read_string(char *p_read, uint16_t num_bytes);
extern void uart_tx_init(void);

extern char uart_read(void);
extern void test_setup(void);

