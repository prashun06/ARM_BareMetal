
#include "i2c.h"
#include "stdint.h"

#define Device_ID_R         0x00
#define Device_address_R  0x53
#define Data_Format_R     0x31
#define Power_Ctrl_R      0x2D
#define DATA_Start_Addr   0x32

#define Four_G            0x01
#define Reset             0x00
#define Set_Measure_B     0x08  
 
void adxl_init(void);
void adxl_read_multivalue(uint32_t reg);
void adxl_write(uint32_t reg, uint32_t value);
void adxl_read_addr(uint32_t reg);


