#include <INTRINS.H>
void Delay(int ms)	//@11.0592MHz
{
	unsigned char data i, j;
int t;
	for(t=0;t<ms;t++){
	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}
}
void Delay45us(void)	//@11.0592MHz
{
	unsigned char data i;

	i = 18;
	while (--i);
}
void Delay50us(unsigned char num)	//@11.0592MHz
{
	unsigned char data i;
	unsigned char t;
	for(t=0;t<num;t++){
	_nop_();
	i = 20;
	while (--i);
	}
}
