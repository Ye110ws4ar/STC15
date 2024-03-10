#include "Timer.h"

void vTimer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0x18;			//设置定时初值
	T2H = 0xFC;			//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;        //开定时器2中断
	EA = 1;				//开启总中断
}

void vTimer2_Init_100us(void) //100微秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x50;			//设置定时初值
	T2H = 0xFB;			//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;        //开定时器2中断
	EA = 1;				//开启总中断
}
