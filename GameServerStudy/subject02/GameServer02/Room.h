#pragma once

#include <vector>
#include <string>
#include <memory>

#include "User.h"


namespace NServerNetLib { class ITcpNetwork; }
namespace NServerNetLib { class ILog; }


	
using TcpNet = NServerNetLib::ITcpNetwork;
using ILog = NServerNetLib::ILog;

class Game;

class Room
{
public:
	Room();
	virtual ~Room();

	void Init(const short index, const short maxUserCount);
	
	void SetNetwork(TcpNet* pNetwork, ILog* pLogger);

	void Clear();
		
	short GetIndex() { return m_Index;  }

	bool IsUsed() { return m_IsUsed; }
		
	short MaxUserCount() { return m_MaxUserCount; }

	short GetUserCount() { return (short)m_UserList.size(); }

	// Home Work
	bool IsUserAlreadyEntered(User* user)
	{
		return (std::find(this->m_UserList.begin(), this->m_UserList.end(), user) != this->m_UserList.end());
	}
	void Enter(User* user) { this->m_UserList.push_back(user); }
	void Leave(User* user) { this->m_UserList.erase(std::find(this->m_UserList.begin(), this->m_UserList.end(), user)); }
	void BroadCastOtherJoin(User* other);
	void BroadCastOtherLeave(User* other);
	void BroadCastOtherChat(User* other, std::string msg);

private:
	ILog* m_pRefLogger;
	TcpNet* m_pRefNetwork;

	short m_Index = -1;
	short m_MaxUserCount;
		
	bool m_IsUsed = false;
	std::vector<User*> m_UserList;

	//Game* m_pGame = nullptr;
};
