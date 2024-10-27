#include <REG51.H>
#include "Delay.h"
#include "Timer0.h"
 
typedef unsigned int u16;
typedef unsigned char u8;
 
#define SPEED 500		//�����ٶȣ�ֵΪ�ķ�������ʱ��(ms)
 
sbit Buzzer = P2^5; //�������˿ڶ���
 
//������������Ӧ��P����ֹ����L��������M��������H���������»��ߣ�����������#
#define P	0
#define L1	1
#define L1_	2
#define L2	3
#define L2_	4
#define L3	5
#define L4	6
#define L4_	7
#define L5	8
#define L5_	9
#define L6	10
#define L6_	11
#define L7	12
#define M1	13
#define M1_	14
#define M2	15
#define M2_	16
#define M3	17
#define M4	18
#define M4_	19
#define M5	20
#define M5_	21
#define M6	22
#define M6_	23
#define M7	24
#define H1	25
#define H1_	26
#define H2	27
#define H2_	28
#define H3	29
#define H4	30
#define H4_	31
#define H5	32
#define H5_	33
#define H6	34
#define H6_	35
#define H7	36
 
//������Ƶ�ʶ��ձ�
u16 FreqTable[] = {0, 63628,63731,63835,63928,64021,64103,64185,64260,64331,
					  64400,64463,64524,64580,64633,64684,64732,64777,64820,
				      64860,64898,64934,64968,65000,65030,65058,65085,65110,
				      65134,65157,65178,65191,65217,65235,65252,65268,65283};	
//����
u8 code Music[] = {
    //������ ʱֵ��
	P,  2,
	L6, 1,
	L6, 1,
	M3, 2,
	M3, 2,
	
	L6, 4,
	P,  4,
	P,  4,
	P,  2,
	L6, 1,
	
	L6, 1 + 2,
	
	P,  2,
	P,  4, 
	P,  2,
	L6, 1,
	L6, 1,
	M3, 2,
	M3, 2,
	
	L6, 4,
	P,  4,
	P,  4,
	P,  2,
	
	L6, 1 + 2,
	
	P,  2,
	P,  2,
	L6, 1,
	L6, 1,
	L6, 2,
	L6, 1,
	L6, 1,
	M3, 2,
	M3, 2,
	
	L6, 4,
	P,  4,
	P,  4,
	P,  2,
	L6, 1,
	
	L6, 1 + 2,
	
	P,  2,
	P,  4,
	P,  2,
	L6, 1,
	L6, 1,
	M3, 2,
	M3, 2,
	
	L6, 4,
	P,  4,
	P,  4,
	P,  2,
	L6, 1,
	L6, 1,
	
	0xFF	//��ֹ��־
};
 
unsigned char FreqSelect,MusicSelect;
 
void main()
{
	Timer0Init();
	while(1)
	{
		if(Music[MusicSelect]!=0xFF)				//�������ֹͣ��־λ
		{
			FreqSelect=Music[MusicSelect];		//ѡ��������Ӧ��Ƶ��
			MusicSelect++;
			Delay(SPEED/4*Music[MusicSelect]);//ѡ��������Ӧ��ʱֵ
			MusicSelect++;
			TR0=0;
			Delay(5);													//���������ͣ�٣��൱��̧��
			TR0=1;
		}
		else																//�����ֹͣ��־λ
		{
			TR0=0;
			Delay(50);
			TR0 = 1;
			//MusicSelect = 0;									//ͣ�ٺ�ѭ������
		}
	}
}
 
void Timer0_Routine() interrupt 1
{
	if(FreqTable[FreqSelect])							//���������ֹ��
	{
		/*ȡ��ӦƵ��ֵ����װ��ֵ����ʱ��*/
		TL0 = FreqTable[FreqSelect]%256;		//���ö�ʱ��ֵ //�ڶ����ж�ʵ��
		TH0 = FreqTable[FreqSelect]/256;		//���ö�ʱ��ֵ//
		Buzzer=!Buzzer;	//��ת������IO��
	}
}
