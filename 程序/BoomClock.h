#ifndef _BoomClock_h_
#define _BoomClock_h_

#include <reg51.h>
#include <intrins.h>

/*******************************************
*                 ��������                  *
*******************************************/
//��ť
sbit Alarm1Key = P1^0;		//����1���̰��������ӣ�������������ʱ�䣬�������ú�̰���ת��һλ
sbit Alarm2Key = P1^1;		//����2���̰��������ӣ�������������ʱ�䣬�������ú�̰���ת��һλ
sbit SetKey = P1^2;				//�̰���ʾ��ݣ���������ʱ�䣬�������ú�̰���ת��һλ
sbit UpKey = P1^3;				//�̰�ʱ������
sbit DownKey = P1^4;			//�̰�ʱ�����
sbit Signal1 = P1^5;			//��������1���ã�����1����ʱ�Ͽ����߹ر����壬����2����ʱ�Ͽ����߳���Boom
sbit Signal2 = P1^6;			//��������2���ã�����2����ʱ�Ͽ����߹ر����壬����1����ʱ�Ͽ����߳���Boom
sbit BoomKey = P1^7;			//�̰���ը��������������

//LED
sbit AlarmLED1 = P3^2;    //����1��ָʾ��
sbit AlarmLED2 = P3^3;    //����2��ָʾ��

//������
sbit Beep = P3^7;

/*******************************************
*                 ��������                  *
*******************************************/
extern unsigned char Brightness;		//���ȣ�Խ��Խ��
extern unsigned char flashCount;		//��˸��ʾ�ã���϶�ʱ����0.8s��һ��
extern bit flashFlag;
extern bit enAlarm1;		//����1��־λ��0���ӹأ�1���ӿ�
extern bit enAlarm2;		//����2��־λ��0���ӹأ�1���ӿ�
extern unsigned char Alarm1Time[7];    //����1��ʱ�䣬��ʽ��ds1302��ͬ
extern unsigned char Alarm2Time[7];    //����2��ʱ�䣬��ʽ��ds1302��ͬ


/*******************************************
*                 ��������                  *
*******************************************/
void InitBoomClock(void);			//��ʼ������Ҫ��ʼ��DS1302
void Delay50us(void);					//��ʱ50us��@11.0592MHz
void DisplayTime(unsigned char* date, bit isFlash);				//����DS1302��ȡ��������ʾʱ�䣬isFlash==1ʱ��ð����˸����֮����
void DisplayRealTime(void);		//��DS302��ȡʱ�䲢��ʾ
void ScanKeys(void);					//ɨ�谴ť
void BrightnessCtrl(unsigned char val);		//���ȿ��ƣ��������ʾ��ʱ��val*1ms
void AlarmSync(void);		//ͬ�����ӱ�־��LED�ƣ��ж��Ƿ񵽴�����ʱ�䣬�ж������Ƿ�Խ��
void Timer0_Init(void);		//��ʱ����ʼ����100΢��@11.0592MHz
void displayFlashAlarm(unsigned char select, unsigned char* date, bit isYear);    //��ʾ��˸��ʱ�䣬select��ʾ�ڼ�λ��˸��date��ʾʱ����Դ��isYearΪ1ʱ��λ��ʾ20xx�꣬���21xx��������Ȼ���ڻ���³���21xx�꣬������¼�����ȡ��������������
void tm0_isr();    //�жϺ���
void AlarmBeep(void);    //��������
void Boom_Display(unsigned int i);    //�ʵ�����ʾ����

//��ť�¼�
void showYear(void);			//��ʾ���
void setTime(void);				//����ʱ��
void reverseAlarm1(void);	//��ת����1����
void reverseAlarm2(void);	//��ת����2����
void setAlarm1(void);			//��������1
void setAlarm2(void);			//��������2
void setBrightness(void);	//��������
void Boom(void);					//С�ʵ�

/*******************************************
*                 ��������                  *
*******************************************/
void music_delay(unsigned char p);
void music_pause();
void music_display();
void PlayMusic(void);    //��������

#endif
