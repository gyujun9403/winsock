#include "Game.h"
#include <string.h>

// ��ģ���� ��Ŷ���� ���� �Ҹ��� ģ���� �̴ϹǷ�.. ����ó���� ũ�� ���� �ʴ´� ->  ��ģ���� �θ����� �̹� �ۿ��� Ȯ���� �� ����.
// �̷��� ���� ����... ReadyGame�� �����ؼ�, ready �� ģ���� �Ѱ��ְ� �ϴ� ��? ready ��� �� ���� nullptr
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
	this->gameStatus = GAMESTATUS::WAITING;
	this->p1 = nullptr;
	this->p2 = nullptr;
}

ERROR_CODE Game::ReadyGame(User* user)
{
	// ���� �ε����� �´���(�濡 ���ԵǾ� �ִ���) -> �̰� �ۿ��� Ȯ���ϱ�. ���ص� ��.
	// ������ ���� ������ �������� -> ������/����� �Ұ�. �������� disable�� ����°�?
	if (this->gameStatus != WAITING)
	{
		// waiting ���°� �ƴѵ� ready�� �� -> ������ ����.
		return ERROR_CODE::NONE;
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
		return ERROR_CODE::NONE;
	}
}

ERROR_CODE Game::PlaceStone(User* user, int32_t x, int32_t y)
{
	if (this->board[x][y] == 0)
	{
		if (user == this->p1)
		{
			this->board[x][y] = 1;
			//�� ���� ���� res��ȯ.
			//this->network_->SendData(); �� ���� ntf send. ������ this->turn�� �״�� ����.
			// Ntf��ȯ
		}
		else if (user == this->p2)
		{
			this->board[x][y] = 2;
			//�� ���� ���� res��ȯ.
			//this->network_->SendData(); �� ���� ntf send. ������ this->turn�� ����� ����.
			// Ntf��ȯ
		}
		else
		{
			//TODO: this->network->SendData();
			return ERROR_CODE::USER_MGR_NOT_CONFIRM_USER;
		}
	}
	else
	{
		// �̹� ������ �ڸ� �ַ�
		// TODO: return;
	}
	return ERROR_CODE::NONE;
}

bool Game::AnalyzeBoard()
{
	// �¸� ���� ������ return true;
	this->gameStatus = GAMESTATUS::SHIFING;
	return false;
}

void Game::MakeWin(User* user)
{
}

//bool Game::IsGaming()
//{
//	return this->isGaming;
//}

GAMESTATUS getGameStatus()
{
	return this->gameStatus;
}
