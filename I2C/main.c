
#include "main.h"


int16_t x,y,z;
float xg,yg,zg;

int main (void){
	adxl_init();
	
	while(1){
		adxl_read_addr(DATA_Start_Addr);
		
		x = ((data_val[1]<<8)|data_val[0]);
		y = ((data_val[3]<<8)|data_val[2]);
		z = ((data_val[5]<<8)|data_val[4]);
		
		xg = (x *0.0078);
		yg = (y * 0.0078);
		zg = (z * 0.0078);
	}
}
