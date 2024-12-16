#ifndef _BoomClock_h_
#define _BoomClock_h_

#include <reg51.h>
#include <intrins.h>

/*******************************************
*                 引脚声明                  *
*******************************************/
//按钮
sbit Alarm1Key = P1^0;		//闹钟1，短按开关闹钟，长按设置闹钟时间，进入设置后短按跳转下一位
sbit Alarm2Key = P1^1;		//闹钟2，短按开关闹钟，长按设置闹钟时间，进入设置后短按跳转下一位
sbit SetKey = P1^2;				//短按显示年份，长按设置时间，进入设置后短按跳转下一位
sbit UpKey = P1^3;				//短按时间增加
sbit DownKey = P1^4;			//短按时间减少
sbit Signal1 = P1^5;			//插上闹钟1启用，闹钟1响铃时断开接线关闭响铃，闹钟2响铃时断开接线出发Boom
sbit Signal2 = P1^6;			//插上闹钟2启用，闹钟2响铃时断开接线关闭响铃，闹钟1响铃时断开接线出发Boom
sbit BoomKey = P1^7;			//短按爆炸，长按调节亮度

//LED
sbit AlarmLED1 = P3^2;    //闹钟1的指示灯
sbit AlarmLED2 = P3^3;    //闹钟2的指示灯

//蜂鸣器
sbit Beep = P3^7;

/*******************************************
*                 变量声明                  *
*******************************************/
extern unsigned char Brightness;		//亮度，越大越亮
extern unsigned char flashCount;		//闪烁显示用，结合定时器，0.8s闪一次
extern bit flashFlag;
extern bit enAlarm1;		//闹钟1标志位，0闹钟关，1闹钟开
extern bit enAlarm2;		//闹钟2标志位，0闹钟关，1闹钟开
extern unsigned char Alarm1Time[7];    //闹钟1的时间，格式与ds1302相同
extern unsigned char Alarm2Time[7];    //闹钟2的时间，格式与ds1302相同


/*******************************************
*                 函数声明                  *
*******************************************/
void InitBoomClock(void);			//初始化，主要初始化DS1302
void Delay50us(void);					//延时50us，@11.0592MHz
void DisplayTime(unsigned char* date, bit isFlash);				//不从DS1302读取，仅仅显示时间，isFlash==1时钟冒号闪烁，反之常亮
void DisplayRealTime(void);		//从DS302读取时间并显示
void ScanKeys(void);					//扫描按钮
void BrightnessCtrl(unsigned char val);		//亮度控制，数码管显示延时，val*1ms
void AlarmSync(void);		//同步闹钟标志与LED灯，判断是否到达闹钟时间，判断日期是否越界
void Timer0_Init(void);		//定时器初始化，100微秒@11.0592MHz
void displayFlashAlarm(unsigned char select, unsigned char* date, bit isYear);    //显示闪烁的时间，select表示第几位闪烁，date表示时间来源，isYear为1时间位显示20xx年，如果21xx年作者仍然建在会更新程序到21xx年，具体更新几百年取决于作者生命力
void tm0_isr();    //中断函数
void AlarmBeep(void);    //闹钟响铃
void Boom_Display(unsigned int i);    //彩蛋中显示数字

//按钮事件
void showYear(void);			//显示年份
void setTime(void);				//设置时间
void reverseAlarm1(void);	//反转闹钟1开关
void reverseAlarm2(void);	//反转闹钟2开关
void setAlarm1(void);			//设置闹钟1
void setAlarm2(void);			//设置闹钟2
void setBrightness(void);	//设置亮度
void Boom(void);					//小彩蛋

/*******************************************
*                 歌曲声明                  *
*******************************************/
void music_delay(unsigned char p);
void music_pause();
void music_display();
void PlayMusic(void);    //播放音乐

#endif
