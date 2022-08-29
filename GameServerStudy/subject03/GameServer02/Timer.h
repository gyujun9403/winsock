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
	RoomManager* m_RoomMgr;
	uint16_t m_limitTime;
	uint16_t m_countTime;
	std::atomic<bool> isRun = true;
};

