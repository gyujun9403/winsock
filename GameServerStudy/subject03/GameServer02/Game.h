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
	void ClearBoard(); // ������ ����
	void ReadyGame(User* user, bool isReady); // ���� ����. -> �� �� ����,
	void PlaceStone(User* user, int32_t x, int32_t y);
	int16_t AnalyzeBoard(); // true��ȯ�� ���� ����.
	void MakeWin(User* user); // ���� ������, �׻�� �¸��ϰ� ����.
	GAMESTATUS getGameStatus();
	//bool IsGaming(); // ���������� �ƴ��� ǥ��.
private:
// Ÿ�̸� �ʿ��ҵ�.
	User* p1;
	User* p2;
	bool p1Ready = false;
	bool p2Ready = false;
	int16_t board[19][19];
	int16_t cntStone = 0;
	bool turn = false;
	GAMESTATUS gameStatus;
	Network* network;
	void SendReadyRes(int sessionIndex, ERROR_CODE code);
	void SendReadyNtf(int sessionIndex, bool isReady);
	void SendPlaceStoneNtf(int sessionIndex, int32_t x, int32_t y, bool color);
	void SendPlaceStoneRes(int sessionIndex, ERROR_CODE code);
};

