#ifndef _ds1302_h_
#define _ds1302_h_

#include <reg51.h>
#include<intrins.h>

unsigned char code READ_RTC_ADDR[8] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d, 0xc1};     //�� �� ʱ �� �� �� �� ��ʼ����־
unsigned char code WRITE_RTC_ADDR[8] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0xc0};    //�� �� ʱ �� �� �� �� ��ʼ����־

extern unsigned char TIME[] = {0, 0, 0x20, 0x20, 0x01, 0x05, 0x23};    //00�� 00�� 20ʱ 01�� 20�� ���� 23��

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

	ds1302_sclk = 0;//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();
	ds1302_ce = 1; //Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();

	for (n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
		ds1302_io = addr & 0x01;//���ݴӵ�λ��ʼ����
		addr >>= 1;
		ds1302_sclk = 1;//������������ʱ��DS1302��ȡ����
		_nop_();
		ds1302_sclk = 0;
		_nop_();
	}
	for (n=0; n<8; n++)//д��8λ����
	{
		ds1302_io = dat & 0x01;
		dat >>= 1;
		ds1302_sclk = 1;//������������ʱ��DS1302��ȡ����
		_nop_();
		ds1302_sclk = 0;
		_nop_();	
	}	
		 
	ds1302_ce = 0;//�������ݽ���
	_nop_();
}

unsigned char ds1302Read(unsigned char addr)
{
	unsigned char n, dat, dat1;
	ds1302_ce = 0;
	_nop_();

	ds1302_sclk = 0;//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();
	ds1302_ce = 1;//Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();

	for(n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
		ds1302_io = addr & 0x01;//���ݴӵ�λ��ʼ����
		addr >>= 1;
		ds1302_sclk = 1;//������������ʱ��DS1302��ȡ����
		_nop_();
		ds1302_sclk = 0;//DS1302�½���ʱ����������
		_nop_();
	}
	_nop_();
	for(n=0; n<8; n++)//��ȡ8λ����
	{
		dat1 = ds1302_io;//�����λ��ʼ����
		dat = (dat>>1) | (dat1<<7);
		ds1302_sclk = 1;
		_nop_();
		ds1302_sclk = 0;//DS1302�½���ʱ����������
		_nop_();
	}

	ds1302_ce = 0;
	_nop_();	//����ΪDS1302��λ���ȶ�ʱ��,����ġ�
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
	for (n=0; n<7; n++)//��ȡ7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		TIME[n] = ds1302Read(READ_RTC_ADDR[n]);
	}
}	

void ds1302WriteTime()
{
	unsigned char n;
	ds1302Write(0x8E,0X00);		 //��ֹд���������ǹر�д��������
	for(n=0; n<7; n++)//д��7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	ds1302Write(0x8E,0x80);		 //��д��������
}

void ds1302Init()    //��ʼ��DS1302����ֹ��ʱ�����ʱ���ܷɣ�ԭ��Ϊ��RAM������ʱ���ǣ����б���򲻳�ʼ�������ޱ�����ʼ����д���
{
	if(ds1302Read(READ_RTC_ADDR[7])!=0x3a)
	{
		ds1302WriteTime();
		ds1302Write(0x8E,0X00);		 //��ֹд���������ǹر�д��������
		ds1302Write(WRITE_RTC_ADDR[7],0x3a);	//д���־
		ds1302Write(0x8E,0x80);		 //��д��������
	}
}

#endif
