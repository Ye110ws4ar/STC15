#include "Uart.h"

void Uart_Init(void)	//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x8F;		//设定定时初值
	TH1 = 0xFD;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	ES = 1;
	EA = 1;
}

bit busy;
u8 rx_buf[RXBUF_NUM];
u8 rx_cnt = 0;
bit rx_flag;
u8 rx_idletime=0;	//串口接收空闲计时，如果计时到了，则需要清空接收缓存
/*----------------------------
UART 中断服务程序
-----------------------------*/
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //清除RI位
		rx_idletime = 0;		//重置串口接收空闲计时，在连续接受N字节时，不会被清空buf
		rx_buf[rx_cnt++] = SBUF;
		if(rx_buf[rx_cnt-1] == '\n')	//如果检测到换行符，则接收结束
		{
			rx_cnt = 0;			//方便下一次接收N个字节的指令
			rx_flag = 1;		//接收完N个字节数据
		}
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy = 0;               //清忙标志
    }
}

/*----------------------------
串口接收空闲计时处理函数
----------------------------*/
void vRxIdle_Process(void)
{
	rx_idletime++;
	if(rx_idletime>=50)			//过了50ms，串口还没有接收到数据，说明一次连续发送已经完成
	{
		rx_idletime=0;
		rx_cnt=0;							//情况rx_cnt缓存技术
		memset(rx_buf,'\0',sizeof(rx_buf));	//清空了rx_buf
	}
}

/*----------------------------
发送串口数据
----------------------------*/
void SendData(u8 dat)
{
    while (busy);               //等待前面的数据发送完成
    busy = 1;
    SBUF = dat;                 //写数据到UART数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}
