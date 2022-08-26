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

void Game::LeaveGame(User* user)
{
	if (gameStatus == GAMESTATUS::RUNNING)
	{
		if (user == p1)
		{
			MakeWin(p2);
		}
		else if (user == p2)
		{
			MakeWin(p1);
		}
	}
	if (p1 == user)
	{
		ClearBoard();
		p1 = nullptr;
	}
	else if (p2 == user)
	{
		ClearBoard();
		p2 = nullptr;
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
	if (p1Ready == true)
	{
		p1Ready = false;
		this->SendReadyNtf(p1->GetSessionIndex(), false);
	}
	if (p2Ready == true)
	{
		p2Ready = false;
		this->SendReadyNtf(p2->GetSessionIndex(), false);
	}
	this->turn = true;
	cntStone = 0;
	gameStatus = GAMESTATUS::WAITING;
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
	if (this->gameStatus != GAMESTATUS::WAITING)
	{
		code = ERROR_CODE::OMOK_CANT_READY;
	}
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
		}
		else if (user == p2)
		{
			this->p2Ready = isReady;
			this->SendReadyRes(user->GetSessionIndex(), isReady, code);
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

// Èæµ¹ÀÌ ¸ÕÀúµÒ-> false»óÅÂÀÏ¶§ 
bool Game::PlaceStone(User* user, int32_t x, int32_t y)
{
	ERROR_CODE code = ERROR_CODE::NONE;

	if (gameStatus != GAMESTATUS::RUNNING)
	{
		return false;
	}
	else if (this->p1 == nullptr || this->p2 == nullptr)
	{
		return false;
	}
	else if (x < 0 || x > 19 || y < 0 || y > 19)
	{
		return false;
	}
	else if (this->board[x][y] == 0)
	{
		if (user == this->p1 && turn == true)
		{
			this->board[x][y] = 1;
			SendPlaceStoneNtf(user->GetSessionIndex(), x, y, p1StoneColor);
			SendPlaceStoneNtf(p2->GetSessionIndex(), x, y, p1StoneColor);
			SendTurnNtf(p2);
			turn = false;

		}
		else if (user == this->p2 && turn == false)
		{
			this->board[x][y] = 2;
			SendPlaceStoneNtf(p1->GetSessionIndex(), x, y, p2StoneColor);
			SendPlaceStoneNtf(user->GetSessionIndex(), x, y, p2StoneColor);
			SendTurnNtf(p1);
			turn = true;
		}
		cntStone++;
	}
	else
	{
		return false;
	}
	//TODO: not your turn, valid place -> 
	SendPlaceStoneRes(user->GetSessionIndex(), code);
	return true;
}

int16_t Game::AxisCheck(int16_t x, int16_t y, int16_t xUnitOffset, int16_t yUnitOffset, int16_t color)
{
	int16_t newX = 0;
	int16_t newY = 0;
	int16_t reVal = 0;

	for (int16_t i = 0; i < 5; i++)
	{
		newX = x + i * xUnitOffset;
		newY = y + i * yUnitOffset;
		if (newX < 0 || newX > 19 || newY < 0 || newY > 19 ||
			board[newX][newY] != color)
		{
			break;
		}
		else
		{
			++reVal;
		}
	}
	for (int16_t i = 1; i < 5; i++)
	{
		newX = x - i * xUnitOffset;
		newY = y - i * yUnitOffset;
		if (newX < 0 || newX > 19 || newY < 0 || newY > 19 ||
			board[newX][newY] != color)
		{
			break;
		}
		else
		{
			++reVal;
		}
	}
	return reVal;
}

void Game::AnalyzeBoard(User* user, int16_t x, int16_t y)
{
	int16_t color;

	if (user == p1)
	{
		color = 1;
	}
	else
	{
		color = 2;
	}
	if (AxisCheck(x, y, 1, 0, color) >= 5 || AxisCheck(x, y, 0, 1, color) >= 5 ||
		AxisCheck(x, y, 1, 1, color) >= 5 || AxisCheck(x, y, -1, 1, color) >= 5)
	{
		ClearBoard();
		MakeWin(user);
	}
}

void Game::MakeWin(User* user)
{
	PktGameResultNtf pktNtfP1;
	PktGameResultNtf pktNtfP2;

	if (p1 == user)
	{
		pktNtfP1.isWin = true;
		pktNtfP2.isWin = false;
	}
	else if (p2 == user)
	{
		pktNtfP1.isWin = false;
		pktNtfP2.isWin = true;
	}
	else
	{
		return;
	}
	this->network->SendData(p1->GetSessionIndex(), (short)PACKET_ID::OMOK_RESULT_NTF, sizeof(PktGameResultNtf), (char*)&pktNtfP1);
	this->network->SendData(p2->GetSessionIndex(), (short)PACKET_ID::OMOK_RESULT_NTF, sizeof(PktGameResultNtf), (char*)&pktNtfP2);
}

GAMESTATUS Game::getGameStatus()
{
	return this->gameStatus;
}
