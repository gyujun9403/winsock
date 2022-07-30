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
	//using �� �ʿ䰡 �ֳ�?
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
	std::cout << "id :" << reqPkt->szID << std::endl;
	if (addRet != ERROR_CODE::NONE)
	{
		resPkt.SetError(addRet);
		// ���� ����
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&resPkt);
		// ������Ȳ ��ȯ
		return addRet;
	}
	// ���� ������ �������µ� �����ߴٸ� �α��� -> ������ �ٸ� ���� ����
	else
	{
		resPkt.ErrorCode = (short)addRet;
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&resPkt);
		return ERROR_CODE::NONE;
	}

}

ERROR_CODE PacketProcess::EnterRoom(PacketInfo packetInfo)
{
	PktLogInRes pktRes; // ����� �α� ��Ŷ.
	auto reqPkt = reinterpret_cast<PktRoomEnterReq*>(packetInfo.pRefData);
	auto room = this->refRoomMgr_->GetRoom(reqPkt->RoomIndex);
	// ���� ������ ���� ������ ������ �䱸�Ѵٸ�??? 
	std::cout << "before : " << reqPkt->RoomIndex << std::endl;
	if (room == nullptr)
	{
		pktRes.SetError(ERROR_CODE::ROOM_MGR_INVALID_INDEX);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&pktRes);
		// this->refNetwork_->SendData
		return ERROR_CODE::ROOM_MGR_INVALID_INDEX;
	}
	else
	{
		//this->refLogger_->Write(LOG_TYPE::L_INFO, "%d", __FUNCTION__, reqPkt->RoomIndex);
		if (room->GetUserCount() >= room->MaxUserCount())
		{
			pktRes.SetError(ERROR_CODE::ROOM_FULL);
			this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&pktRes);
			// this->refNetwork_->SendData
			return ERROR_CODE::ROOM_FULL;
		}
		else
		{
			User* user = std::get<1>(this->refUserMgr_->GetUser(packetInfo.SessionIndex));
			if (user == nullptr)
			{
				pktRes.SetError(ERROR_CODE::USER_MGR_NOT_CONFIRM_USER);
				this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&pktRes);
				return ERROR_CODE::ROOM_NOT_LOGIN_USER;
			}
			else if (room->IsUserAlreadyEntered(user))
			{
				pktRes.SetError(ERROR_CODE::ROOM_ALREADY_ENTERED_ROOM);
				this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&pktRes);
				return ERROR_CODE::ROOM_ALREADY_ENTERED_ROOM;
			}
			else
			{
				pktRes.SetError(ERROR_CODE::NONE);
				this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&pktRes);
				room->Enter(user);
				// this->refNetwork_->SendData
				return ERROR_CODE::NONE;
			}
		}
	}
}

ERROR_CODE PacketProcess::LeaveRoom(PacketInfo packetInfo)
{
	PktLogInRes pktRes;
	// �޴� ���� : ���µ�?
	// ���� ã�� ��� -> �Ŵ������� ������ �� ã�� �Լ� ����
	User* user = std::get<1>(this->refUserMgr_->GetUser(packetInfo.SessionIndex));
	if (user == nullptr)
	{
		pktRes.SetError(ERROR_CODE::ROOM_NOT_LOGIN_USER);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&pktRes);
		return ERROR_CODE::ROOM_NOT_LOGIN_USER;
	}
	Room* userEnteredRoom = this->refRoomMgr_->GetRoomUserEntered(user);
	if (userEnteredRoom == nullptr)
	{
		pktRes.SetError(ERROR_CODE::ROOM_NO_ROOM_TO_LEAVE);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&pktRes);
		return ERROR_CODE::ROOM_NO_ROOM_TO_LEAVE;
	}
	else
	{
		userEnteredRoom->Leave(user);
		pktRes.SetError(ERROR_CODE::NONE);
		this->refNetwork_->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&pktRes);
		return ERROR_CODE::NONE;
	}
	//������ ��� �������� �޼������
}