#pragma once

#include "RoomManager.h"

class Timer
{
	// try_lock�� ���ؼ� 
public:
	Timer() = delete;
	Timer(RoomManager* serverRoomMgr);
	void RunTimer();
private:
	RoomManager* RoomMgr;
};

