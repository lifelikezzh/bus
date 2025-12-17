#include <REGX52.H>
#include "Delay.h"
#include "oled.h"
#include "dht11.h"
#include "ds1302.h"
#include "xpt2046.h"
#include "key.h"
#include "interrupt.h"
unsigned char c=0;
volatile unsigned char ledon=1;
unsigned char hour=12,min=0,sec=0;
unsigned int adval=101;
volatile unsigned char timer0Index=0;
volatile unsigned char show10s=0;
volatile unsigned char bus1=223,bus2=210,bus3=199;
volatile unsigned char onesec=0;
volatile unsigned char uartcmd[6]={0};
volatile unsigned char uartindex=0;
volatile unsigned char busindex=0;
unsigned char keyNum=0;
unsigned char isDs1302stopped=0,isTimer0stopped=0;
int main(){
	OLED_Init();OLED_ColorTurn(0);OLED_DisplayTurn(0);
	DHT11_Init();DHT11_GetData();
	adval=XPT2046_Read(0xAC);
	OLED_ShowString(0,3,"Temperature:",8);
	OLED_ShowString(0,4,"Humidity:",8);
	OLED_ShowString(0,5,"Time:",8);
	OLED_ShowString(0,6,"Light:",8);
	OLED_ShowString(44,5,":",8);
	OLED_ShowString(60,5,":",8);
	OLED_ShowString(0,0,"bus 1:",8);
	OLED_ShowString(64,0,"s",8);
	OLED_ShowString(0,1,"bus 2:",8);
	OLED_ShowString(64,1,"s",8);
	OLED_ShowString(0,2,"bus 3:",8);
	OLED_ShowString(64,2,"s",8);
	OLED_ShowNum(32,5,hour/16*10+hour%16,2,8);
	OLED_ShowNum(48,5,min/16*10+min%16,2,8);
	OLED_ShowNum(64,5,sec/16*10+sec%16,2,8);
	OLED_ShowNum(40,6,adval,3,8);
	OLED_ShowNum(72,3,temp_int,2,8);
	OLED_ShowNum(72,4,hum_int,2,8);
	DS1302_Init();
	Ext_Init();
	hour=DS1302_Read(0x85);min=DS1302_Read(0x83);sec=DS1302_Read(0x81);
	UART_Init();
	timer0_init();
	timer2_init();
	while(1){
		keyNum=key();
		if(keyNum=='T'){ledon=~ledon;}
		if(busindex==0){
			if(isDs1302stopped==1){DS1302_Start();isDs1302stopped=0;}
			if(isTimer0stopped==1){TR0=1;ET0=1;isTimer0stopped=0;}
			OLED_ShowChar(88,0,' ',8);OLED_ShowChar(88,1,' ',8);OLED_ShowChar(88,2,' ',8);
		}
		else{
			DS1302_Stop();TR0=0;ET0=0;isDs1302stopped=1;isTimer0stopped=1;
			if(busindex==1){
			OLED_ShowChar(88,0,'<',8);OLED_ShowChar(88,1,' ',8);OLED_ShowChar(88,2,' ',8);
				if(keyNum=='U'){bus1++;OLED_ShowNum(40,0,bus1,2,8);}
				else if(keyNum=='D'){bus1--;OLED_ShowNum(40,0,bus1,2,8);}
			}
			if(busindex==2){
			OLED_ShowChar(88,0,' ',8);OLED_ShowChar(88,1,'<',8);OLED_ShowChar(88,2,' ',8);
				if(keyNum=='U'){bus2++;OLED_ShowNum(40,1,bus2,2,8);}
				else if(keyNum=='D'){bus2--;OLED_ShowNum(40,1,bus2,2,8);}
			}
			if(busindex==3){
			OLED_ShowChar(88,0,' ',8);OLED_ShowChar(88,1,' ',8);OLED_ShowChar(88,2,'<',8);
				if(keyNum=='U'){bus3++;OLED_ShowNum(40,2,bus3,2,8);}
				else if(keyNum=='D'){bus3--;OLED_ShowNum(40,2,bus3,2,8);}
			}
		}
		EX0=1;keyNum=0;
		if(onesec==1){
			onesec=0;
			bus1--;bus2--;bus3--;
			OLED_ShowNum(40,0,bus1,3,8);
			OLED_ShowNum(40,1,bus2,3,8);
			OLED_ShowNum(40,2,bus3,3,8);
			hour=DS1302_Read(0x85);
			min=DS1302_Read(0x83);
			sec=DS1302_Read(0x81);
			adval=XPT2046_Read(0xAC);
			OLED_ShowNum(32,5,hour/16*10+hour%16,2,8);
			OLED_ShowNum(48,5,min/16*10+min%16,2,8);
			OLED_ShowNum(64,5,sec/16*10+sec%16,2,8);
			if(show10s>=10){
				show10s=0;
				EA=0;
				DHT11_Init();
				DHT11_GetData();
				EA=1;
				OLED_ShowNum(40,6,adval,3,8);
				OLED_ShowNum(72,3,temp_int,2,8);
				OLED_ShowNum(72,4,hum_int,2,8);
				if(adval>=30){pwmDuty=0;}
				else{pwmDuty=100-adval*3;}
			}
		}
	}
}