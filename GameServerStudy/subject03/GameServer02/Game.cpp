#include "Game.h"
#include <string.h>

// 이친구는 패킷으로 인해 불리는 친구가 이니므로.. 예외처리는 크게 하지 않는다 ->  이친구를 부를때는 이미 밖에서 확인을 다 했음.
// 이렇게 하지 말고... ReadyGame를 통합해서, ready 한 친구만 넘겨주게 하는 건? ready 취소 할 때는 nullptr
ERROR_CODE Game::NewGame(User* p1, User* P2)
{
	this->p1 = p1;
	this->p2 = p2;
	this->gameStatus = GAMESTATUS::WAITING;
	return ERROR_CODE::NONE;
}

void Game::ResetBoard()
{
	memset(this->board, 0, sizeof(this->board));
	this->turn != this->turn;
}

ERROR_CODE Game::ReadyGame(User* user)
{
	// 유저 인덱스가 맞는지(방에 포함되어 있는지) -> 이건 밖에서 확인하기. 안해도 됨.
	// 유저가 레디 가능한 상태인지 -> 게임중/대기중 불가. 아이콘을 disable로 만드는건?

	return ERROR_CODE();
}

ERROR_CODE Game::PlaceStone(User* user, int32_t x, int32_t y)
{
	if (this->board[x][y] == 0)
	{
		if (user == this->p1)
		{
			this->board[x][y] = 1;
			// Ntf반환
		}
		else if (user == this->p2)
		{
			this->board[x][y] = 2;
			// Ntf반환
		}
		else
		{
			//TODO: this->network->SendData()
			return ERROR_CODE::USER_MGR_NOT_CONFIRM_USER;
		}
	}
	else
	{
		// 이미 놓아진 자리 애러
		// TODO: return;
	}
	return ERROR_CODE::NONE;
}

bool Game::AnalyzeBoard()
{
	// 승리 조건 만족시 return true;
	return false;
}

void Game::MakeWin(User* user)
{
}

//bool Game::IsGaming()
//{
//	return this->isGaming;
//}
