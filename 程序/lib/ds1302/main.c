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
		display(9, 0);    //数码管并没有第9段，此处防止第八段显示时间过长，亮度最亮，因为ds1302ReadTime相当于一个大延时
	}
}

/*******************************
延时时间越长数码管亮度越大
延时过慢会导致刷新率过低，肉眼可见的闪烁
*******************************/
