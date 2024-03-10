#include "NE555.h"

void vCounter0_Init(void)
{
    TMOD |= 0x05;  //定时器0/计数器0配置成计数模模式
				   //且不自动重载
    TL0 = 0x00;   //TL0初值为0
    TH0 = 0x00;   //TH0初值为0
    TR0 = 1;	  //启动计数器
}

//NE555频率测量
u16 cnt_ne555;
u16 freq_ne555;
void vNE555_Process()	//需要放在1ms定时器2中断里执行
{
	cnt_ne555++;
	if(cnt_ne555>=1000)		//精确的1s
	{
		cnt_ne555=0;
		freq_ne555 = (TH0<<8)|TL0;
		TL0 = 0;
		TH0 = 0;
	}
}