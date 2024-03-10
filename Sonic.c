#include "Sonic.h"

void Delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void vSend_Wave()
{
	u8 i;
	for(i=0;i<8;i++)	//发送8个40kHz方波
	{
		P10 = 1;
		Delay12us();
		P10 = 0;
		Delay12us();
	}
}


u16 uiDistance_Get()
{
	u16 dist;
	vSend_Wave();		//发送8个40kHz方波
	TH1 = 0; TL1 = 0;	//清零定时器1
	TR1 = 1;			//开启定时器1
	while(P11 == 1);	//等待P11变成低电平
	TR1 = 0;			//关闭定时器1
	dist = (TH1<<8|TL1)*0.017;	//计算距离，单位是cm

	return dist;
}
