#include "main.h"
#include "string.h"

char A[4];
int main(){
	uart_tx_init();
	while(1){
		
		//S = uart_read();
		UART_Read_String(A);
		UART_Write_String(A);
		if (A[0] == 'A')
		{
			UART_Write_String("Hello ....\r\n");
		}
	}
}