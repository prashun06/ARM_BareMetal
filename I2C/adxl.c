
#include "adxl.h"

uint32_t data[2];
uint32_t data_val[6];

void adxl_read_addr(uint32_t reg){
	I2C1_byteRead(Device_address_R, reg, &data);
}
void adxl_write(uint32_t reg, uint32_t value){
	
	/*uint32_t data[1];*/
	data[0]=value;
	
	I2C1_burstWrite(Device_address_R, reg, 1, data);
}

void adxl_read_multivalue(uint32_t reg){
	I2C1_burstRead(Device_address_R, reg, 6, data_val);
}
void adxl_init(void){
	I2C1_init();
	/*read dev_id, return 0xe5 = everything good*/
	adxl_read_addr(Device_ID_R);
	/*set data format range +-4g*/
	adxl_write(Data_Format_R, Four_G );
	/*reset all bits*/
	adxl_write(Power_Ctrl_R, Reset);
	/*configure power control measure bit*/ 
	adxl_write(Power_Ctrl_R, Set_Measure_B );
}
