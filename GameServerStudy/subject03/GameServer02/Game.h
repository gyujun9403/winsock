#pragma once

// game은 어떤 클래스여야 할까?
// 일단 룸 안에 들어 있으니, 방마다 누가 흑/백돌일지 정하는 로직 -> 방 인덱스에 따라서 정함
// 호출하면, 승패 결과를 확인하고, 이를 전송하는 클래스
// 오목판 배치 형태, 누구턴인지 등등에 대한 정보는 game클래스 밖으로 나가지 않는다.
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
	void ReadyGame(User* user, bool isReady); // 게임 시작. -> 둘 다 레디,
	bool PlaceStone(User* user, int32_t x, int32_t y);
	void AnalyzeBoard(User* user, int16_t x, int16_t y);
	void TimeOver(User* user);
	//void AnalyzeBoard();
	GAMESTATUS getGameStatus();
	// 게임중인지 확인할 수 있는 함수
	bool IsGaming();
	// 누가 몇 초 카운트 남았는지 전달하는 함수.
	// -> pair로 해서 반환?
	void ManageDragingProcess(uint16_t limitTime, uint16_t countTime);
	std::pair<User*, time_t> GetGameP1AndTime();
	std::pair<User*, time_t> GetGameP2AndTime();
	//bool IsGaming(); // 게임중인지 아닌지 표시.
	//std::mutex* getMutex();
private:
	void ClearBoard(); // 오목판 비우기
	void MakeWin(User* user); // 누가 나가, 그사람 승리하게 만듦.
// 타이머 필요할듯.
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
	bool whoIsFirst = false; // true면 p1이 black, false면 p2가 black
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

