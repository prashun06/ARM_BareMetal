#include "main.h"
#include "string.h"

char S[100];
int main(){
	uart_tx_init();
	test_setup();
	while(1){
		
		//UART_read_string((char*)S, 10);
		//S = uart3_read();
		

			//UART_Write_String("Hello ....\r\n");
		
	}
}
