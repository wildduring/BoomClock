#ifndef _BoomClock_c_
#define _BoomClock_c_

#include "BoomClock.h"

#include "lib/digital/digital.h"
#include "lib/ds1302/ds1302.h"

#define BrightnessHigh 40
#define BrightnessLow 1

unsigned char Brightness = BrightnessHigh;
unsigned char flashCount = 0;
bit flashFlag = 1;
bit enAlarm1 = 0;
bit enAlarm2 = 0;
unsigned char Alarm1Time[7] = {0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00};
unsigned char Alarm2Time[7] = {0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00};

unsigned char m,n;
unsigned char code T[49][2]={{0,0},
{0xF8,0x8B},{0xF8,0xF2},{0xF9,0x5B},{0xF9,0xB7},{0xFA,0x14},{0xFA,0x66},{0xFA,0xB9},{0xFB,0x03},{0xFB,0x4A},{0xFB,0x8F},{0xFB,0xCF},{0xFC,0x0B},
{0xFC,0x43},{0xFC,0x78},{0xFC,0xAB},{0xFC,0xDB},{0xFD,0x08},{0xFD,0x33},{0xFD,0x5B},{0xFD,0x81},{0xFD,0xA5},{0xFD,0xC7},{0xFD,0xE7},{0xFE,0x05},
{0xFE,0x21},{0xFE,0x3C},{0xFE,0x55},{0xFE,0x6D},{0xFE,0x84},{0xFE,0x99},{0xFE,0xAD},{0xFE,0xC0},{0xFE,0x02},{0xFE,0xE3},{0xFE,0xF3},{0xFF,0x02},
{0xFF,0x10},{0xFF,0x1D},{0xFF,0x2A},{0xFF,0x36},{0xFF,0x42},{0xFF,0x4C},{0xFF,0x56},{0xFF,0x60},{0xFF,0x69},{0xFF,0x71},{0xFF,0x79},{0xFF,0x81}
};
unsigned char code music[][2]={{0,4},
{17,8},{17,8},{17,8},{13,8},{17,4},{18,8},{17,12},{18,4},{17,4},{15,4},{15,12},{22,8},{15,4},{15,12},{0,8},{0,8},{0,8},{20,8},{20,8},{20,8},{15,8},{20,4},{22,4},{20,4},{20,8},{20,4},{22,4},{20,4},{18,4},{18,8},{18,4},{20,4},{18,8},{17,12},{0,8},{0,8},{0,8},{17,8},{17,8},{17,8},{13,8},{17,4},{18,8},{17,8},{17,4},{18,4},{17,4},{15,4},{15,8},{15,4},{22,8},{15,4},{15,12},{0,8},{0,8},{0,8},{20,8},{20,8},{20,8},{15,8},{20,4},{22,4},{20,4},{20,8},{20,4},{22,4},{24,4},{25,4},{25,12},{24,4},{24,8},{25,20},
{0xFF,0xFF}};

void InitBoomClock(void)
{
	unsigned int i;
	for(i=1024; i>0; i--);    //等待ds1302先上电
	ds1302Init();
	Timer0_Init();
}

void Delay50us(void)	//延时100us@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 20;
	while (--i);
}

void BrightnessCtrl(unsigned char val)
{
	while(val>0)
	{
		Delay50us();
		val -= 1;
	}
}

void DisplayRealTime(void)
{
	ds1302ReadTime();
	DisplayTime(TIME, 1);
}

void DisplayTime(unsigned char* date, bit isFlash)		//显示时间
{
	display(1, date[2]/16);
	BrightnessCtrl(Brightness);
	if(isFlash && date[0]%2) display(2, date[2]%0x10+10);
	else display(2, date[2]&0x0f);
	BrightnessCtrl(Brightness);
	display(3, date[1]/16);
	BrightnessCtrl(Brightness);
	display(4, date[1]&0x0f);
	BrightnessCtrl(Brightness);
	display(5, TIME[4]/16);
	BrightnessCtrl(Brightness);
	display(6, TIME[4]&0x0f);
	BrightnessCtrl(Brightness);
	display(7, TIME[3]/16);
	BrightnessCtrl(Brightness);
	display(8, TIME[3]&0x0f);
	BrightnessCtrl(Brightness);
	display(9, 0);    //数码管并没有第9段，此处防止第八段显示时间过长，亮度最亮，因为ds1302ReadTime相当于一个大延时
	if(Brightness == BrightnessLow) BrightnessCtrl(500);
}

