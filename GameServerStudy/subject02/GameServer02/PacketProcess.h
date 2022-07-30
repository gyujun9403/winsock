#pragma once
#include <memory>
#include "Packet.h"
#include "ErrorCode.h"
#include "NetLib/Define.h"

// ���� NServerNetLib���� ���ԵǾ� ���� ������, �̰� ���� �����ͼ� ���Խ��� ���� ������?
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
// �Լ�
public:
	// ������ �Ҹ���
	PacketProcess();
	~PacketProcess();
	// �ʱ�ȭ
	void Init(TcpNet* network, UserManager* userMgr, RoomManager* roomMgr,
		NServerNetLib::ServerConfig* config, ILog* logger);
	// ���� ��Ŷ�� �Ӽ��� ���� ���� ������ �����ϴ� �Լ�
	void Process(PacketInfo packetInfo);
	// Process���� ������ ���� ������.
	ERROR_CODE NtfSysConnectSession(PacketInfo packetInfo);
	ERROR_CODE NtfSysCloseSession(PacketInfo packetInfo);
	ERROR_CODE Login(PacketInfo packetInfo);
	ERROR_CODE EnterRoom(PacketInfo packetInfo);
	ERROR_CODE LeaveRoom(PacketInfo packetInfo);
	ERROR_CODE ChatInRoom(PacketInfo packetInfo);
// ������
public:
	ILog* refLogger_;
	TcpNet* refNetwork_;
	UserManager* refUserMgr_;
	RoomManager* refRoomMgr_;
};

