#include <reg51.h>
#include <intrins.h>

sbit Beep = P3^7;

void beep();
void Delay50us(void);

void main()
{
	unsigned char i, j;
	beep();
	for(i=0; i<100; i++) for(j=0; j<25; j++) Delay50us();
	beep();
	for(i=0; i<100; i++) for(j=0; j<25; j++) Delay50us();
	beep();
	for(i=0; i<100; i++) for(j=0; j<25; j++) Delay50us();
	beep();
	for(i=0; i<100; i++) for(j=0; j<25; j++) Delay50us();
	while(1);
}

void beep()
{
	unsigned char i, j, k;
	for(j=0; j<40; j++)
	{
		Beep = !Beep;
		for(k=0; k<20; k++) 
		{
			Delay50us();
		}
	}
}

void Delay50us(void)	//ÑÓÊ±100us@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 20;
	while (--i);
}
