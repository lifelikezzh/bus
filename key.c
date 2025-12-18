#include <REGX52.H>
#include "Delay.h"
unsigned char key(){
	unsigned char keyNum=0;
	 if(P3_0==0 || P3_1==0){
        Delay(15);
        if(P3_0==0 && P3_1==0){
            while(P3_0==0 || P3_1==0);
			keyNum='T';return keyNum;
        }
        else if(P3_0==0){
            while(P3_0==0);
			keyNum='U';return keyNum;
        }
        else if(P3_1==0){
            while(P3_1==0);
			keyNum='D';return keyNum;
        }
    }
	return keyNum;
}