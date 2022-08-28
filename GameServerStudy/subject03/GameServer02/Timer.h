#pragma once

#include "RoomManager.h"

class Timer
{
	// try_lock을 통해서 
public:
	Timer() = delete;
	Timer(RoomManager* serverRoomMgr);
	void RunTimer();
private:
	RoomManager* RoomMgr;
};

