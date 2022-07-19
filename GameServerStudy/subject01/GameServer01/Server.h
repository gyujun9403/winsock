#pragma once
#include <memory>
#include "Packet.h" // �Ⱦ��°� ������?
#include "ErrorCode.h"

// ������ ������ ������, �ϴ��� �ش� ���ӽ����̽� �ȿ� �̷��� �͵��� �ִٰ� ���漱��
// �ٸ� �������� include�ϸ� ��� ����� �ʹ� Ŀ���� �׷���? �ٵ� pragma once�� �ִµ� �ص� ���� �ȳ�
namespace NServerNetLib
{
	struct ServerConfig;
	class ILog;
	class ITcpNetwork;
}

class UserManager;
class RoomManager;
class PacketProcess;

class Server
{
// �Լ���
public:
	Server();
	~Server();
	ERROR_CODE Init();
	void Run();
	void Stop();
private:
	void Release(); //TODO: ���� �������� �˾ƺ��� -> TcpNetwork -> WSACleanup()
	ERROR_CODE LoadConfig();
// ������
private:
	bool isRun_ = false;
	std::unique_ptr<NServerNetLib::ServerConfig> serverConfig_;
	std::unique_ptr<NServerNetLib::ILog> logger_;
	std::unique_ptr<NServerNetLib::ITcpNetwork> network_;
	std::unique_ptr<PacketProcess> packetProc_;
	std::unique_ptr<UserManager> userMgr_;
	std::unique_ptr<RoomManager> roomMgr_;
};

