#include "KBD.h"

/**
  * @brief  ������� - ��ȡ��ֵ
  * @param  None
  * @retval key_io: ��ǰ�����ļ�ֵ��ʵ�ʵİ������
  * @author dianshe.taobao.com
  */
u8 ucKBD_KeyIO_State()
{
	u8 key_io = 0;
	P3=0xf0;P4=0xff;	//��ɨ��
	if(P44==0) key_io=4;
	if(P42==0) key_io=8;
	if(P35==0) key_io=12;
	if(P34==0) key_io=16;

	P3=0x0f;P4=0x00;	//��ɨ��
	if(P33==0) key_io=key_io + 0;
	if(P32==0) key_io=key_io + 1;
	if(P31==0) key_io=key_io + 2;
	if(P30==0) key_io=key_io + 3;
	
	return key_io;
}

/**
  * @brief  ������� - ״̬����
  * @param  None
  * @retval key_retrun: �����ļ�ֵ�����δ���
  * @author dianshe.taobao.com
  */
#define KEY_NO   0  //�ް���״̬�������ж��Ƿ���
#define KEY_DOWN 1  //�а�������״̬���ж��Ƿ�Ϊ����
#define KEY_UP   2  //�ȴ�����״̬���ж��Ƿ���
u8 ucKBD_Read_State()
{
	static u8 key_state = 0; 	   // ����key_stateΪ��̬���������ڱ���ÿ�ΰ�����״̬
	u8 key_io = 0, key_return = 0; // key_io:��ȡ��IO״̬, key_retrun:���صļ�ֵ;
	
	key_io = ucKBD_KeyIO_State();
	switch(key_state) 
	{ 	
		case KEY_NO:     								//�ް���״̬�������ж��Ƿ���
			if(key_io) key_state = KEY_DOWN; 
		break; 

		case KEY_DOWN:    								//�а�������״̬���ж��Ƿ�Ϊ����
			if(key_io)
			{
				key_state = KEY_UP;
				key_return = key_io;
			} 
			else 
				key_state = KEY_NO;  
		break;  
		
		case KEY_UP: 									//�ȴ�����״̬���ж��Ƿ���
			if(key_io==0) key_state = KEY_NO; 
		break; 
	} 
	return key_return; 
}



/**
  * @brief  ������� - ��ȡ��ֵ
  * @param  None
  * @retval key_io: ��ǰ�����ļ�ֵ�����¡�λ��Ϊ�͵�ƽ
  * @author dianshe.taobao.com
  */
u8 ucKBD_KeyIO_ThreeLine()
{
	u8 key_io = 0xff;
	P3=0xf0;P4=0xff;	//��ɨ��
	if(P44==0) key_io=0x70;
	if(P42==0) key_io=0xb0;
	if(P35==0) key_io=0xd0;
	if(P34==0) key_io=0xe0;

	P3=0x0f;P4=0x00;	//��ɨ��
	if(P33==0) key_io=key_io | 0x07;
	if(P32==0) key_io=key_io | 0x0b;
	if(P31==0)key_io=key_io | 0x0d;
	if(P30==0) key_io=key_io | 0x0e;
	
	return key_io;
}

/**
  * @brief  ������� - ���а�����
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