void ScanKeys(void)    //按键扫描
{
	unsigned char i;
	if(Alarm1Key == 0)
	{
		DisplayRealTime();		//消抖延时
		if(Alarm1Key == 0)
		{
			for(i=0; i<10; i++) DisplayRealTime();
			if(Alarm1Key == 1)		//短按
			{
				reverseAlarm1();
			}
			else		//长按
			{
				for(i=0; i<30; i++) DisplayRealTime();
				if(Alarm1Key == 0)    //长按
				{
					while(Alarm1Key == 0)		//等他松开
					{
						//////////////////这个时候先闪着，如果他敢不松手就操作那不理他//////////////////////////
						TR0 = 1;
						if(flashFlag)
						{
							DisplayTime(Alarm1Time, 0);
						}
						else 
						{
							for(i=1; i<5; i++) display(i, 20);
								display(5, TIME[4]/16);
								BrightnessCtrl(Brightness);
								display(6, TIME[4]&0x0f);
								BrightnessCtrl(Brightness);
								display(7, TIME[3]/16);
								BrightnessCtrl(Brightness);
								display(8, TIME[3]&0x0f);
								BrightnessCtrl(Brightness);
								display(9, 0);    //数码管并没有第9段，此处防止第八段显示时间过长，亮度最亮，因为ds1302ReadTime相当于一个大延时
						}
					}
					setAlarm1();
				}
				else if(Alarm1Key == 1)    //短按
				{
					reverseAlarm1();
				}
			}
		}
	}
	if(Alarm2Key == 0)
	{
		DisplayRealTime();		//消抖延时
		if(Alarm2Key == 0)
		{
			for(i=0; i<10; i++) DisplayRealTime();
			if(Alarm2Key == 1)		//短按
			{
				reverseAlarm2();
			}
			else		//长按
			{
				for(i=0; i<30; i++) DisplayRealTime();
				if(Alarm2Key == 0)    //长按
				{
					while(Alarm2Key == 0)		//等他松开
					{
						//////////////////这个时候先闪着，如果他敢不松手就操作那不理他//////////////////////////
						TR0 = 1;
						if(flashFlag)
						{
							DisplayTime(Alarm2Time, 0);
						}
						else 
						{
							for(i=1; i<5; i++) display(i, 20);
								display(5, TIME[4]/16);
								BrightnessCtrl(Brightness);
								display(6, TIME[4]&0x0f);
								BrightnessCtrl(Brightness);
								display(7, TIME[3]/16);
								BrightnessCtrl(Brightness);
								display(8, TIME[3]&0x0f);
								BrightnessCtrl(Brightness);
								display(9, 0);    //数码管并没有第9段，此处防止第八段显示时间过长，亮度最亮，因为ds1302ReadTime相当于一个大延时
						}
					}
					setAlarm2();
				}
				else if(Alarm2Key == 1)    //短按
				{
					reverseAlarm2();
				}
			}
		}
	}
	if(SetKey == 0)
	{
		DisplayRealTime();		//消抖延时
		if(SetKey == 0)
		{
			for(i=0; i<10; i++) DisplayRealTime();
			if(SetKey == 1)		//短按
			{
				showYear();
			}
			else		//长按
			{
				for(i=0; i<50; i++) DisplayRealTime();
				if(SetKey == 0)
				{
					while(SetKey == 0)		//等他松开
					{
						//////////////////这个时候先闪着，如果他敢不松手就操作那不理他//////////////////////////
						TR0 = 1;
						if(flashFlag) DisplayTime(TIME, 0);
						else for(i=1; i<10; i++) display(i, 20);
					}
					setTime();
				}
			}
		}
		else if(SetKey == 1)    //短按
		{
			showYear();
		}
	}
	if(BoomKey == 0)
	{
		DisplayRealTime();		//消抖延时
		if(BoomKey == 0)
		{
			for(i=0; i<10; i++) DisplayRealTime();
			if(BoomKey == 1)		//短按
			{
				Boom();
			}
			else		//长按
			{
				for(i=0; i<50; i++) DisplayRealTime();
				if(BoomKey == 0)
				{
					setBrightness();
					while(BoomKey == 0) DisplayRealTime();
				}
			}
		}
		else if(BoomKey == 1)    //短按
		{
			Boom();
		}
	}
	TR0 = 0;					//关定时器
	TL0 = 0xCD;				//设置定时初始值
	TH0 = 0xD4;				//设置定时初始值
	for(i=0; i<10; i++) DisplayRealTime();		//按后也消抖
}

