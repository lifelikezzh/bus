#include <REGX52.H>
#include "Delay.h"
unsigned char key(){
	unsigned char keyNum=0;
	if(P3_0==0){Delay(20);while(P3_0==0);Delay(20);keyNum='U';return keyNum;}
	if(P3_1==0){Delay(20);while(P3_1==0);Delay(20);keyNum='D';return keyNum;}
	return keyNum;
}