#include "Sonic.h"

void Delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void vSend_Wave()
{
	u8 i;
	for(i=0;i<8;i++)	//����8��40kHz����
	{
		P10 = 1;
		Delay12us();
		P10 = 0;
		Delay12us();
	}
}


u16 uiDistance_Get()
{
	u16 dist;
	vSend_Wave();		//����8��40kHz����
	TH1 = 0; TL1 = 0;	//���㶨ʱ��1
	TR1 = 1;			//������ʱ��1
	while(P11 == 1);	//�ȴ�P11��ɵ͵�ƽ
	TR1 = 0;			//�رն�ʱ��1
	dist = (TH1<<8|TL1)*0.017;	//������룬��λ��cm

	return dist;
}
