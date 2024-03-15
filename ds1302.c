/*
  ����˵��: DS1302��������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨ 8051��12MHz
  ��    ��: 2011-8-9
*/

#include "ds1302.h"

sbit SCK=P1^7;		
sbit IO=P2^3;		
sbit CE = P1^3;   // DS1302��λ												

void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		IO=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	CE=0;	_nop_();
 	SCK=0;	_nop_();
 	CE=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	CE=0; 
}

unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	CE=0;	_nop_();
 	SCK=0;	_nop_();
 	CE=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(IO)
 		temp|=0x80;	
 		SCK=1;
	} 
 	CE=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	IO=0;	_nop_();
	IO=1;	_nop_();
	return (temp);			
}


//----------���º�����Ҫ�Լ�ʵ��-----------------//
void vClock_Set(u8 hour,u8 minute,u8 second)
{
	Write_Ds1302_Byte(0x8e,0x00);	//�ر�д����
	Write_Ds1302_Byte(0x80,DecToBCD(second));
	Write_Ds1302_Byte(0x82,DecToBCD(minute));
	Write_Ds1302_Byte(0x84,DecToBCD(hour));
	Write_Ds1302_Byte(0x8e,0x80);	//��д����
}
