#include <algorithm>
#include <cstring>
#include <wchar.h>
#include <iostream>

#include "NetLib/ILog.h"
#include "NetLib/TcpNetwork.h"
#include "Packet.h"
#include "ErrorCode.h"
#include "User.h"
#include "Room.h"
#include "Game.h"


Room::Room() {}

Room::~Room()
{
	delete(this->m_pGame);
}
	
void Room::Init(const short index, const short maxUserCount)
{
	m_Index = index;
	m_MaxUserCount = maxUserCount;
	m_pGame = new Game();
}

void Room::SetNetwork(TcpNet* pNetwork, ILog* pLogger)
{
	m_pRefLogger = pLogger;
	m_pRefNetwork = pNetwork;
	this->m_pGame->setNetwork(pNetwork);
}

void Room::Clear()
{
	m_IsUsed = false;
	m_UserList.clear();
}

void Room::BroadCastOtherJoin(User* other)
{
	PktRoomEnterUserInfoNtf pktRes;

	pktRes.uniqueId = other->GetIndex();
	pktRes.idlen = (char)other->GetId().size();
	memcpy(pktRes.UserID, other->GetId().c_str(), pktRes.idlen);

	for (User* element : this->m_UserList)
	{
		if (element != other)
		{
			this->m_pRefNetwork->SendData(element->GetSessionIndex(), (short)PACKET_ID::ROOM_ENTER_NEW_USER_, sizeof(PktRoomEnterUserInfoNtf), (char*)&pktRes);
		}
	}
}

void Room::GetListOtherUser(User* other)
{
	PktRoomEnterUserInfoNtf pktRes;

	for (User* element : this->m_UserList)
	{
		pktRes.uniqueId = element->GetIndex();
		pktRes.idlen = (char)element->GetId().size();
		memcpy(pktRes.UserID, element->GetId().c_str(), pktRes.idlen);
		this->m_pRefNetwork->SendData(other->GetSessionIndex(), (short)PACKET_ID::ROOM_ENTER_NEW_USER_, sizeof(PktRoomEnterUserInfoNtf), (char*)&pktRes);
	}
}

void Room::OmokReadyUser(User* user, bool isReady)
{
	this->m_pGame->ReadyGame(user, isReady);
}

void Room::OmokPlaceStone(User* user, int32_t x, int32_t y)
{
	int16_t winner;
	this->m_pGame->PlaceStone(user, x, y);
	winner = this->m_pGame->AnalyzeBoard();
	if (winner != 0)
	{
		this->BroadCastResult(winner);
		this->m_pGame->ClearBoard();
	}
	PktGameResultNtf pktNtf;
}

void Room::OmokLeaveOther(User* user)
{
}

void Room::BroadCastOtherLeave(User* other)
{
	PktRoomLeaveUserInfoNtf pktRes;

	pktRes.uniqueId = other->GetIndex();
	pktRes.idlen = other->GetId().size();
	memcpy(pktRes.UserID, other->GetId().c_str(), pktRes.idlen);
	for (User* element : this->m_UserList)
	{
		//if (element != other)
		//{
			this->m_pRefNetwork->SendData(element->GetSessionIndex(), (short)PACKET_ID::ROOM_LEAVE_USER_NTF, sizeof(PktRoomLeaveUserInfoNtf), (char*)&pktRes);
		//}
	}
}

void Room::BroadCastOtherChat(User* other, std::string msg)
{
	PktRoomChatNtf pktRes;

	pktRes.uniqueId = other->GetIndex();
	pktRes.msgLen = msg.size();
	memcpy(pktRes.Msg, msg.c_str(), msg.size());
	for (User* element : this->m_UserList)
	{
		this->m_pRefNetwork->SendData(element->GetSessionIndex(), (short)PACKET_ID::ROOM_CHAT_NTF, sizeof(PktRoomChatNtf), (char*)&pktRes);
	}
}

void Room::BroadCastResult(int16_t winner)
{
	PktGameResultNtf pktNtf;

	pktNtf.result = winner;
	for (User* element : this->m_UserList)
	{
		this->m_pRefNetwork->SendData(element->GetSessionIndex(), (short)PACKET_ID::OMOK_RESULT_NTF, sizeof(PktGameResultNtf), (char*)&pktNtf);
	}
}