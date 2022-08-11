#include <string.h>
#include "Game.h"
#include "User.h"
#include "Packet.h"
#include "PacketID.h"

void Game::ResetBoard()
{
	memset(this->board, 0, sizeof(this->board));
	this->color != this->color;
	this->gameStatus = GAMESTATUS::WAITING;
	this->p1 = nullptr;
	this->p2 = nullptr;
}

void Game::SendReadyRes(int sessionIndex, ERROR_CODE code)
{
	PktReadyRes pktRes;

	pktRes.SetError(code);
	this->network->SendData(sessionIndex, (short)PACKET_ID::OMOK_PLACE_STONE_RES, sizeof(PktPlaceStoneRes), (char*)&pktRes);
}

void Game::SendReadyNtf(int sessionIndex, bool isReady)
{
	PktReadyNtf pktNtf;

	pktNtf.uniqueId = sessionIndex;
	pktNtf.isReady = isReady;
	this->network->SendData(sessionIndex, (short)PACKET_ID::OMOK_READY_NTF, sizeof(PktReadyNtf), (char*)&pktNtf);
}

void Game::ReadyGame(User* user, bool isReady)
{
	ERROR_CODE code = ERROR_CODE::NONE;
	// ���� �ε����� �´���(�濡 ���ԵǾ� �ִ���) -> �̰� �ۿ��� Ȯ���ϱ�. ���ص� ��.
	// ������ ���� ������ �������� -> ������/����� �Ұ�. �������� disable�� ����°�?
	if (this->gameStatus != GAMESTATUS::WAITING)
	{
		// waiting ���°� �ƴѵ� ready�� �� -> ������ ����.
		code = ERROR_CODE::OMOK_CANT_READY;
	}
	else
	{
		if (p1 != nullptr)
		{
			p1 = user;
		}
		else
		{
			p2 = user;
			this->gameStatus = GAMESTATUS::RUNNING;
		}
		this->SendReadyNtf(user->GetSessionIndex(), isReady);
	}
	this->SendReadyRes(user->GetSessionIndex(), code);
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
	pktNtf.color = true;
	pktNtf.x = x;
	pktNtf.y = y;
	this->network->SendData(sessionIndex, (short)PACKET_ID::OMOK_PLACE_STONE_NTF, sizeof(PktPlaceStoneNtf), (char*)&pktNtf);
}

void Game::PlaceStone(User* user, int32_t x, int32_t y)
{
	ERROR_CODE code = ERROR_CODE::NONE;

	if (this->board[x][y] == 0)
	{
		if (user == this->p1)
		{
			this->board[x][y] = 1;
			SendPlaceStoneNtf(user->GetSessionIndex(), x, y, true);
		}
		else if (user == this->p2)
		{
			this->board[x][y] = 2;
			SendPlaceStoneNtf(user->GetSessionIndex(), x, y, false);
		}
		//else
		//{
		//	//TODO: this->network->SendData();
		//	return ERROR_CODE::USER_MGR_NOT_CONFIRM_USER;
		//} ->  ¥ġ  room���� Ȯ���ؼ� �����ٰ���.
	}
	else
	{
		code = ERROR_CODE::OMOK_PLACE_OCCUFIED;
	}
	SendPlaceStoneRes(user->GetSessionIndex(), code);
}

bool Game::AnalyzeBoard()
{
	// �¸� ���� ������ return true;
	this->gameStatus = GAMESTATUS::SHIFTING;
	return false;
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
