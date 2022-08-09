#include <cstring>
#include <iostream>
#include "PacketProcess.h"
#include "NetLib/ILog.h"
#include "NetLib/TcpNetwork.h"
#include "UserManager.h"
#include "Room.h"
#include "RoomManager.h"

using LOG_TYPE = NServerNetLib::LOG_TYPE;
using ServerConfig = NServerNetLib::ServerConfig;

PacketProcess::PacketProcess()
{
}

PacketProcess::~PacketProcess()
{
}

void PacketProcess::Init(TcpNet* network, UserManager* userMgr, RoomManager* roomMgr, NServerNetLib::ServerConfig* config, ILog* logger)
{
	//using 쓸 필요가 있나?
	this->refLogger_ = logger;
	this->refNetwork_ = network;
	this->refUserMgr_ = userMgr;
	this->refRoomMgr_ = roomMgr;
}

void PacketProcess::Process(PacketInfo packetInfo)
{
	using netLibPacketId = NServerNetLib::PACKET_ID;
	using commonPacketId = PACKET_ID;
	auto packetId = packetInfo.PacketId;

	switch (packetId)
	{
		case static_cast<int>(netLibPacketId::NTF_SYS_CONNECT_SESSION):
			this->NtfSysConnectSession(packetInfo);
			break;
		case static_cast<int>(netLibPacketId::NTF_SYS_CLOSE_SESSION):
			this->NtfSysCloseSession(packetInfo);
			break;
		case static_cast<int>(commonPacketId::LOGIN_IN_REQ):
			this->Login(packetInfo);
			break;
		case static_cast<int>(commonPacketId::ROOM_ENTER_REQ):
			this->EnterRoom(packetInfo);
			break;
		case static_cast<int>(commonPacketId::ROOM_LEAVE_REQ):
			this->LeaveRoom(packetInfo);
			break;
		case static_cast<int>(commonPacketId::ROOM_CHAT_REQ):
			this->ChatInRoom(packetInfo);
			break;
		case static_cast<int>(commonPacketId::OMOK_PLACE_STONE_REQ):
			this->OmokPlaceStone(packetInfo);
			break;
	}

}

ERROR_CODE PacketProcess::NtfSysConnectSession(PacketInfo packetInfo)
{
	this->refLogger_->Write(LOG_TYPE::L_INFO, "%s | NtfSysConnctSession. sessionIndex(%d)", __FUNCTION__, packetInfo.SessionIndex);
	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::NtfSysCloseSession(PacketInfo packetInfo)
{
	// TODO: std::get<1>??
	auto user = std::get<1>(this->refUserMgr_->GetUser(packetInfo.SessionIndex));
	if (user)
	{
		this->refUserMgr_->RemeveUser(packetInfo.SessionIndex);
		// Homework
		this->LeaveRoom(packetInfo);
	}
	else
	{
		this->refLogger_->Write(LOG_TYPE::L_INFO, "%s | NtfSysCloseSesson. sessionIndex(%d)", __FUNCTION__, packetInfo.SessionIndex);
	}
	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::Login(PacketInfo packetInfo)
{
	PktLogInRes resPkt;
	auto reqPkt = (PktLogInReq*)packetInfo.pRefData;
	auto addRet = this->refUserMgr_->AddUser(packetInfo.SessionIndex, reqPkt->szID);
	if (addRet != ERROR_CODE::NONE)
	{
		resPkt.SetError(addRet);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&resPkt);
		return addRet;
	}
	else
	{
		resPkt.ErrorCode = (short)addRet;
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&resPkt);
		return ERROR_CODE::NONE;
	}

}

ERROR_CODE PacketProcess::EnterRoom(PacketInfo packetInfo)
{
	PktRoomEnterRes pktRes; // 응답용 로그 패킷.
	auto reqPkt = reinterpret_cast<PktRoomEnterReq*>(packetInfo.pRefData);
	auto room = this->refRoomMgr_->GetRoom(reqPkt->RoomIndex);
	if (room == nullptr)
	{
		pktRes.SetError(ERROR_CODE::ROOM_MGR_INVALID_INDEX);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(PktRoomEnterRes), (char*)&pktRes);
		return ERROR_CODE::ROOM_MGR_INVALID_INDEX;
	}
	else
	{
		if (room->GetUserCount() >= room->MaxUserCount())
		{
			pktRes.SetError(ERROR_CODE::ROOM_FULL);
			this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(PktRoomEnterRes), (char*)&pktRes);
			return ERROR_CODE::ROOM_FULL;
		}
		else
		{
			User* user = std::get<1>(this->refUserMgr_->GetUser(packetInfo.SessionIndex));
			if (user == nullptr)
			{
				pktRes.SetError(ERROR_CODE::USER_MGR_NOT_CONFIRM_USER);
				this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(PktRoomEnterRes), (char*)&pktRes);
				return ERROR_CODE::ROOM_NOT_LOGIN_USER;
			}
			else if (room->IsUserAlreadyEntered(user))
			{
				pktRes.SetError(ERROR_CODE::ROOM_ALREADY_ENTERED_ROOM);
				this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(PktRoomEnterRes), (char*)&pktRes);
				return ERROR_CODE::ROOM_ALREADY_ENTERED_ROOM;
			}
			else
			{
				pktRes.SetError(ERROR_CODE::NONE);
				pktRes.UniqueId = user->GetIndex();
				this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(PktRoomEnterRes), (char*)&pktRes);
				room->Enter(user);
				room->BroadCastOtherJoin(user);
				room->GetListOtherUser(user);
				return ERROR_CODE::NONE;
			}
		}
	}
}

