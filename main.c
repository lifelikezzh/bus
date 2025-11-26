#include <REGX52.H>
#include "Delay.h"
#include "oled.h"
#include "dht11.h"
int i=0;
unsigned char c=0;
int main(){
	OLED_Init();
	OLED_ColorTurn(0);
	OLED_DisplayTurn(0);
	OLED_ShowString(0,3,"Temperature:",8);
	OLED_ShowString(0,4,"Humidity:",8);
	OLED_ShowString(0,5,"Time:10:21:44",8);
	DHT11_Init();
	DHT11_GetData();
	Delay(2000);
	while(1){
		OLED_ShowString(0,0,"bus 1:",8);
		OLED_ShowNum(40,0,i,2,8);
		OLED_ShowString(64,0,"min",8);
		OLED_ShowString(0,1,"bus 2:",8);
		OLED_ShowNum(40,1,i,2,8);
		OLED_ShowString(64,1,"min",8);
		OLED_ShowString(0,2,"bus 3:",8);
		OLED_ShowNum(40,2,i,2,8);
		OLED_ShowString(64,2,"min",8);
		DHT11_Init();
		if(DHT11_GetData()==1){
			OLED_ShowNum(72,3,temp_int,2,8);
			OLED_ShowNum(72,4,hum_int,2,8);
		}
		i++;
		Delay(2000);
	}
}