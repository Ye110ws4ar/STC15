#include "Delay.h"

/**
  * @brief  实现毫秒级延时 （不推荐长时间延时，会造成程序阻塞）
  * @param  ms输入范围为1~65535
  * @retval None
  * @author dianshe.taobao.com
  */
void vDelay_Ms(unsigned int ms)		//@12.000MHz
{
	unsigned char i, j;
	unsigned int k;					//应该将k定义成unsigned int
	
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
  * @brief  实现1~10微秒延时
  * @param  us输入范围为1~10
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
  * @brief  实现10~100微秒延时
  * @param  us输入范围为1~10
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
  * @brief  实现100~1000微秒延时
  * @param  us输入范围为1~10
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