#include "main.h"

char key;
int main (void){
	
	uart3_tx_init();
	
	
	while (1)
	{
		key=uart3_read();
	}
}
