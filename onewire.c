/*
  ����˵��: ��������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨(�ⲿ����12MHz) STC89C52RC��Ƭ��
  ��    ��: 2011-8-9
*/
#include "onewire.h"

sbit DQ = P1^4;  //�����߽ӿ�

//��������ʱ����
void Delay_OneWire(unsigned int t)  //��ע������1��STC89C52RC --> IAP15
{
	t = t * 12;
	while(t--);
}

//ͨ����������DS18B20дһ���ֽ�
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

//��DS18B20��ȡһ���ֽ�
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

//DS18B20�豸��ʼ��
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


//��Ҫ�Լ���ɶ�ȡDS18B20�¶ȵĳ���
float fRead_Temperature(void)
{
	float temp_return;
	u8 low,high;
	init_ds18b20();
	Write_DS18B20(0xcc); 		//������ȡROM
	Write_DS18B20(0x44); 		//�����¶�ת��
								//������Delay��ʱ���ȴ��¶�ת����ɣ�ֱ��ȥ��ȡ�ݴ���
	init_ds18b20();
	Write_DS18B20(0xcc); 		//������ȡROM
	Write_DS18B20(0xbe); 		//��ȡ�ݴ�������
	low = Read_DS18B20();		//�¶ȵ�8λ����
	high = Read_DS18B20();		//�¶ȸ�8λ����
	temp_return = (high<<8|low)*0.0625;
	
	return temp_return;
}


	





