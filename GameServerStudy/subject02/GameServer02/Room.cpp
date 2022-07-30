#include <algorithm>
#include <cstring>
#include <wchar.h>

#include "NetLib/ILog.h"
#include "NetLib/TcpNetwork.h"
#include "Packet.h"
#include "ErrorCode.h"
#include "User.h"
#include "Room.h"



Room::Room() {}

Room::~Room()
{
}
	
void Room::Init(const short index, const short maxUserCount)
{
	m_Index = index;
	m_MaxUserCount = maxUserCount;
}

void Room::SetNetwork(TcpNet* pNetwork, ILog* pLogger)
{
	m_pRefLogger = pLogger;
	m_pRefNetwork = pNetwork;
}

void Room::Clear()
{
	m_IsUsed = false;
	m_UserList.clear();
}

void Room::BroadCastOtherLeave(User* other)
{
	PktRoomLeaveUserInfoNtf pktRes;

	memcpy(pktRes.UserId, other->GetId().c_str(), other->GetId().size());
	for (User* element : this->m_UserList)
	{
		if (element != other)
		{
			this->m_pRefNetwork->SendData(element->GetSessionIndex(), (short)PACKET_ID::ROOM_LEAVE_USER_NTF, sizeof(PktRoomLeaveRes), (char*)&pktRes);
		}
	}
}

void Room::BroadCastOtherChat(User* other, std::string msg)
{
	PktRoomChatNtf pktRes;

	memcpy(pktRes.Msg, msg.c_str(), msg.size());
	memcpy(pktRes.UserID, other->GetId().c_str(), other->GetId().size());
	for (User* element : this->m_UserList)
	{
		if (element != other)
		{
			this->m_pRefNetwork->SendData(element->GetSessionIndex(), (short)PACKET_ID::ROOM_CHAT_NTF, sizeof(PktRoomChatNtf), (char*)&pktRes);
		}
	}
}