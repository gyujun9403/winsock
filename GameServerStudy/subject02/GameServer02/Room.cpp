#include <algorithm>
#include <cstring>
#include <wchar.h>

#include "NetLib/ILog.h"
#include "NetLib/TcpNetwork.h"
#include "Packet.h"
#include "ErrorCode.h"
#include "User.h"
#include "Room.h"
#include <iostream>


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
			// TODO: 테스트를 위해서라도 함수/클래스를 세분화 할 필요 있음.
			// TODO: SendData하나만을 위해 Network객체를 가지고 있을 필요는 없다. 함수포인터 등을 이용해서 해당 함수만 가져오거나... 싱글톤? 아무튼 전역에서 사용할 수 있게...
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
	//memcpy(pktRes.UserID, other->GetId().c_str(), other->GetId().size());
	for (User* element : this->m_UserList)
	{
		//if (element != other)
		//{
			this->m_pRefNetwork->SendData(element->GetSessionIndex(), (short)PACKET_ID::ROOM_CHAT_NTF, sizeof(PktRoomChatNtf), (char*)&pktRes);
		//}
	}
}