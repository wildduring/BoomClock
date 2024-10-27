#include <reg51.h>
#include <intrins.h>

#include "../digital/digital.h"

sbit BoomKey = P3^2;
sbit Beep = P3^7;

void Boom();
void Delay50us(void);
void Delay80ms(void);

void main()
{
	while(1)
	{
		if(BoomKey == 0)
		{
			Delay80ms();
			if(BoomKey == 0)
				Boom();
		}
	}
}

void Boom()
{
	unsigned char i, j, k, l;
	for(i=10; i>0; i--)
	{
		display(4, i);
		for(j=0; j<4; j++)
		{
			for(k=0; k<50; k++)
			{
				Beep = ! Beep;
				for(l=0; l<50; l++) 
				{
					Delay50us();
				}
			}
			for(k=0; k<100; k++)
			{
				Delay50us();
			}
		}
		for(j=0; j<200; j++)
		{
			Delay50us();
		}
	}
}

void Delay80ms(void)	//@11.0592MHz
{
	unsigned char data i, j;

	i = 144;
	j = 109;
	do
	{
		while (--j);
	} while (--i);
}

void Delay50us(void)	//@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 20;
	while (--i);
}
