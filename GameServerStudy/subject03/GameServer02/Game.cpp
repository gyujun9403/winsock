#include <string.h>
#include <time.h>
#include "Game.h"
#include "User.h"
#include "Packet.h"
#include "PacketID.h"

void Game::setNetwork(Network* net)
{
	this->network = net;
	this->turn = true;
}

void Game::EnterGame(User* user)
{
	if (this->p1 == nullptr)
	{
		this->p1 = user;
	}
	else if (this->p2 == nullptr)
	{
		this->p2 = user;
	}
	else
	{
		;
	}
}

void Game::ClearBoard()
{
	PktGameResetNtf pktNtf;

	memset(this->board, 0, sizeof(this->board));
	this->gameStatus = GAMESTATUS::WAITING;
	this->network->SendData(this->p1->GetSessionIndex(), (short)PACKET_ID::OMOK_GAME_END_NTF, sizeof(PktGameResetNtf), (char*)&pktNtf);
	this->network->SendData(this->p2->GetSessionIndex(), (short)PACKET_ID::OMOK_GAME_END_NTF, sizeof(PktGameResetNtf), (char*)&pktNtf);
	this->p1 = nullptr;
	this->p2 = nullptr;
	this->turn = true;
	cntStone = 0;
}

void Game::SendReadyRes(int sessionIndex, bool isReady,  ERROR_CODE code)
{
	PktReadyRes pktRes;
	
	pktRes.SetError(ERROR_CODE::NONE);
	pktRes.isReady = isReady;
	pktRes.SetError(code);
	this->network->SendData(sessionIndex, (short)PACKET_ID::OMOK_READY_RES, sizeof(PktReadyRes), (char*)&pktRes);
}

void Game::SendReadyNtf(int sessionIndex, bool isReady)
{
	PktReadyNtf pktNtf;

	pktNtf.uniqueId = sessionIndex;
	pktNtf.isReady = isReady;
	this->network->SendData(sessionIndex, (short)PACKET_ID::OMOK_READY_NTF, sizeof(PktReadyNtf), (char*)&pktNtf);
}

void Game::SendGameStartNtf()
{
	PktGameStartNtf pktNtf;

	this->network->SendData(p1->GetSessionIndex(), (short)PACKET_ID::OMOK_GAME_START_NTF, sizeof(PktGameStartNtf), (char*)&pktNtf);
	this->network->SendData(p2->GetSessionIndex(), (short)PACKET_ID::OMOK_GAME_START_NTF, sizeof(PktGameStartNtf), (char*)&pktNtf);
}

void Game::ReadyGame(User* user, bool isReady)
{
	ERROR_CODE code = ERROR_CODE::NONE;
	// 유저 인덱스가 맞는지(방에 포함되어 있는지) -> 이건 밖에서 확인하기. 안해도 됨.
	// 유저가 레디 가능한 상태인지 -> 게임중/대기중 불가. 아이콘을 disable로 만드는건?
	if (this->gameStatus != GAMESTATUS::WAITING)
	{
		// waiting 상태가 아닌데 ready를 함 -> 문제가 있음.
		code = ERROR_CODE::OMOK_CANT_READY;
	}
	// p1 p2모두 있어야 ready가능
	else if (p1 == nullptr || p2 == nullptr)
	{
		;
	}
	else
	{
		if (user == p1)
		{
			this->p1Ready = isReady;
			this->SendReadyRes(user->GetSessionIndex(), isReady, code);
			//this->SendReadyNtf(this->p1->GetSessionIndex(), isReady);
			//this->SendReadyNtf(this->p2->GetSessionIndex(), isReady);
		}
		else if (user == p2)
		{
			this->p2Ready = isReady;
			this->SendReadyRes(user->GetSessionIndex(), isReady, code);
			//this->SendReadyNtf(this->p1->GetSessionIndex(), isReady);
			//this->SendReadyNtf(this->p2->GetSessionIndex(), isReady);

		}
		if (p1Ready == true && p2Ready == true)
		{
			gameStatus = GAMESTATUS::RUNNING;
			SendGameStartNtf();
			SetWhoIsFirst();
			if (whoIsFirst == true)
			{
				SendTurnNtf(p1);
				p1StoneColor = false;
				p2StoneColor = true;
				turn = true;
			}
			else
			{
				SendTurnNtf(p2);
				p1StoneColor = true;
				p2StoneColor = false;
				turn = false;
			}
		}
	}
}

void Game::SetWhoIsFirst()
{
	srand(time(NULL));
	(rand() % 2 == 0) ? whoIsFirst = true : whoIsFirst = false;
}

