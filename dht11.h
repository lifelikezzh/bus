#ifndef __DHT11_H__
#define __DHT11_H__
extern unsigned char hum_int;
extern unsigned char hum_dec;
extern unsigned char temp_int;
extern unsigned char temp_dec;
extern unsigned char verify;
void DHT11_Init();
unsigned char DHT11_GetData();
#endif
