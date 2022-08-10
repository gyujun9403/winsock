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

enum class GAMESTATUS
{
	WAITING = 0,
	RUNNING = 1,
	SHIFTING = 2
};

class Game
{
public:
	ERROR_CODE NewGame(User* p1, User* P2); // ��������. 
	void ResetBoard(); // ������ ����
	ERROR_CODE ReadyGame(User* user); // ���� ����. -> �� �� ����, 
	ERROR_CODE PlaceStone(User* user, int32_t x, int32_t y);
	bool AnalyzeBoard(); // true��ȯ�� ���� ����.
	void MakeWin(User* user); // ���� ������, �׻�� �¸��ϰ� ����.
	//bool IsGaming(); // ���������� �ƴ��� ǥ��.
private:
	User* p1;
	User* p2;
	short board[18][18]; 
	GAMESTATUS gameStatus;
	bool turn; // false : 1p, ture : 2p
	Network* network;
};

