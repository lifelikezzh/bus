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