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
volatile unsigned int bus1=223,bus2=210,bus3=199;
volatile unsigned char onesec=0;
volatile unsigned char uartcmd[6]={0};
volatile unsigned char uartindex=0;
volatile unsigned char busindex=0;
unsigned char keyNum=0;
unsigned char isDs1302stopped=0,isTimer0stopped=0;
volatile unsigned char bus1near=0,bus2near=0,bus3near=0;
unsigned char beepcnt=0;
int main(){
	OLED_Init();OLED_ColorTurn(0);OLED_DisplayTurn(0);
	DHT11_Init();DHT11_GetData();
	adval=XPT2046_Read(0xAC);
	OLED_ShowString(32,0,"  :  :  ",16);
	OLED_ShowNum(32,0,(hour/16*10+hour%16)/10,1,16);
	OLED_ShowNum(40,0,(hour/16*10+hour%16)%10,1,16);
	OLED_ShowNum(56,0,(min/16*10+min%16)/10,1,16);
	OLED_ShowNum(64,0,(min/16*10+min%16)%10,1,16);
	OLED_ShowNum(80,0,(sec/16*10+sec%16)/10,1,16);
	OLED_ShowNum(88,0,(sec/16*10+sec%16)%10,1,16);
	OLED_ShowString(0,2,"=====================",8);
	OLED_ShowString(0,3,"BUS 1:",8);
	OLED_ShowNum(42,3,bus1/60/10,1,8);
	OLED_ShowNum(48,3,bus1/60%10,1,8);
	OLED_ShowString(55,3,":",8);
	OLED_ShowNum(60,3,bus1%60/10,1,8);
	OLED_ShowNum(66,3,bus1%60%10,1,8);
	OLED_ShowString(0,4,"BUS 2:",8);
	OLED_ShowNum(42,4,bus2/60/10,1,8);
	OLED_ShowNum(48,4,bus2/60%10,1,8);
	OLED_ShowString(55,4,":",8);
	OLED_ShowNum(60,4,bus2%60/10,1,8);
	OLED_ShowNum(66,4,bus2%60%10,1,8);
	OLED_ShowString(0,5,"BUS 3:",8);
	OLED_ShowNum(42,5,bus3/60/10,1,8);
	OLED_ShowNum(48,5,bus3/60%10,1,8);
	OLED_ShowString(55,5,":",8);
	OLED_ShowNum(60,5,bus3%60/10,1,8);
	OLED_ShowNum(66,5,bus3%60%10,1,8);
	OLED_ShowString(0,6,"=====================",8);
	OLED_ShowString(0,7,"T:  C  L:   %  H:  %",8);
	OLED_ShowNum(12,7,temp_int,2,8);
	OLED_ShowNum(52,7,(adval*5+1)/3,3,8);
	OLED_ShowNum(100,7,hum_int,2,8);
	DS1302_Init();
	Ext_Init();
	hour=DS1302_Read(0x85);min=DS1302_Read(0x83);sec=DS1302_Read(0x81);
	UART_Init();
	timer0_init();
	timer2_init();
	while(1){
		keyNum=key();
		UART_main();
		if(P1_5==0){beepcnt++;if(beepcnt>=3){P1_5=1;beepcnt=0;}}
		if(bus1<=180){OLED_ShowChar(72,3,'*',8);}else{OLED_ShowChar(72,3,' ',8);}
		if(bus2<=180){OLED_ShowChar(72,4,'*',8);}else{OLED_ShowChar(72,4,' ',8);}
		if(bus3<=180){OLED_ShowChar(72,5,'*',8);}else{OLED_ShowChar(72,5,' ',8);}
		if(keyNum=='T'){ledon=~ledon;}
		if(busindex==0){
			if(isDs1302stopped==1){DS1302_Start();isDs1302stopped=0;}
			if(isTimer0stopped==1){TR0=1;ET0=1;isTimer0stopped=0;}
			OLED_ShowChar(78,3,' ',8);OLED_ShowChar(78,4,' ',8);OLED_ShowChar(78,5,' ',8);
		}
		else{
			DS1302_Stop();TR0=0;ET0=0;isDs1302stopped=1;isTimer0stopped=1;
			if(busindex==1){
			OLED_ShowChar(78,3,'<',8);OLED_ShowChar(78,4,' ',8);OLED_ShowChar(78,5,' ',8);
				if(keyNum=='U'){bus1++;OLED_ShowNum(42,3,bus1/60/10,1,8);OLED_ShowNum(48,3,bus1/60%10,1,8);OLED_ShowNum(60,3,bus1%60/10,1,8);OLED_ShowNum(66,3,bus1%60%10,1,8);}
				else if(keyNum=='D'){bus1--;OLED_ShowNum(42,3,bus1/60/10,1,8);OLED_ShowNum(48,3,bus1/60%10,1,8);OLED_ShowNum(60,3,bus1%60/10,1,8);OLED_ShowNum(66,3,bus1%60%10,1,8);}
			}
			if(busindex==2){
			OLED_ShowChar(78,3,' ',8);OLED_ShowChar(78,4,'<',8);OLED_ShowChar(78,5,' ',8);
				if(keyNum=='U'){bus2++;OLED_ShowNum(42,4,bus2/60/10,1,8);OLED_ShowNum(48,4,bus2/60%10,1,8);OLED_ShowNum(60,4,bus2%60/10,1,8);OLED_ShowNum(66,4,bus2%60%10,1,8);}
				else if(keyNum=='D'){bus2--;OLED_ShowNum(42,4,bus2/60/10,1,8);OLED_ShowNum(48,4,bus2/60%10,1,8);OLED_ShowNum(60,4,bus2%60/10,1,8);OLED_ShowNum(66,4,bus2%60%10,1,8);}
			}
			if(busindex==3){
			OLED_ShowChar(78,3,' ',8);OLED_ShowChar(78,4,' ',8);OLED_ShowChar(78,5,'<',8);
				if(keyNum=='U'){bus3++;OLED_ShowNum(42,5,bus3/60/10,1,8);OLED_ShowNum(48,5,bus3/60%10,1,8);OLED_ShowNum(60,5,bus3%60/10,1,8);OLED_ShowNum(66,5,bus3%60%10,1,8);}
				else if(keyNum=='D'){bus3--;OLED_ShowNum(42,5,bus3/60/10,1,8);OLED_ShowNum(48,5,bus3/60%10,1,8);OLED_ShowNum(60,5,bus3%60/10,1,8);OLED_ShowNum(66,5,bus3%60%10,1,8);}
			}
		}
		EX0=1;keyNum=0;
		if(onesec==1){
			onesec=0;
			if(bus1>0){bus1--;}
			if(bus2>0){bus2--;}
			if(bus3>0){bus3--;}
			if(bus1==180){bus1near=1;}
			if(bus2==180){bus2near=1;}
			if(bus3==180){bus3near=1;}
			OLED_ShowNum(42,3,bus1/60/10,1,8);
			OLED_ShowNum(48,3,bus1/60%10,1,8);
			OLED_ShowNum(60,3,bus1%60/10,1,8);
			OLED_ShowNum(66,3,bus1%60%10,1,8);
			OLED_ShowNum(42,4,bus2/60/10,1,8);
			OLED_ShowNum(48,4,bus2/60%10,1,8);
			OLED_ShowNum(60,4,bus2%60/10,1,8);
			OLED_ShowNum(66,4,bus2%60%10,1,8);
			OLED_ShowNum(42,5,bus3/60/10,1,8);
			OLED_ShowNum(48,5,bus3/60%10,1,8);
			OLED_ShowNum(60,5,bus3%60/10,1,8);
			OLED_ShowNum(66,5,bus3%60%10,1,8);
			hour=DS1302_Read(0x85);
			min=DS1302_Read(0x83);
			sec=DS1302_Read(0x81);
			adval=XPT2046_Read(0xAC);
			OLED_ShowNum(32,0,(hour/16*10+hour%16)/10,1,16);
			OLED_ShowNum(40,0,(hour/16*10+hour%16)%10,1,16);
			OLED_ShowNum(56,0,(min/16*10+min%16)/10,1,16);
			OLED_ShowNum(64,0,(min/16*10+min%16)%10,1,16);
			OLED_ShowNum(80,0,(sec/16*10+sec%16)/10,1,16);
			OLED_ShowNum(88,0,(sec/16*10+sec%16)%10,1,16);
			if(show10s>=10){
				show10s=0;
				EA=0;
				DHT11_Init();
				DHT11_GetData();
				EA=1;
				OLED_ShowNum(12,7,temp_int,2,8);
				OLED_ShowNum(52,7,(adval*5+1)/3,3,8);
				OLED_ShowNum(100,7,hum_int,2,8);
				if(adval>=30){pwmDuty=0;}
				else{pwmDuty=100-adval*3;}
			}
		}
	}
}