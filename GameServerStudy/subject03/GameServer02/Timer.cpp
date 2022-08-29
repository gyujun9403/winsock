#include "timer.h"
//#include <synchapi.h>
#include <Windows.h>

Timer::Timer(RoomManager* serverRoomMgr, uint16_t limitTime, uint16_t countTime)
: m_RoomMgr(serverRoomMgr), m_limitTime(limitTime), m_countTime(countTime)
{

}

void Timer::RunTimer()
{
	while (isRun)
	{
		m_RoomMgr->ManageDraggingUsers(m_limitTime, m_countTime);
		Sleep(200);
	}
}

void Timer::setTimerFinish()
{
	isRun = false;
}
