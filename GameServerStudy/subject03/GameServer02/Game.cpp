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
}

ERROR_CODE Game::ReadyGame(User* user)
{
	// ���� �ε����� �´���(�濡 ���ԵǾ� �ִ���) -> �̰� �ۿ��� Ȯ���ϱ�. ���ص� ��.
	// ������ ���� ������ �������� -> ������/����� �Ұ�. �������� disable�� ����°�?

	return ERROR_CODE();
}

ERROR_CODE Game::PlaceStone(User* user, int32_t x, int32_t y)
{
	if (this->board[x][y] == 0)
	{
		if (user == this->p1)
		{
			this->board[x][y] = 1;
			// Ntf��ȯ
		}
		else if (user == this->p2)
		{
			this->board[x][y] = 2;
			// Ntf��ȯ
		}
		else
		{
			//TODO: this->network->SendData()
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
	return false;
}

void Game::MakeWin(User* user)
{
}

//bool Game::IsGaming()
//{
//	return this->isGaming;
//}
