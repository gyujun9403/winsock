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
	void ResetBoard(); // ������ ����
	void ReadyGame(User* user, bool isReady); // ���� ����. -> �� �� ����,
	void PlaceStone(User* user, int32_t x, int32_t y);
	bool AnalyzeBoard(); // true��ȯ�� ���� ����.
	void MakeWin(User* user); // ���� ������, �׻�� �¸��ϰ� ����.
	GAMESTATUS getGameStatus();
	//bool IsGaming(); // ���������� �ƴ��� ǥ��.
private:
// Ÿ�̸� �ʿ��ҵ�.
	User* p1;
	User* p2;
	short board[18][18]; 
	GAMESTATUS gameStatus;
	bool color; // false : 1p, ture : 2p
	Network* network;
	void SendReadyRes(int sessionIndex, ERROR_CODE code);
	void SendReadyNtf(int sessionIndex, bool isReady);
	void SendPlaceStoneNtf(int sessionIndex, int32_t x, int32_t y, bool color);
	void SendPlaceStoneRes(int sessionIndex, ERROR_CODE code);
};

