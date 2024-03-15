/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台(外部晶振12MHz) STC89C52RC单片机
  日    期: 2011-8-9
*/
#include "onewire.h"

sbit DQ = P1^4;  //单总线接口

//单总线延时函数
void Delay_OneWire(unsigned int t)  //【注意事项1】STC89C52RC --> IAP15
{
	t = t * 12;
	while(t--);
}

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20设备初始化
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}


//需要自己完成读取DS18B20温度的程序
float fRead_Temperature(void)
{
	float temp_return;
	u8 low,high;
	init_ds18b20();
	Write_DS18B20(0xcc); 		//跳过读取ROM
	Write_DS18B20(0x44); 		//启动温度转换
								//不加上Delay延时，等待温度转换完成，直接去读取暂存器
	init_ds18b20();
	Write_DS18B20(0xcc); 		//跳过读取ROM
	Write_DS18B20(0xbe); 		//获取暂存器数据
	low = Read_DS18B20();		//温度低8位数据
	high = Read_DS18B20();		//温度高8位数据
	temp_return = (high<<8|low)*0.0625;
	
	return temp_return;
}


	





