#include "User.h"

User::User()
{
}

User::~User()
{
}

void User::Init(const short index)
{
	this->index_ = index;
}

// 각 유저를 맨 처음 값(점유 안하는 값)으로 초기화.
void User::Clear()
{
	this->sessionIndex_ = 0;
	this->id_ = "";
	this->isConfirm_ = false;
	this->curDomainState_ = DOMAIN_STATE::NONE;
	this->roomIndex_ = -1;
}

void User::Set(const int SessionIndex, const char* pdzID)
{
	this->isConfirm_ = true;
	this->curDomainState_ = DOMAIN_STATE::LOGIN;
	this->sessionIndex_ = SessionIndex;
	this->id_ = pdzID;
}

short User::GetIndex()
{
	return this->index_;
}

int User::GetSessionIndex()
{
	return this->sessionIndex_;
}

std::string& User::GetId()
{
	return this->id_;
}

bool User::IsConfirm()
{
	return this->isConfirm_;
}

short User::GetRoomIndex()
{
	return this->roomIndex_;
}

bool User::IsCurDomainInLogIn()
{
	return this->curDomainState_ == DOMAIN_STATE::LOGIN ? true : false;
}

bool User::IsCurDomainInRoom()
{
	return this->curDomainState_ == DOMAIN_STATE::ROOM ? true : false;
}

void User::EnterRoom(const short roomIndex)
{
	this->roomIndex_ = roomIndex;
	this->curDomainState_ = DOMAIN_STATE::ROOM;
}
