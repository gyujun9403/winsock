#pragma once

// game�� � Ŭ�������� �ұ�?
// �ϴ� �� �ȿ� ��� ������, �渶�� ���� ��/�鵹���� ���ϴ� ���� -> �� �ε����� ���� ����
// ȣ���ϸ�, ���� ����� Ȯ���ϰ�, �̸� �����ϴ� Ŭ����
// ������ ��ġ ����, ���������� �� ���� ������ gameŬ���� ������ ������ �ʴ´�.
#include <stdint.h>
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
	void ClearBoard(); // ������ ����
	void ReadyGame(User* user, bool isReady); // ���� ����. -> �� �� ����,
	bool PlaceStone(User* user, int32_t x, int32_t y);
	void AnalyzeBoard(User* user, int16_t x, int16_t y);
	//void AnalyzeBoard();
	void MakeWin(User* user); // ���� ������, �׻�� �¸��ϰ� ����.
	GAMESTATUS getGameStatus();
	//bool IsGaming(); // ���������� �ƴ��� ǥ��.
private:
// Ÿ�̸� �ʿ��ҵ�.
	User* p1;
	User* p2;
	bool p1Ready = false;
	bool p1StoneColor = false;
	bool p2Ready = false;
	bool p2StoneColor = false;
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

