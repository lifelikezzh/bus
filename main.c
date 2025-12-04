#include <REGX52.H>
#include "Delay.h"
#include "oled.h"
#include "dht11.h"
#include "ds1302.h"
#include "xpt2046.h"
#include "key.h"
unsigned char c=0;
unsigned char ledon=0;
unsigned char hour=12,min=0,sec=0;
unsigned int adval=101;
unsigned char interrupt1=0;
unsigned char show10s=0;
unsigned char bus1=57,bus2=40,bus3=99;
unsigned char onesec=0;
unsigned char uartcmd[6]={0};
unsigned char uartindex=0;
unsigned char busindex=0;
unsigned char keyNum=0;
unsigned char isDs1302stopped=0,isTimer0stopped=0;
unsigned char pwm_count=0,pwm_duty=0;
void timer0_init(){
	TMOD=0x01;
	TL0=0x00;
	TH0=0x4C;
	TF0=0;
	TR0=1;
	ET0=1;
	EA=1;
	PT0=0;
}
//void timer2_init(void) {
//    RCAP2H=0xFF;
//    RCAP2L=0x1A;
//    TH2=0xFF;
//    TL2=0x1A;
//    T2CON=0;
//    ET2=1;
//    TR2=1;
//    EA=1;
//}

void UART_Init(){
	PCON&=0x7F;
	SCON=0x50;
	TMOD&=0x0F;
	TMOD|=0x20;
	TL1=0xFD;
	TH1=0xFD;
	ET1=0;
	TR1=1;
	ES=1;
}

void Ext_Init(void)
{
	IT0=1;
	EX0=1;
	IT1=0;
	EX1=1;
}
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}
void UART_SendString(unsigned char *String)
{
	unsigned char g=0;
	while(String[g]!=0){
		UART_SendByte(String[g]);
		g++;
	}
}
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
	OLED_ShowString(64,0,"min",8);
	OLED_ShowString(0,1,"bus 2:",8);
	OLED_ShowString(64,1,"min",8);
	OLED_ShowString(0,2,"bus 3:",8);
	OLED_ShowString(64,2,"min",8);
	OLED_ShowNum(32,5,hour/16*10+hour%16,2,8);
	OLED_ShowNum(48,5,min/16*10+min%16,2,8);
	OLED_ShowNum(64,5,sec/16*10+sec%16,2,8);
	OLED_ShowNum(40,6,adval,3,8);
	OLED_ShowNum(72,3,temp_int,2,8);
	OLED_ShowNum(72,4,hum_int,2,8);
	timer0_init();
//	timer2_init();
	DS1302_Init();
	Ext_Init();
	hour=DS1302_Read(0x85);min=DS1302_Read(0x83);sec=DS1302_Read(0x81);
	UART_Init();
	while(1){
		keyNum=key();
		OLED_ShowNum(0,7,busindex,1,8);
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
			OLED_ShowNum(40,0,bus1,2,8);
			OLED_ShowNum(40,1,bus2,2,8);
			OLED_ShowNum(40,2,bus3,2,8);
			hour=DS1302_Read(0x85);
			min=DS1302_Read(0x83);
			sec=DS1302_Read(0x81);
			adval=XPT2046_Read(0xAC);
			DHT11_Init();
			DHT11_GetData();
			OLED_ShowNum(32,5,hour/16*10+hour%16,2,8);
			OLED_ShowNum(48,5,min/16*10+min%16,2,8);
			OLED_ShowNum(64,5,sec/16*10+sec%16,2,8);
			if(show10s>=10){
				show10s=0;
				OLED_ShowNum(40,6,adval,3,8);
				OLED_ShowNum(72,3,temp_int,2,8);
				OLED_ShowNum(72,4,hum_int,2,8);
//				if(adval>=120){pwm_duty=0;}
//				else{pwm_duty=adval/3;}
			}
		}
	}
}
void timer0_interrupt()interrupt 1
{
	interrupt1++;
	TL0=0x00;TH0=0x4C;
	if(interrupt1>=20){
		interrupt1=0;show10s++;onesec=1;
	}
}
//void timer2_interrupt()interrupt 5 
//{
//    T2CON&=0x7F;
//    pwm_count++;
//    if(pwm_count>=40){pwm_count=0;}
//    if(pwm_count<pwm_duty){P2_0=0;}
//	else{P2_0=1;}
//}
void INT1_Isr(void) interrupt 2
{
	UART_SendString("HELP");
	while(P3_3==0);
}
void INT0_Isr(void) interrupt 0
{
	EX0=0;
	busindex++;
	if(busindex>=4){busindex=0;}
}
void uart_interrupt()interrupt 4
{
	if(RI==1){
		unsigned char rec;
		RI=0;
		rec=SBUF;
		if(rec!='\n'){uartcmd[uartindex]=rec;uartindex++;}
		else{
			if(uartcmd[0]=='Q' && uartcmd[1]==0){
				UART_SendString("bus1: ");
				UART_SendByte(bus1/10+'0');
				UART_SendByte(bus1%10+'0');
				UART_SendString("min\n");
				UART_SendString("bus2: ");
				UART_SendByte(bus2/10+'0');
				UART_SendByte(bus2%10+'0');
				UART_SendString("min\n");
				UART_SendString("bus3: ");
				UART_SendByte(bus3/10+'0');
				UART_SendByte(bus3%10+'0');
				UART_SendString("min\n");
			}
			else if(uartcmd[0]=='S'){
				unsigned char which=uartcmd[1]-'0';
				switch(which){
					case 1:
						bus1=(uartcmd[2]-'0')*100+(uartcmd[3]-'0')*10+uartcmd[4]-'0';
						break;
					case 2:
						bus2=(uartcmd[2]-'0')*100+(uartcmd[3]-'0')*10+uartcmd[4]-'0';
						break;
					case 3:
						bus3=(uartcmd[2]-'0')*100+(uartcmd[3]-'0')*10+uartcmd[4]-'0';
						break;
					default:
						UART_SendString("error");
						break;
				}
			}
			uartindex=0;uartcmd[0]=0;uartcmd[1]=0;uartcmd[2]=0;uartcmd[3]=0;uartcmd[4]=0;uartcmd[5]=0;
		}
	}
}