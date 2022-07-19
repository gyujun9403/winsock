#pragma once
#include <memory>
#include "Packet.h" // 안쓰는거 같은데?
#include "ErrorCode.h"

// 지금은 정보가 없으니, 일단은 해당 네임스페이스 안에 이러한 것들이 있다고 전방선언
// 다른 헤더들까지 include하면 헤더 사이즈가 너무 커져서 그런가? 근데 pragma once가 있는데 해도 되지 안나
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
// 함수부
public:
	Server();
	~Server();
	ERROR_CODE Init();
	void Run();
	void Stop();
private:
	void Release(); //TODO: 무슨 역할인지 알아보기 -> TcpNetwork -> WSACleanup()
	ERROR_CODE LoadConfig();
// 데이터
private:
	bool isRun_ = false;
	std::unique_ptr<NServerNetLib::ServerConfig> serverConfig_;
	std::unique_ptr<NServerNetLib::ILog> logger_;
	std::unique_ptr<NServerNetLib::ITcpNetwork> network_;
	std::unique_ptr<PacketProcess> packetProc_;
	std::unique_ptr<UserManager> userMgr_;
	std::unique_ptr<RoomManager> roomMgr_;
};

