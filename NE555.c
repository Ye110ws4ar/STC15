#include "NE555.h"

void vCounter0_Init(void)
{
    TMOD |= 0x05;  //��ʱ��0/������0���óɼ���ģģʽ
				   //�Ҳ��Զ�����
    TL0 = 0x00;   //TL0��ֵΪ0
    TH0 = 0x00;   //TH0��ֵΪ0
    TR0 = 1;	  //����������
}

//NE555Ƶ�ʲ���
u16 cnt_ne555;
u16 freq_ne555;
void vNE555_Process()	//��Ҫ����1ms��ʱ��2�ж���ִ��
{
	cnt_ne555++;
	if(cnt_ne555>=1000)		//��ȷ��1s
	{
		cnt_ne555=0;
		freq_ne555 = (TH0<<8)|TL0;
		TL0 = 0;
		TH0 = 0;
	}
}