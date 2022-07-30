#include <cstring>
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