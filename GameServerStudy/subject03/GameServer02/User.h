#pragma once
#include <string>

class User
{
public:
	enum class DOMAIN_STATE
	{
		NONE = 0,
		LOGIN = 1,
		ROOM = 2
	};
	User();
	virtual ~User();
	void Init(const short index);
	void Clear();
	void Set(const int SessionIndex, const char* pdzID);
	short GetIndex();
	int GetSessionIndex();
	std::string& GetId();
	bool IsConfirm();
	short GetRoomIndex();
	bool IsCurDomainInLogIn(); // TODO: 도메인? 세션?
	bool IsCurDomainInRoom();
	void EnterRoom(const short roomIndex);
protected:
	short index_ = -1;
	int sessionIndex_ = -1;
	std::string id_;
	bool isConfirm_ = false;
	DOMAIN_STATE curDomainState_ = DOMAIN_STATE::NONE;
	short roomIndex_ = -1;
};

