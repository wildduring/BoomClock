#include <reg51.h>
#include <intrins.h>

#include "digital.h"

void Delay50us(void);
void BrightnessCtrl(unsigned int val);

unsigned int Brightness = 20;
unsigned char i;

void main(void)
{
	while(1)
	{
		for(i=0; i<5 ; i++)
		{
			display(i+1, i);
			BrightnessCtrl(Brightness);
		}
	}
}

void BrightnessCtrl(unsigned int val)
{
	while(val>0)
	{
		Delay50us();
		val -= 1;
	}
}

void Delay50us(void)	//—” ±100us@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 20;
	while (--i);
}