void reverseAlarm1(void)	//反转闹钟1开关
{
	unsigned char i, j;
	enAlarm1 = !enAlarm1;
	AlarmLED1 = enAlarm1;
	if(Brightness == BrightnessHigh) for(i=0; i<127; i++) displayFlashAlarm(0, Alarm1Time, 0);
	else for(i=0; i<255; i++) for(j=0; j<8; j++) displayFlashAlarm(0, Alarm1Time, 0);
}

void reverseAlarm2(void)	//反转闹钟2开关
{
	unsigned char i, j;
	enAlarm2 = !enAlarm2;
	AlarmLED2 = enAlarm2;
	if(Brightness == BrightnessHigh) for(i=0; i<127; i++) displayFlashAlarm(0, Alarm2Time, 0);
	else for(i=0; i<255; i++) for(j=0; j<8; j++) displayFlashAlarm(0, Alarm2Time, 0);
}

void setBrightness(void)	//设置亮度
{
	if(Brightness > 10)
	{
		Brightness = BrightnessLow;
	}
	else if(Brightness < 10)
	{
		Brightness = BrightnessHigh;
	}
}

void Boom_Display(unsigned int i)    //彩蛋中显示数字
{
	unsigned char l, delay = 5;
	display(1, i/1000);
	for(l=delay; l>0; l--);
	display(2, i/100%10);
	for(l=delay; l>0; l--);
	display(3, i/10%10);
	for(l=delay; l>0; l--);
	display(4, i%10);
	for(l=delay; l>0; l--);
	display(9, 0);
}

void Boom(void)					//小彩蛋
{
	unsigned int i, j, k;
	for(i=10; i>0; i--)
	{
		if(i<10)
		{
			for(j=50; j>0; j--)
			{
				Beep = !Beep;
				Boom_Display(i);
			}
		}
		for(j=1024; j>0; j--)    //延时
		{
			Boom_Display(i);
			if(BoomKey == 0)    //如果Boom按钮按下时间增加10单位
			{
				for(k=20; k>0; k--)    //消抖
				{
					Boom_Display(i);
				}
				while(BoomKey == 0) Boom_Display(i);
				i += 10;
				if(i>9999) i=9999;
			}
			if((Alarm1Key & Alarm2Key & SetKey & UpKey &	DownKey) == 0)    //如果其他按键按下退出
			{
				for(k=20; k>0; k--)    //消抖
				{
					Boom_Display(i);
				}
				while((Alarm1Key & Alarm2Key & SetKey & UpKey &	DownKey) == 0) Boom_Display(i);
				return;
			}
		}
	}
	PlayMusic();
}

