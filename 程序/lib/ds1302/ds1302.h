#ifndef _ds1302_h_
#define _ds1302_h_

#include <reg51.h>
#include<intrins.h>

unsigned char code READ_RTC_ADDR[8] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d, 0xc1};     //秒 分 时 日 月 周 年 初始化标志
unsigned char code WRITE_RTC_ADDR[8] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0xc0};    //秒 分 时 日 月 周 年 初始化标志

extern unsigned char TIME[] = {0, 0, 0x20, 0x20, 0x01, 0x05, 0x23};    //00秒 00分 20时 01日 20月 周五 23年

/*
sbit ds1302_io = P3^4;
sbit ds1302_ce = P3^5;
sbit ds1302_sclk = P3^6;
*/

sbit ds1302_io = P3^5;
sbit ds1302_ce = P3^6;
sbit ds1302_sclk = P3^4;


void ds1302Write(unsigned char addr, unsigned char dat)
{
	unsigned char n;
	ds1302_ce = 0;
	_nop_();

	ds1302_sclk = 0;//先将SCLK置低电平。
	_nop_();
	ds1302_ce = 1; //然后将RST(CE)置高电平。
	_nop_();

	for (n=0; n<8; n++)//开始传送八位地址命令
	{
		ds1302_io = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		ds1302_sclk = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		ds1302_sclk = 0;
		_nop_();
	}
	for (n=0; n<8; n++)//写入8位数据
	{
		ds1302_io = dat & 0x01;
		dat >>= 1;
		ds1302_sclk = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		ds1302_sclk = 0;
		_nop_();	
	}	
		 
	ds1302_ce = 0;//传送数据结束
	_nop_();
}

unsigned char ds1302Read(unsigned char addr)
{
	unsigned char n, dat, dat1;
	ds1302_ce = 0;
	_nop_();

	ds1302_sclk = 0;//先将SCLK置低电平。
	_nop_();
	ds1302_ce = 1;//然后将RST(CE)置高电平。
	_nop_();

	for(n=0; n<8; n++)//开始传送八位地址命令
	{
		ds1302_io = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		ds1302_sclk = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		ds1302_sclk = 0;//DS1302下降沿时，放置数据
		_nop_();
	}
	_nop_();
	for(n=0; n<8; n++)//读取8位数据
	{
		dat1 = ds1302_io;//从最低位开始接收
		dat = (dat>>1) | (dat1<<7);
		ds1302_sclk = 1;
		_nop_();
		ds1302_sclk = 0;//DS1302下降沿时，放置数据
		_nop_();
	}

	ds1302_ce = 0;
	_nop_();	//以下为DS1302复位的稳定时间,必须的。
	ds1302_sclk = 1;
	_nop_();
	ds1302_io = 0;
	_nop_();
	ds1302_io = 1;
	_nop_();
	return dat;
}	

void ds1302ReadTime()
{
	unsigned char n;
	for (n=0; n<7; n++)//读取7个字节的时钟信号：分秒时日月周年
	{
		TIME[n] = ds1302Read(READ_RTC_ADDR[n]);
	}
}	

void ds1302WriteTime()
{
	unsigned char n;
	ds1302Write(0x8E,0X00);		 //禁止写保护，就是关闭写保护功能
	for(n=0; n<7; n++)//写入7个字节的时钟信号：分秒时日月周年
	{
		ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	ds1302Write(0x8E,0x80);		 //打开写保护功能
}

void ds1302Init()    //初始化DS1302，防止长时间掉电时间跑飞，原理为在RAM中设置时间标记，若有标记则不初始化，若无标记则初始化并写标记
{
	if(ds1302Read(READ_RTC_ADDR[7])!=0x3a)
	{
		ds1302WriteTime();
		ds1302Write(0x8E,0X00);		 //禁止写保护，就是关闭写保护功能
		ds1302Write(WRITE_RTC_ADDR[7],0x3a);	//写入标志
		ds1302Write(0x8E,0x80);		 //打开写保护功能
	}
}

#endif
