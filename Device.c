#include "Device.h"

/**
  * @brief  ������ƺ�����P2=0xa0��ѡͨY5C������ULN2003����P2=0x80��ѡͨY4C������LED�ƣ�
  * @param  None
  * @retval None
  * @author dianshe.taobao.com
  */
void vDevice_Ctrl(unsigned char p2data,unsigned char p0data)
{
	P0 = p0data;
	P2 = P2 & 0x1f | p2data;
	P2 &= 0x1f;
}