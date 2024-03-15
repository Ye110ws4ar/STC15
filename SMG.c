#include "SMG.h"

u8 smg_buf[8];	//�������ʾ������
u8 code smg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};	//������������
/**
  * @brief  �������ʾ����
  * @param  None
  * @retval None
  * @author dianshe.taobao.com
  */
void vSMG_Display()
{
	static u8 i=0;
	vDevice_Ctrl(0xc0,0);
	vDevice_Ctrl(0xe0,~smg_buf[i]);
	vDevice_Ctrl(0xc0,0x01<<i);
	i = (i + 1) % 8;
}