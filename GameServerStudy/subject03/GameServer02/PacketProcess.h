#pragma once
#include <memory>
#include "Packet.h"
#include "ErrorCode.h"
#include "NetLib/Define.h"

// 실제 NServerNetLib에는 포함되어 있지 않은데, 이걸 여기 가져와서 포함시켜 놓는 이유는?
namespace NServerNetLib
{
	class ITcpNetwork;
	class ILog;
}

class UserManager;
class RoomManager;

class PacketProcess
{
	using PacketInfo = NServerNetLib::RecvPacketInfo;
	using TcpNet = NServerNetLib::ITcpNetwork;
	using ILog = NServerNetLib::ILog;
// 함수
public:
	// 생성자 소멸자
	PacketProcess();
	~PacketProcess();
	// 초기화
	void Init(TcpNet* network, UserManager* userMgr, RoomManager* roomMgr,
		NServerNetLib::ServerConfig* config, ILog* logger);
	// 받은 패킷의 속서에 따라서 서비스 로직을 실행하는 함수
	void Process(PacketInfo packetInfo);
	// Process에서 실행할 서비스 로직들.
	ERROR_CODE NtfSysConnectSession(PacketInfo packetInfo);
	ERROR_CODE NtfSysCloseSession(PacketInfo packetInfo);
	ERROR_CODE Login(PacketInfo packetInfo);
	ERROR_CODE EnterRoom(PacketInfo packetInfo);
	ERROR_CODE LeaveRoom(PacketInfo packetInfo);
	ERROR_CODE ChatInRoom(PacketInfo packetInfo);
// 데이터
public:
	ILog* refLogger_;
	TcpNet* refNetwork_;
	UserManager* refUserMgr_;
	RoomManager* refRoomMgr_;
};

