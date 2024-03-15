#include "Device.h"

/**
  * @brief  外设控制函数，P2=0xa0（选通Y5C，控制ULN2003）；P2=0x80（选通Y4C，控制LED灯）
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