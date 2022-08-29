#pragma once

// game�� � Ŭ�������� �ұ�?
// �ϴ� �� �ȿ� ��� ������, �渶�� ���� ��/�鵹���� ���ϴ� ���� -> �� �ε����� ���� ����
// ȣ���ϸ�, ���� ����� Ȯ���ϰ�, �̸� �����ϴ� Ŭ����
// ������ ��ġ ����, ���������� �� ���� ������ gameŬ���� ������ ������ �ʴ´�.
#include <stdint.h>
#include <thread>
#include <mutex>
#include "NetLib/ITcpNetwork.h"
#include "ErrorCode.h"
class User;

using Network = NServerNetLib::ITcpNetwork;

enum class GAMESTATUS : short
{
	WAITING = 0,
	RUNNING = 1,
	SHIFTING = 2
};

class Game
{
public:
	void setNetwork(Network* net);
	void EnterGame(User* user);
	void LeaveGame(User* user);
	void ReadyGame(User* user, bool isReady); // ���� ����. -> �� �� ����,
	bool PlaceStone(User* user, int32_t x, int32_t y);
	void AnalyzeBoard(User* user, int16_t x, int16_t y);
	void TimeOver(User* user);
	//void AnalyzeBoard();
	GAMESTATUS getGameStatus();
	// ���������� Ȯ���� �� �ִ� �Լ�
	bool IsGaming();
	// ���� �� �� ī��Ʈ ���Ҵ��� �����ϴ� �Լ�.
	// -> pair�� �ؼ� ��ȯ?
	void ManageDragingProcess(uint16_t limitTime, uint16_t countTime);
	std::pair<User*, time_t> GetGameP1AndTime();
	std::pair<User*, time_t> GetGameP2AndTime();
	//bool IsGaming(); // ���������� �ƴ��� ǥ��.
	//std::mutex* getMutex();
private:
	void ClearBoard(); // ������ ����
	void MakeWin(User* user); // ���� ����, �׻�� �¸��ϰ� ����.
// Ÿ�̸� �ʿ��ҵ�.
	std::mutex m;
	User* p1;
	User* p2;
	bool p1Ready = false;
	bool p1StoneColor = false;
	bool p2Ready = false;
	bool p2StoneColor = false;
	time_t p1TurnStartTime = 0;
	time_t p2TurnStartTime = 0;
	int16_t board[19][19];
	int16_t cntStone = 0;
	bool turn = false;
	bool whoIsFirst = false; // true�� p1�� black, false�� p2�� black
	GAMESTATUS gameStatus;
	Network* network;
	void SendReadyRes(int sessionIndex, bool isReady, ERROR_CODE code);
	void SendReadyNtf(int sessionIndex, bool isReady);
	void SendTurnNtf(User* turnedUser);
	void SendGameStartNtf();
	void SetWhoIsFirst();
	void SendPlaceStoneNtf(int sessionIndex, int32_t x, int32_t y, bool color);
	void SendPlaceStoneRes(int sessionIndex, ERROR_CODE code);
	
	int16_t AxisCheck(int16_t x, int16_t y, int16_t xOffset, int16_t yOffset, int16_t color);
};