void displayFlashAlarm(unsigned char select, unsigned char* date, bit isYear)    //用这个函数前记得把定时器打开，用完再关上
{
	if(!isYear)
	{
		if(select==1 && flashFlag) display(1, 20);
		else display(1, date[2]/16);
		BrightnessCtrl(Brightness);
		if(select==2 && flashFlag) display(2, 21);
		else display(2, date[2]%0x10+10);
		BrightnessCtrl(Brightness);
		if(select==3 && flashFlag) display(3, 20);
		else display(3, date[1]/16);
		BrightnessCtrl(Brightness);
		if(select==4 && flashFlag) display(4, 20);
		else display(4, date[1]&0x0f);
		BrightnessCtrl(Brightness);
		if(select==5 && flashFlag) display(5, 20);
		else display(5, TIME[4]/16);
		BrightnessCtrl(Brightness);
		if(select==6 && flashFlag) display(6, 20);
		else display(6, TIME[4]&0x0f);
		BrightnessCtrl(Brightness);
		if(select==7 && flashFlag) display(7, 20);
		else display(7, TIME[3]/16);
		BrightnessCtrl(Brightness);
		if(select==8 && flashFlag) display(8, 20);
		else display(8, TIME[3]&0x0f);
		BrightnessCtrl(Brightness);
	}
	else
	{
		if(select==1 && flashFlag) display(1, 20);
		else display(1, 2);
		BrightnessCtrl(Brightness);
		if(select==2 && flashFlag) display(2, 20);
		else display(2, 0);
		BrightnessCtrl(Brightness);
		if(select==3 && flashFlag) display(3, 20);
		else display(3, TIME[6]/16);
		BrightnessCtrl(Brightness);
		if(select==4 && flashFlag) display(4, 20);
		else display(4, TIME[6]&0x0f);
		BrightnessCtrl(Brightness);
		if(select==5 && flashFlag) display(5, 20);
		else display(5, TIME[4]/16);
		BrightnessCtrl(Brightness);
		if(select==6 && flashFlag) display(6, 20);
		else display(6, TIME[4]&0x0f);
		BrightnessCtrl(Brightness);
		if(select==7 && flashFlag) display(7, 20);
		else display(7, TIME[3]/16);
		BrightnessCtrl(Brightness);
		if(select==8 && flashFlag) display(8, 20);
		else display(8, TIME[3]&0x0f);
		BrightnessCtrl(Brightness);
	}
}

void setAlarm1(void)			//设置闹钟1
{
	unsigned char i;
	unsigned char select = 1;
	while(select<5)
	{
		displayFlashAlarm(select, Alarm1Time, 0);
		if(Alarm1Key ==0)
		{
			for(i=0; i<10; i++) displayFlashAlarm(select, Alarm1Time, 0);
			if(Alarm1Key ==0)
			{
				while(Alarm1Key ==0) displayFlashAlarm(select, Alarm1Time, 0);
				if(Alarm1Time[2]>0x23) Alarm1Time[2] = 0x20;
				select++;
			}
		}
		if(UpKey == 0)
		{
			for(i=0; i<5; i++) displayFlashAlarm(select, Alarm1Time, 0);
			if(UpKey == 0)
			{
				while(UpKey ==0) displayFlashAlarm(select, Alarm1Time, 0);
				switch (select)
				{
					case 1:
						if(Alarm1Time[2]/0x10 < 0x02) Alarm1Time[2] += 0x10;
						else Alarm1Time[2] %= 0x10;
						break;
					case 2:
						if(Alarm1Time[2]%0x10 < 0x09) Alarm1Time[2] += 0x01;
						else Alarm1Time[2] = Alarm1Time[2]/0x10*0x10;
						if(Alarm1Time[2] > 0x23) Alarm1Time[2] = 0x20;
						break;
					case 3:
						if(Alarm1Time[1]/0x10 < 0x05) Alarm1Time[1] += 0x10;
						else Alarm1Time[1] %= 0x10;
						break;
					case 4:
						if(Alarm1Time[1]%0x10 < 0x09) Alarm1Time[1] += 0x01;
						else Alarm1Time[1] = Alarm1Time[1]/0x10*0x10;
						break;
				}
			}
		}
		if(DownKey == 0)
		{
			for(i=0; i<5; i++) displayFlashAlarm(select, Alarm1Time, 0);
			if(DownKey == 0)
			{
				while(DownKey ==0) displayFlashAlarm(select, Alarm1Time, 0);
				switch (select)
				{
					case 1:
						if(Alarm1Time[2]/0x10 == 0x00) Alarm1Time[2] += 0x20;
						else Alarm1Time[2] -= 0x10;
						break;
					case 2:
						if(Alarm1Time[2]%0x10 == 0x00) Alarm1Time[2] += 0x09;
						else Alarm1Time[2] -= 0x01;
						if(Alarm1Time[2] > 0x23) Alarm1Time[2] = 0x23;
						break;
					case 3:
						if(Alarm1Time[1]/0x10 == 0x00) Alarm1Time[1] += 0x50;
						else Alarm1Time[1] -= 0x10;
						break;
					case 4:
						if(Alarm1Time[1]%0x10 == 0x00) Alarm1Time[1] += 0x09;
						else Alarm1Time[1] -= 0x01;
						break;
				}
			}
		}
	}
	AlarmLED1 = 1;
	enAlarm1 = 1;
}

