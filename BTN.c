#include "BTN.h"

/**
  * @brief  �������� - ״̬����
  * @param  None
  * @retval key_retrun: �����ļ�ֵ�����δ���
  * @author dianshe.taobao.com
  */
#define KEY_NO   0  //�ް���״̬�������ж��Ƿ���
#define KEY_DOWN 1  //�а�������״̬���ж��Ƿ�Ϊ����
#define KEY_UP   2  //�ȴ�����״̬���ж��Ƿ���
u8 ucBTN_Read_State(void) 				//10msִ��һ��
{ 
	static u8 key_state = 0; 	   //����key_stateΪ��̬���������ڱ���ÿ�ΰ�����״̬
	u8 key_io = 0, key_return = 0; //key_io:��ȡ��IO״̬; key_retrun:���صļ�ֵ;
	
	key_io = P3 & 0x0f; 		//��P3�������ĸ�4λIO���㣬���β����ĵ�IO״̬
	switch(key_state) 
	{ 	
		case KEY_NO:     								//�ް���״̬�������ж��Ƿ���
			if(key_io!=0x0f) key_state = KEY_DOWN; 
		break; 

		case KEY_DOWN:    								//�а�������״̬���ж��Ƿ�Ϊ����
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
		
		case KEY_UP: 									//�ȴ�����״̬���ж��Ƿ���
			if(key_io==0x0f) key_state = KEY_NO; 
		break; 
	} 
	return key_return; 
} 


/**
  * @brief  �������� - ���а�����
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