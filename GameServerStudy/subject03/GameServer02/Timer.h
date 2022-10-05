#pragma once

#include "RoomManager.h"

class Timer
{
	// try_lock을 통해서 
public:
	Timer() = delete;
	Timer(RoomManager* serverRoomMgr, uint16_t limitTime, uint16_t countTime);
	void RunTimer();
	void setTimerFinish();
private:
	//
	RoomManager* m_RoomMgr; 
	//FIXME: X select에 타임아웃이 있으니, 서버쪽에서 턴에 대한 정보를 던져줘서(메세지, 이 부분에만 락을 걸어주면됨. 
	//서버에 있는 데이터를 건들지 않고) 타이머에서 처리하게ㄷ한다.
	uint16_t m_limitTime;
	uint16_t m_countTime;
	std::atomic<bool> isRun = true;
};

