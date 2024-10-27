#include <reg51.h>
#include <intrins.h>

#include "ds1302.h"
#include "../digital/digital.h"

void Delay(void)	//@11.0592MHz
{
	unsigned char data i, j;

	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}

void main(void)
{
	ds1302Init();
	while(1)
	{
		ds1302ReadTime();
		display(1, TIME[2]/16);
		Delay();
		display(2, TIME[2]&0x0f);
		Delay();
		display(3, TIME[1]/16);
		Delay();
		display(4, TIME[1]&0x0f);
		Delay();
		display(5, TIME[0]/16);
		Delay();
		display(6, TIME[0]&0x0f);
		Delay();
		display(7, TIME[5]/16);
		Delay();
		display(8, TIME[5]&0x0f);
		Delay();
		display(9, 0);    //����ܲ�û�е�9�Σ��˴���ֹ�ڰ˶���ʾʱ�������������������Ϊds1302ReadTime�൱��һ������ʱ
	}
}

/*******************************
��ʱʱ��Խ�����������Խ��
��ʱ�����ᵼ��ˢ���ʹ��ͣ����ۿɼ�����˸
*******************************/
