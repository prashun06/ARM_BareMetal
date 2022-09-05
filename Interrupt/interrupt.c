
#include "intrrupt.h"
//#include "exti.h"

int main (void){
	
	pc13_exit_init();
	uart3_tx_init();
	while (1)
	{
		EXTI15_10_IRQHandler();
	}
	
}

