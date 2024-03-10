#include "BTN.h"

/**
  * @brief  独立按键 - 状态机法
  * @param  None
  * @retval key_retrun: 按键的键值，单次触发
  * @author dianshe.taobao.com
  */
#define KEY_NO   0  //无按键状态，用于判断是否按下
#define KEY_DOWN 1  //有按键按下状态，判断是否为抖动
#define KEY_UP   2  //等待松手状态，判断是否弹起
u8 ucBTN_Read_State(void) 				//10ms执行一次
{ 
	static u8 key_state = 0; 	   //定义key_state为静态变量，用于保存每次按键的状态
	u8 key_io = 0, key_return = 0; //key_io:读取的IO状态; key_retrun:返回的键值;
	
	key_io = P3 & 0x0f; 		//对P3读回来的高4位IO清零，屏蔽不关心的IO状态
	switch(key_state) 
	{ 	
		case KEY_NO:     								//无按键状态，用于判断是否按下
			if(key_io!=0x0f) key_state = KEY_DOWN; 
		break; 

		case KEY_DOWN:    								//有按键按下状态，判断是否为抖动
			if(key_io!=0x0f)
			{ 
				key_state = KEY_UP; 
				
				if(key_io==0x0e) key_return = 7;  	//S7
				if(key_io==0x0d) key_return = 6;  	//S6
				if(key_io==0x0b) key_return = 5;  	//S5
				if(key_io==0x07) key_return = 4;  	//S4
			} 
			else 
				key_state = KEY_NO;  
		break;  
		
		case KEY_UP: 									//等待松手状态，判断是否弹起
			if(key_io==0x0f) key_state = KEY_NO; 
		break; 
	} 
	return key_return; 
} 


/**
  * @brief  独立按键 - 三行按键法
  * @param  None
  * @retval None
  * @author dianshe.taobao.com
  */
unsigned char Trg_BTN;
unsigned char Cont_BTN;
void vBTN_Read_ThreeLine(void)
{
    unsigned char ReadData = P3^0xff;
    Trg_BTN = ReadData & (ReadData ^ Cont_BTN);
    Cont_BTN = ReadData;
}