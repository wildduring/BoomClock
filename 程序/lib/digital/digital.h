#ifndef _digital_h_
#define _digital_h_

#include <reg51.h>

unsigned char code num_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
																 0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF, 0x00, 0x80};    //0��1��2��3��4��5��6��7��8��9����

void display(unsigned char location, number)
{
	P0 = 0x00;
	P2 = ~(1<<(location-1));
	P0 = num_code[number];
}

#endif