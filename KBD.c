#include "KBD.h"

/**
  * @brief  矩阵键盘 - 获取键值
  * @param  None
  * @retval key_io: 当前按键的键值，实际的按键标号
  * @author dianshe.taobao.com
  */
u8 ucKBD_KeyIO_State()
{
	u8 key_io = 0;
	P3=0xf0;P4=0xff;	//列扫描
	if(P44==0) key_io=4;
	if(P42==0) key_io=8;
	if(P35==0) key_io=12;
	if(P34==0) key_io=16;

	P3=0x0f;P4=0x00;	//行扫描
	if(P33==0) key_io=key_io + 0;
	if(P32==0) key_io=key_io + 1;
	if(P31==0) key_io=key_io + 2;
	if(P30==0) key_io=key_io + 3;
	
	return key_io;
}

/**
  * @brief  矩阵键盘 - 状态机法
  * @param  None
  * @retval key_retrun: 按键的键值，单次触发
  * @author dianshe.taobao.com
  */
#define KEY_NO   0  //无按键状态，用于判断是否按下
#define KEY_DOWN 1  //有按键按下状态，判断是否为抖动
#define KEY_UP   2  //等待松手状态，判断是否弹起
u8 ucKBD_Read_State()
{
	static u8 key_state = 0; 	   // 定义key_state为静态变量，用于保存每次按键的状态
	u8 key_io = 0, key_return = 0; // key_io:读取的IO状态, key_retrun:返回的键值;
	
	key_io = ucKBD_KeyIO_State();
	switch(key_state) 
	{ 	
		case KEY_NO:     								//无按键状态，用于判断是否按下
			if(key_io) key_state = KEY_DOWN; 
		break; 

		case KEY_DOWN:    								//有按键按下状态，判断是否为抖动
			if(key_io)
			{
				key_state = KEY_UP;
				key_return = key_io;
			} 
			else 
				key_state = KEY_NO;  
		break;  
		
		case KEY_UP: 									//等待松手状态，判断是否弹起
			if(key_io==0) key_state = KEY_NO; 
		break; 
	} 
	return key_return; 
}



/**
  * @brief  矩阵键盘 - 获取键值
  * @param  None
  * @retval key_io: 当前按键的键值，按下“位”为低电平
  * @author dianshe.taobao.com
  */
u8 ucKBD_KeyIO_ThreeLine()
{
	u8 key_io = 0xff;
	P3=0xf0;P4=0xff;	//列扫描
	if(P44==0) key_io=0x70;
	if(P42==0) key_io=0xb0;
	if(P35==0) key_io=0xd0;
	if(P34==0) key_io=0xe0;

	P3=0x0f;P4=0x00;	//行扫描
	if(P33==0) key_io=key_io | 0x07;
	if(P32==0) key_io=key_io | 0x0b;
	if(P31==0)key_io=key_io | 0x0d;
	if(P30==0) key_io=key_io | 0x0e;
	
	return key_io;
}

/**
  * @brief  矩阵键盘 - 三行按键法
  * @param  None
  * @retval None
  * @author dianshe.taobao.com
  */
unsigned char Trg_KBD; 
unsigned char Cont_KBD; 
void vKBD_Read_ThreeLine() 
{ 
    unsigned char ReadData = ucKBD_KeyIO_ThreeLine() ^ 0xff;  
    Trg_KBD  = ReadData & (ReadData ^ Cont_KBD); 
    Cont_KBD = ReadData; 
} 