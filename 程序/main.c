#include "BoomClock.h"

void main(void)
{
	InitBoomClock();
	while(1)
	{
		DisplayRealTime();		//��ʾʱ��
		ScanKeys();						//ɨ�谴��
		AlarmSync();					//�������ʱ���ͬ������ָʾ״̬
	}
}
