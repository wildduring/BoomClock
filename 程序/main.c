#include "BoomClock.h"

void main(void)
{
	InitBoomClock();
	while(1)
	{
		DisplayRealTime();		//显示时间
		ScanKeys();						//扫描按键
		AlarmSync();					//检测闹钟时间和同步闹钟指示状态
	}
}