void setAlarm2(void)			//设置闹钟2
{
	unsigned char i;
	unsigned char select = 1;
	while(select<5)
	{
		displayFlashAlarm(select, Alarm2Time, 0);
		if(Alarm2Key ==0)
		{
			for(i=0; i<10; i++) displayFlashAlarm(select, Alarm2Time, 0);
			if(Alarm2Key ==0)
			{
				while(Alarm2Key ==0) displayFlashAlarm(select, Alarm2Time, 0);
				if(Alarm2Time[2]>0x23) Alarm2Time[2] = 0x20;
				select++;
			}
		}
		if(UpKey == 0)
		{
			for(i=0; i<5; i++) displayFlashAlarm(select, Alarm2Time, 0);
			if(UpKey == 0)
			{
				while(UpKey ==0) displayFlashAlarm(select, Alarm2Time, 0);
				switch (select)
				{
					case 1:
						if(Alarm2Time[2]/0x10 < 0x02) Alarm2Time[2] += 0x10;
						else Alarm2Time[2] %= 0x10;
						break;
					case 2:
						if(Alarm2Time[2]%0x10 < 0x09) Alarm2Time[2] += 0x01;
						else Alarm2Time[2] = Alarm2Time[2]/0x10*0x10;
						if(Alarm2Time[2] > 0x23) Alarm2Time[2] = 0x20;
						break;
					case 3:
						if(Alarm2Time[1]/0x10 < 0x05) Alarm2Time[1] += 0x10;
						else Alarm2Time[1] %= 0x10;
						break;
					case 4:
						if(Alarm2Time[1]%0x10 < 0x09) Alarm2Time[1] += 0x01;
						else Alarm2Time[1] = Alarm2Time[1]/0x10*0x10;
						break;
				}
			}
		}
		if(DownKey == 0)
		{
			for(i=0; i<5; i++) displayFlashAlarm(select, Alarm2Time, 0);
			if(DownKey == 0)
			{
				while(DownKey ==0) displayFlashAlarm(select, Alarm2Time, 0);
				switch (select)
				{
					case 1:
						if(Alarm2Time[2]/0x10 == 0x00) Alarm2Time[2] += 0x20;
						else Alarm2Time[2] -= 0x10;
						break;
					case 2:
						if(Alarm2Time[2]%0x10 == 0x00) Alarm2Time[2] += 0x09;
						else Alarm2Time[2] -= 0x01;
						if(Alarm2Time[2] > 0x23) Alarm2Time[2] = 0x23;
						break;
					case 3:
						if(Alarm2Time[1]/0x10 == 0x00) Alarm2Time[1] += 0x50;
						else Alarm2Time[1] -= 0x10;
						break;
					case 4:
						if(Alarm2Time[1]%0x10 == 0x00) Alarm2Time[1] += 0x09;
						else Alarm2Time[1] -= 0x01;
						break;
				}
			}
		}
	}
	AlarmLED2 = 1;
	enAlarm2 = 1;
}

void showYear(void)
{
	unsigned char i;
	for(i=0; i<255; i++) displayFlashAlarm(0, TIME, 1);
}

void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0xCD;				//设置定时初始值
	TH0 = 0xD4;				//设置定时初始值
	TF0 = 0;					//清除TF0标志
	ET0 = 1;
	EA = 1;
	//TR0 = 1;				//定时器0开始计时
}

void tm0_isr() interrupt 1 using 1
{
	TL0 = 0xCD;				//设置定时初始值
	TH0 = 0xD4;				//设置定时初始值
	if(flashCount<30) flashCount++;
	else
	{
		flashCount = 0;
		flashFlag = !flashFlag;
	}
}

