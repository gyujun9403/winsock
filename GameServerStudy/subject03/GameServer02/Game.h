#pragma once

// game은 어떤 클래스여야 할까?
// 일단 룸 안에 들어 있으니, 방마다 누가 흑/백돌일지 정하는 로직 -> 방 인덱스에 따라서 정함
// 호출하면, 승패 결과를 확인하고, 이를 전송하는 클래스
// 오목판 배치 형태, 누구턴인지 등등에 대한 정보는 game클래스 밖으로 나가지 않는다.
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
	ERROR_CODE NewGame(User* p1, User* P2); // 유저세팅. 
	void ResetBoard(); // 오목판 비우기
	ERROR_CODE ReadyGame(User* user); // 게임 시작. -> 둘 다 레디,
	ERROR_CODE UnReadyGame(User* user);
	ERROR_CODE PlaceStone(User* user, int32_t x, int32_t y);
	bool AnalyzeBoard(); // true반환시 방을 리셋.
	void MakeWin(User* user); // 누가 나가면, 그사람 승리하게 만듦.
	GAMESTATUS getGameStatus();
	//bool IsGaming(); // 게임중인지 아닌지 표시.
private:
// 타이머 필요할듯.
	User* p1;
	User* p2;
	short board[18][18]; 
	GAMESTATUS gameStatus;
	bool color; // false : 1p, ture : 2p
	Network* network;
};

