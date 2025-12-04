#ifndef __DS1302_H__
#define __DS1302_H__
void DS1302_Init();
void DS1302_Write(unsigned char command,unsigned char dat);
unsigned char DS1302_Read(unsigned char command);
void DS1302_Start();
void DS1302_Stop();
#endif