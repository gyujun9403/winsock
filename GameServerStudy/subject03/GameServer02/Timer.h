#pragma once

#include "RoomManager.h"

class Timer
{
	// try_lock�� ���ؼ� 
public:
	Timer() = delete;
	Timer(RoomManager* serverRoomMgr, uint16_t limitTime, uint16_t countTime);
	void RunTimer();
	void setTimerFinish();
private:
	//
	RoomManager* m_RoomMgr; 
	//FIXME: X select�� Ÿ�Ӿƿ��� ������, �����ʿ��� �Ͽ� ���� ������ �����༭(�޼���, �� �κп��� ���� �ɾ��ָ��. 
	//������ �ִ� �����͸� �ǵ��� �ʰ�) Ÿ�̸ӿ��� ó���ϰԤ��Ѵ�.
	uint16_t m_limitTime;
	uint16_t m_countTime;
	std::atomic<bool> isRun = true;
};

