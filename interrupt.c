#include <REGX52.H>
#include <interrupt.h>
extern volatile unsigned char busindex;
extern volatile unsigned char onesec;
extern volatile unsigned char show10s;
extern volatile unsigned char timer0Index;
extern volatile unsigned char uartcmd[6];
extern volatile unsigned char uartindex;
extern volatile unsigned int bus1,bus2,bus3;
volatile unsigned char pwmCount=0,pwmDuty=0;
extern volatile unsigned char ledon;
volatile unsigned char sendHelp=0,readCmd=0;
extern volatile unsigned char bus1near,bus2near,bus3near;
void timer0_init(){
	TMOD&=0xF0;
	TMOD|=0x01;
	TL0=0x00;
	TH0=0x4C;
	TF0=0;
	TR0=1;
	ET0=1;
	EA=1;
	PT0=0;
}
void timer2_init() {
    RCAP2H=0xFF;
    RCAP2L=0x9C;
    TH2=0xFF;
    TL2=0x9C;
    T2CON=0;
    ET2=1;
    TR2=1;
    EA=1;
}
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

void Ext_Init(){
	IT0=1;
	EX0=1;
	IT1=1;
	EX1=1;
}
void UART_SendByte(unsigned char Byte){
	ES=0;
	SBUF=Byte;
	while(TI==0);
	TI=0;
	ES=1;
}
void UART_SendString(unsigned char *String){
	unsigned char g=0;
	while(String[g]!=0){
		UART_SendByte(String[g]);
		g++;
	}
}
void timer0_interrupt()interrupt 1
{
	timer0Index++;
	TL0=0x00;TH0=0x4C;
	if(timer0Index>=20){
		timer0Index=0;show10s++;onesec=1;
	}
}
void timer2_interrupt() interrupt 5
{
    TF2=0;
    pwmCount++;
    if(pwmCount>=100){pwmCount=0;}
    if(pwmCount<pwmDuty && ledon==1){P2=0x00;}
	else{P2=0xFF;}
}
void INT1_Isr() interrupt 2
{
	sendHelp=1;
	P1_5=0;
}
void INT0_Isr() interrupt 0
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
			readCmd=1;
		}
	}
}
void UART_main()
{
	if(bus1near==1 || bus2near==1 || bus3near==1){
		if(bus1near==1){
			bus1near=0;UART_SendString("bus1 is approaching\n");
		}
		if(bus2near==1){
			bus2near=0;UART_SendString("bus2 is approaching\n");
		}
		if(bus3near==1){
			bus3near=0;UART_SendString("bus3 is approaching\n");
		}
	}
	if(sendHelp==1){
		UART_SendString("HELP\n");
		sendHelp=0;
	}
	if(readCmd==1){
		if(uartcmd[0]=='Q' && uartcmd[1]==0){
			UART_SendString("bus1: ");
			UART_SendByte(bus1/60/10+'0');
			UART_SendByte(bus1/60%10+'0');
			UART_SendString(":");
			UART_SendByte(bus1%60/10+'0');
			UART_SendByte(bus1%60%10+'0');
			UART_SendString("\n");
			UART_SendString("bus2: ");
			UART_SendByte(bus2/60/10+'0');
			UART_SendByte(bus2/60%10+'0');
			UART_SendString(":");
			UART_SendByte(bus2%60/10+'0');
			UART_SendByte(bus2%60%10+'0');
			UART_SendString("\n");
			UART_SendString("bus3: ");
			UART_SendByte(bus3/60/10+'0');
			UART_SendByte(bus3/60%10+'0');
			UART_SendString(":");
			UART_SendByte(bus3%60/10+'0');
			UART_SendByte(bus3%60%10+'0');
			UART_SendString("\n");
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
					UART_SendString("error\n");
					break;
			}
		}
		uartindex=0;
		uartcmd[0]=0;uartcmd[1]=0;uartcmd[2]=0;uartcmd[3]=0;uartcmd[4]=0;uartcmd[5]=0;
		readCmd=0;
	}
}