void tm1_isr() interrupt 3 using 1
{
	Beep=!Beep;
	TH1=T[m][0]; TL1=T[m][1];
}

void AlarmSync(void)
{
	AlarmLED1 = enAlarm1;
	AlarmLED2 = enAlarm2;
	if(enAlarm1 &&
		 TIME[0] == 0x00 &&
		 Alarm1Time[1] == TIME[1] &&
	   Alarm1Time[2] == TIME[2]) AlarmBeep();
	if(enAlarm2 &&
		 TIME[0] == 0x00 &&
		 Alarm2Time[1] == TIME[1] &&
	   Alarm2Time[2] == TIME[2]) AlarmBeep();
}

void AlarmBeep(void)    //闹钟响铃
{
	unsigned char i, j, k;
	bit flag = 1;
	while(flag)
	{
		for(i=0; i<4; i++)
		{
			for(j=0; j<50; j++)
			{
				Beep = !Beep;
				for(k=0; k<15; k++) 
				{
					Delay50us();
					if(!(Alarm1Key && Alarm2Key && SetKey && UpKey && DownKey && BoomKey)) flag = 0;
					if(!flag) break;
				}
				if(!(Alarm1Key && Alarm2Key && SetKey && UpKey && DownKey && BoomKey)) flag = 0;
				if(!flag) break;
			}
			for(j=0; j<4; j++)
			{
				DisplayRealTime();
			}
			if(!(Alarm1Key && Alarm2Key && SetKey && UpKey && DownKey && BoomKey)) flag = 0;
			if(!flag) break;
		}
		for(i=0; i<20; i++)
		{
			DisplayRealTime();
			if(!(Alarm1Key && Alarm2Key && SetKey && UpKey && DownKey && BoomKey)) flag = 0;
			if(!flag) break;
		}
	}
	Beep = 1;
	for(i=0; i<10; i++) Delay50us();
}