ERROR_CODE PacketProcess::LeaveRoom(PacketInfo packetInfo)
{
	PktRoomLeaveRes pktRes;
	if (std::get<0>(this->refUserMgr_->GetUser(packetInfo.SessionIndex)) == ERROR_CODE::USER_MGR_INVALID_SESSION_INDEX)
	{
		pktRes.SetError(ERROR_CODE::ROOM_NOT_LOGIN_USER);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_LEAVE_RES, sizeof(PktRoomLeaveRes), (char*)&pktRes);
		return ERROR_CODE::ROOM_NOT_LOGIN_USER;
	}
	User* user = std::get<1>(this->refUserMgr_->GetUser(packetInfo.SessionIndex));
	Room* userEnteredRoom = this->refRoomMgr_->GetRoomUserEntered(user);
	if (userEnteredRoom == nullptr)
	{
		pktRes.SetError(ERROR_CODE::ROOM_NO_ROOM_TO_LEAVE);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_LEAVE_RES, sizeof(PktRoomLeaveRes), (char*)&pktRes);
		return ERROR_CODE::ROOM_NO_ROOM_TO_LEAVE;
	}
	else
	{
		userEnteredRoom->BroadCastOtherLeave(user);
		pktRes.SetError(ERROR_CODE::NONE);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_LEAVE_RES, sizeof(PktRoomLeaveRes), (char*)&pktRes);
		userEnteredRoom->Leave(user);
		return ERROR_CODE::NONE;
	}
}

ERROR_CODE PacketProcess::ChatInRoom(PacketInfo packetInfo)
{
	PktRoomChatRes pktRes;

	PktRoomChatReq* pktReq = reinterpret_cast<PktRoomChatReq*>(packetInfo.pRefData);
	User* user = std::get<1>(this->refUserMgr_->GetUser(packetInfo.SessionIndex));
	if (user == nullptr)
	{
		pktRes.SetError(ERROR_CODE::ROOM_NOT_LOGIN_USER);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CHAT_RES, sizeof(PktRoomChatRes), (char*)&pktRes);
		return ERROR_CODE::ROOM_NOT_LOGIN_USER;
	}
	Room* room = this->refRoomMgr_->GetRoomUserEntered(user);
	if (room == nullptr)
	{
		pktRes.SetError(ERROR_CODE::ROOM_NO_ROOM_TO_CHAT);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CHAT_RES, sizeof(PktRoomChatRes), (char*)&pktRes);
		return ERROR_CODE::ROOM_NO_ROOM_TO_CHAT;
	}
	std::string Msg;
	Msg.assign(pktReq->Msg, 0, pktReq->Msglen);
	room->BroadCastOtherChat(user, Msg);
	pktRes.SetError(ERROR_CODE::NONE);
	this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CHAT_RES, sizeof(PktRoomChatRes), (char*)&pktRes);
	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::OmokPlaceStone(PacketInfo packetInfo)
{
	PktPlaceStoneReq* pktReq = reinterpret_cast<PktPlaceStoneReq*>(packetInfo.pRefData);
	PktPlaceStoneRes pktRes;
	PktPlaceStoneNtf pktNtf;
	pktRes.SetError(ERROR_CODE::NONE);
	this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::OMOK_PLACE_STONE_RES, sizeof(PktPlaceStoneRes), (char*)&pktRes);
	pktNtf.color = true;
	pktNtf.x = pktReq->x;
	pktNtf.y = pktReq->y;
	this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::OMOK_PLACE_STONE_NTF, sizeof(PktPlaceStoneNtf), (char*)&pktNtf);
	return ERROR_CODE::NONE;
}