void Game::SendPlaceStoneRes(int sessionIndex, ERROR_CODE code)
{
	PktPlaceStoneRes pktRes;

	pktRes.SetError(code);
	this->network->SendData(sessionIndex, (short)PACKET_ID::OMOK_PLACE_STONE_RES, sizeof(PktPlaceStoneRes), (char*)&pktRes);
}

void Game::SendPlaceStoneNtf(int sessionIndex, int32_t x, int32_t y, bool color)
{
	PktPlaceStoneNtf pktNtf;
	pktNtf.color = color;
	pktNtf.x = x;
	pktNtf.y = y;
	this->network->SendData(sessionIndex, (short)PACKET_ID::OMOK_PLACE_STONE_NTF, sizeof(PktPlaceStoneNtf), (char*)&pktNtf);
}

void Game::SendTurnNtf(User* turnedUser)
{
	PktTurnNtf pktNtfForP1;
	PktTurnNtf pktNtfForP2;
	
	/*pktNtf.uniqueId = turnedUser->GetSessionIndex();
	pktNtf.idlen = turnedUser->GetId().size();
	memcpy(pktNtf.UserID, turnedUser->GetId().c_str(), pktNtf.idlen);
	this->network->SendData(p1->GetSessionIndex(), (short)PACKET_ID::OMOK_TURN_NTF, sizeof(PktTurnNtf), (char*)&pktNtf);
	this->network->SendData(p2->GetSessionIndex(), (short)PACKET_ID::OMOK_TURN_NTF, sizeof(PktTurnNtf), (char*)&pktNtf);*/
	if (turnedUser == p1)
	{
		pktNtfForP1.yourTurn = true;
		pktNtfForP2.yourTurn = false;
	}
	else
	{
		pktNtfForP1.yourTurn = false;
		pktNtfForP2.yourTurn = true;
	}
	this->network->SendData(p1->GetSessionIndex(), (short)PACKET_ID::OMOK_TURN_NTF, sizeof(PktTurnNtf), (char*)&pktNtfForP1);
	this->network->SendData(p2->GetSessionIndex(), (short)PACKET_ID::OMOK_TURN_NTF, sizeof(PktTurnNtf), (char*)&pktNtfForP2);
}

// 흑돌이 먼저둠-> false상태일때 
void Game::PlaceStone(User* user, int32_t x, int32_t y)
{
	ERROR_CODE code = ERROR_CODE::NONE;

	if (gameStatus != GAMESTATUS::RUNNING)
	{
		return;
	}
	else if (this->p1 == nullptr || this->p2 == nullptr)
	{
		code = ERROR_CODE::OMOK_GAME_NOT_STARTED;
	}
	else if (x < 0 || x > 19 || y < 0 || y > 19)
	{
		code = ERROR_CODE::OMOK_NOT_VALIED_PLACE;
	}
	else if (this->board[x][y] == 0)
	{
		if (user == this->p1 && turn == true)
		{
			this->board[x][y] = 1;
			SendPlaceStoneNtf(user->GetSessionIndex(), x, y, true);
			SendPlaceStoneNtf(p2->GetSessionIndex(), x, y, true);
			SendTurnNtf(p2);
			turn = false;

		}
		else if (user == this->p2 && turn == false)
		{
			this->board[x][y] = 2;
			SendPlaceStoneNtf(p1->GetSessionIndex(), x, y, false);
			SendPlaceStoneNtf(user->GetSessionIndex(), x, y, false);
			SendTurnNtf(p1);
			turn = true;
		}
		cntStone++;
		//else
		//{
		//	//TODO: this->network->SendData();
		//	return ERROR_CODE::USER_MGR_NOT_CONFIRM_USER;
		//} ->  짜치  room에서 확인해서 보내줄거임.
	}
	else
	{
		code = ERROR_CODE::OMOK_PLACE_OCCUFIED;
	}
	SendPlaceStoneRes(user->GetSessionIndex(), code);
}

User* Game::AnalyzeBoard()
{
	// 승리 조건 만족시 return true;
	if (cntStone > 10)
	{

		//this->gameStatus = GAMESTATUS::SHIFTING;
		gameStatus = GAMESTATUS::WAITING;
		if (turn == true)
		{
			
			return p1;

		}
		else
		{
			return p2;
		}
		// 비길시 return -1
		ClearBoard();
	}
	return nullptr;
}

void Game::MakeWin(User* user)
{
}

//bool Game::IsGaming()
//{
//	return this->isGaming;
//}

GAMESTATUS Game::getGameStatus()
{
	return this->gameStatus;
}
