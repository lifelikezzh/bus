#include <REGX52.H>
sbit DS1302_SCLK=P3^6;
sbit DS1302_IO=P3^4;
sbit DS1302_CE=P3^5;
void DS1302_Write(unsigned char command,unsigned char dat);
void DS1302_Init(){
	DS1302_CE=0;
	DS1302_SCLK=0;
	DS1302_Write(0x8E,0x00);
    DS1302_Write(0x80,0x00);
    DS1302_Write(0x82,0x00);
    DS1302_Write(0x84,0x12);
	DS1302_Write(0x8E,0x80);
}
void DS1302_Write(unsigned char command,unsigned char dat){
	unsigned char i;
	DS1302_CE=1;
	for(i=0;i<8;i++){
		DS1302_IO=command&(0x01<<i);
		DS1302_SCLK=1;
		DS1302_SCLK=0;
	}
	for(i=0;i<8;i++){
		DS1302_IO=dat&(0x01<<i);
		DS1302_SCLK=1;
		DS1302_SCLK=0;
	}
	DS1302_CE=0;
}
unsigned char DS1302_Read(unsigned char command){
	unsigned char i,dat=0;
	command|=0x01;
	DS1302_CE=1;
	for(i=0;i<8;i++){
		DS1302_IO=command&(0x01<<i);
		DS1302_SCLK=0;
		DS1302_SCLK=1;
	}
	for(i=0;i<8;i++){
		DS1302_SCLK=1;
		DS1302_SCLK=0;
		if(DS1302_IO==1){dat|=(0x01<<i);}
	}
	DS1302_CE=0;
	DS1302_IO=0;
	return dat;
}