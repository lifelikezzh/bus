#include <REGX52.H>
#include "Delay.h"
sbit DHT11_DATA=P1^3;
unsigned char hum_int;
unsigned char hum_dec;
unsigned char temp_int;
unsigned char temp_dec;
unsigned char verify;
void DHT11_Init(){
	DHT11_DATA=0;
	Delay(20);
	DHT11_DATA=1;
	Delay45us();
	if(DHT11_DATA==0){
		while(DHT11_DATA==0);
		while(DHT11_DATA==1);
	}
}
unsigned char Byte_Read(){
	unsigned char i,dat=0;
	for(i=0;i<8;i++){
		while(DHT11_DATA==0);
		Delay45us();
		dat<<=1;
		if(DHT11_DATA==1){dat|=1;while(DHT11_DATA==1);}
	}
	return dat;
}
unsigned char DHT11_GetData(){
	unsigned char t_hi,t_hd,t_ti,t_td;
	t_hi=Byte_Read();
	t_hd=Byte_Read();
	t_ti=Byte_Read();
	t_td=Byte_Read();
	verify=Byte_Read();
	Delay45us();
	if(t_hi+t_hd+t_ti+t_td==verify){
		hum_int=t_hi;
		hum_dec=t_hd;
		temp_int=t_ti;
		temp_dec=t_td;
		return 1;
	}return 0;
}
