  
#include "main.h"
//#include "bmp180.h"
//#include "i2c.h"


extern bmp_t bmp;
 
 
int main(){
Configure_I2C();
bmp_init (&bmp);
while (1)
  {
			bmp.uncomp.temp = get_ut ();
			bmp.data.temp = get_temp (&bmp);
			bmp.uncomp.press = get_up (bmp.oss); 
			bmp.data.press = get_pressure (bmp)/100;   // Nm2  1 atmospheres = 1.013x105 N/m-2
			bmp.data.altitude = get_altitude (&bmp);
		
		for(int i = 0; i<80000; i++){}
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