void setTime(void)
{
	unsigned char i;
	unsigned char select = 1;
	while(select<11)
	{
		if(select <9) displayFlashAlarm(select, TIME, 0);
		if(select >8) displayFlashAlarm(select-6, TIME, 1);
		if(SetKey ==0)
		{
			if(select < 9) for(i=0; i<10; i++) displayFlashAlarm(select, TIME, 0);
			if(select > 8) for(i=0; i<10; i++) displayFlashAlarm(select, TIME, 1);
			if(SetKey ==0)
			{
				while(SetKey ==0)
				{
					if(select < 9) for(i=0; i<10; i++) displayFlashAlarm(select, TIME, 0);
					if(select > 8) for(i=0; i<10; i++) displayFlashAlarm(select, TIME, 1);
				}
				if(TIME[2] > 0x23) TIME[2] = 0x20;
				if(TIME[4] > 0x12) TIME[4] = 0x10;
				if(TIME[4] == 0x00) TIME[4] = 0x01;
				if(TIME[3] == 0x00) TIME[3] = 0x01;
				if(TIME[3] > 0x31 && (TIME[4] == 0x01 ||    //大月31天
														TIME[4] == 0x03 ||
														TIME[4] == 0x05 ||
														TIME[4] == 0x07 ||
														TIME[4] == 0x08 ||
														TIME[4] == 0x10 ||
														TIME[4] == 0x12)) TIME[3] = 0x30;
				if(TIME[3]>0x30 && (TIME[4] == 0x04 ||    //小月30天
														TIME[4] == 0x06 ||
														TIME[4] == 0x09 ||
														TIME[4] == 0x11)) TIME[3] = 0x30;
				if(TIME[3]>0x29 && TIME[4] == 0x02 && TIME[6]%4 == 0) TIME[3] = 0x20;    //闰二月29天
				if(TIME[3]>0x28 && TIME[4] == 0x02 && TIME[6]%4 != 0) TIME[3] = 0x20;    //平二月28天
				select++;
			}
		}
		if(UpKey == 0)
		{
			if(select < 9) for(i=0; i<10; i++) displayFlashAlarm(select, TIME, 0);
			if(select > 8) for(i=0; i<10; i++) displayFlashAlarm(select, TIME, 1);
			if(UpKey == 0)
			{
				if(select < 9) while(UpKey ==0) displayFlashAlarm(select, TIME, 0);
				if(select > 8) while(UpKey ==0) displayFlashAlarm(select, TIME, 1);
				switch (select)
				{
					case 1:    //设置小时十位
						if(TIME[2]/0x10 < 0x02) TIME[2] += 0x10;
						else TIME[2] %= 0x10;
						break;
					case 2:    //设置小时个位
						if(TIME[2]%0x10 < 0x09) TIME[2] += 0x01;
						else TIME[2] = TIME[2]/0x10*0x10;
						if(TIME[2] > 0x23) TIME[2] = 0x20;
						break;
					case 3:    //设置分钟十位
						if(TIME[1]/0x10 < 0x05) TIME[1] += 0x10;
						else TIME[1] %= 0x10;
						break;
					case 4:    //设置分钟个位
						if(TIME[1]%0x10 < 0x09) TIME[1] += 0x01;
						else TIME[1] = TIME[1]/0x10*0x10;
						break;
					case 5:    //设置月份十位
						if(TIME[4]/0x10 < 0x01) TIME[4] += 0x10;
						else TIME[4] %= 0x10;
						break;
					case 6:    //设置月份个位
						if(TIME[4]%0x10 < 0x09) TIME[4] += 0x01;
						else TIME[4] = TIME[4]/0x10*0x10;
						if(TIME[4] > 0x12) TIME[4] = 0x10;
						if(TIME[4] == 0x00) TIME[4] = 0x01;
						break;
					case 7:    //设置日期十位
						if(TIME[3]/0x10 < 0x03) TIME[3] += 0x10;
						else TIME[3] %= 0x10;
						break;
					case 8:    //设置日期个位
						if(TIME[3]%0x10 < 0x09) TIME[3] +=0x01;
						else TIME[3] = TIME[3]/0x10*0x10;
						if(TIME[3] == 0x00) TIME[3] = 0x01;
						if(TIME[3]>0x31 && (TIME[4] == 0x01 ||    //大月31天
																TIME[4] == 0x03 ||
																TIME[4] == 0x05 ||
																TIME[4] == 0x07 ||
																TIME[4] == 0x08 ||
																TIME[4] == 0x10 ||
																TIME[4] == 0x12)) TIME[3] = 0x30;
						if(TIME[3]>0x30 && (TIME[4] == 0x04 ||    //小月30天
																TIME[4] == 0x06 ||
																TIME[4] == 0x09 ||
																TIME[4] == 0x11)) TIME[3] = 0x30;
						if(TIME[3]>0x29 && TIME[4] == 0x02 && TIME[6]%4 == 0) TIME[3] = 0x20;    //闰二月29天
						if(TIME[3]>0x28 && TIME[4] == 0x02 && TIME[6]%4 != 0) TIME[3] = 0x20;    //平二月28天
						break;
					case 9:    //设置年10位
						if(TIME[6]/0x10 <0x09) TIME[6] += 0x10;
						else TIME[6] %= 0x10;
						break;
					case 10:    //设置年个位
						if(TIME[6]%0x10 <0x09) TIME[6] += 0x01;
						else TIME[6] = TIME[6]/0x10*0x10;
						break;
				}
			}
		}
		if(DownKey == 0)
		{
			if(select < 9) for(i=0; i<10; i++) displayFlashAlarm(select, TIME, 0);
			if(select > 8) for(i=0; i<10; i++) displayFlashAlarm(select, TIME, 1);
			if(DownKey == 0)
			{
				if(select < 9) while(UpKey ==0) displayFlashAlarm(select, TIME, 0);
				if(select > 8) while(UpKey ==0) displayFlashAlarm(select, TIME, 1);
				switch (select)
				{
					case 1:    //设置小时十位
						if(TIME[2]/0x10 == 0x00) TIME[2] += 0x20;
						else TIME[2] -= 0x10;
						break;
					case 2:    //设置小时个位
						if(TIME[2]%0x10 == 0x00) TIME[2] += 0x09;
						else TIME[2] -= 0x01;
						if(TIME[2] > 0x23) TIME[2] = 0x23;
						break;
					case 3:    //设置分钟十位
						if(TIME[1]/0x10 == 0x00) TIME[1] += 0x50;
						else TIME[1] -= 0x10;
						break;
					case 4:    //设置分钟个位
						if(TIME[1]%0x10 == 0x00) TIME[1] += 0x09;
						else TIME[1] -= 0x01;
						break;
					case 5:    //设置月份十位
						if(TIME[4]/0x10 == 0x00) TIME[4] += 0x10;
						else TIME[4] -= 0x10;
						break;
					case 6:    //设置月份个位
						if(TIME[4]%0x10 == 0x00) TIME[4] += 0x09;
						else TIME[4] -= 0x01;
						if(TIME[4] > 0x12) TIME[4] = 0x10;
						if(TIME[4] == 0x00) TIME[4] = 0x09;
						break;
					case 7:    //设置日期十位
						if(TIME[3]/0x10 == 0x00)
							if(TIME[4] == 0x02) TIME[3] += 0x20;
							else TIME[3] += 0x30;
						else TIME[3] -= 0x10;
						break;
					case 8:    //设置日期个位
						if(TIME[3]%0x10 == 0x00) TIME[3] += 0x09;
						else TIME[3] -= 0x01;
						if(TIME[3] == 0x00) TIME[3] = 0x09;
						if(TIME[3]>0x31 && (TIME[4] == 0x01 ||    //大月31天
																TIME[4] == 0x03 ||
																TIME[4] == 0x05 ||
																TIME[4] == 0x07 ||
																TIME[4] == 0x08 ||
																TIME[4] == 0x10 ||
																TIME[4] == 0x12)) TIME[3] = 0x31;
						if(TIME[3]>0x30 && (TIME[4] == 0x04 ||    //小月30天
																TIME[4] == 0x06 ||
																TIME[4] == 0x09 ||
																TIME[4] == 0x11)) TIME[3] = 0x30;
						if(TIME[3]>0x29 && TIME[4] == 0x02 && TIME[6]%4 == 0) TIME[3] = 0x29;    //闰二月29天
						if(TIME[3]>0x28 && TIME[4] == 0x02 && TIME[6]%4 != 0) TIME[3] = 0x28;    //平二月28天
						break;
					case 9:    //设置年10位
						if(TIME[6]/0x10 == 0x00) TIME[6] += 0x10;
						else TIME[6] -= 0x10;
						break;
					case 10:    //设置年个位
						if(TIME[6]%0x10 == 0x00) TIME[6] += 0x09;
						else TIME[6] -= 0x01;
						break;
				}
			}
		}
	}
	TIME[0] = 0x00;
	ds1302WriteTime();
}

void music_delay(unsigned char p)
{
	unsigned char i,j; 
	for(;p>0;p--)
	for(i=151;i>0;i--)
	for(j=21;j>0;j--)
	{
		music_display();
	}
}

void music_pause()
{
	unsigned char i,j;
	for(i=120;i>0;i--)
	for(j=1;j>0;j--)
	{
		music_display();
	}
}

void music_display()
{
	P0 = 0x00;
	P2 = 0xFE;
	P0 = 0x7C;
	
	P0 = 0x00;
	P2 = 0xF1;
	P0 = 0x5C;
	
	P0 = 0x00;
}

void PlayMusic()
{
	unsigned char i=0; 
	TMOD&=0x0F;  TMOD|=0x10;  EA=1; ET1=1;
	while(1)
	{
		m=music[i][0];n=music[i][1]/2;
		if(m==0x00)
		{
			TR1=0;
			music_delay(n);
			i++;
		}
		else if(m==0xFF)
		{
			TR1=0;
			Beep=1;
			return;
		}
		else if(m==music[i+1][0])
		{
			TR1=1;
			music_delay(n);
			TR1=0;
			music_pause();
			i++;
		}
		else
		{
			TR1=1;
			music_delay(n);
			i++;
		}
	}
}

#endif
