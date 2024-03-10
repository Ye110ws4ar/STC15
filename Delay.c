#include "Delay.h"

/**
  * @brief  ʵ�ֺ��뼶��ʱ �����Ƽ���ʱ����ʱ������ɳ���������
  * @param  ms���뷶ΧΪ1~65535
  * @retval None
  * @author dianshe.taobao.com
  */
void vDelay_Ms(unsigned int ms)		//@12.000MHz
{
	unsigned char i, j;
	unsigned int k;					//Ӧ�ý�k�����unsigned int
	
	for(k=0;k<ms;k++)
	{
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}	
}

/**
  * @brief  ʵ��1~10΢����ʱ
  * @param  us���뷶ΧΪ1~10
  * @retval None
  * @author dianshe.taobao.com
  */
void vDelay_Us(unsigned int us)		//@12.000MHz
{
	unsigned int k;
	for(k=0;k<us;k++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}

/**
  * @brief  ʵ��10~100΢����ʱ
  * @param  us���뷶ΧΪ1~10
  * @retval None
  * @author dianshe.taobao.com
  */
void vDelay_10us(unsigned int us)	//@12.000MHz
{
	unsigned char i;
	unsigned int k;
	
	for(k=0;k<us;k++)
	{
		_nop_();
		_nop_();
		i = 27;
		while (--i);
	}	
}

/**
  * @brief  ʵ��100~1000΢����ʱ
  * @param  us���뷶ΧΪ1~10
  * @retval None
  * @author dianshe.taobao.com
  */
void vDelay_100us(unsigned int us)	//@12.000MHz
{
	unsigned char i, j;
	unsigned int k;
		
	for(k=0;k<us;k++)
	{
		i = 2;
		j = 39;
		do
		{
			while (--j);
		} while (--i);
	}
}