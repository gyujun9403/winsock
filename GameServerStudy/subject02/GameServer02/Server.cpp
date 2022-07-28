#include <thread>
#include <chrono>

#include "NetLib/Define.h"
#include "NetLib/ServerNetErrorCode.h"
#include "NetLib/TcpNetWork.h"
#include "ConsoleLogger.h"
#include "RoomManager.h"
//#include "Room.h" RoomManager에서 알아서 관리하므로, Server에선 Room 호출할 필요 없다. User도 마찬가지.
#include "PacketProcess.h"
#include "UserManager.h"
#include "Server.h"

using NET_ERROR_CODE = NServerNetLib::NET_ERROR_CODE;
using LOG_TYPE = NServerNetLib::LOG_TYPE;
//template<typename T>
//using std::make_unique<T> unique<T> 정규화된 이름은 사용할 수 없습니다.

Server::Server()
{
}

Server::~Server()
{
    this->Release();
}

ERROR_CODE Server::Init()
{
    this->logger_ = std::make_unique<ConsoleLog>(); // 로그 인스턴스 생성
    this->LoadConfig(); // config load
    this->network_ = std::make_unique<NServerNetLib::TcpNetwork>(); // 통신할 인스턴스 생성. 왜 빨간줄?
    auto result = this->network_->Init(this->serverConfig_.get(), this->logger_.get());
    if (result != NET_ERROR_CODE::NONE)
    {
        this->logger_->Write(LOG_TYPE::L_ERROR, "%s | Init Fail. NetErrorCode(%s)", __FUNCTION__, (short)result);
        return ERROR_CODE::MAIN_INIT_NETWORK_INIT_FAIL;
    }
    // userManager 등등 동적할당 및 초기화
    this->userMgr_ = std::make_unique<UserManager>();
    this->userMgr_->Init(this->serverConfig_->MaxClientCount);
    this->roomMgr_ = std::make_unique<RoomManager>();
    this->roomMgr_->Init(this->serverConfig_->MaxRoomCount, this->serverConfig_->MaxRoomUserCount);
    this->roomMgr_->SetNetwork(this->network_.get(), this->logger_.get()); // 네트워크를 받아오고, 로거를 받아옴 객체지향 참고.
    this->packetProc_ = std::make_unique<PacketProcess>();
    // TODO:C++ 함수 (선언됨 줄 28 /)에 액세스할 수 없습니다. -> private였음...
    this->packetProc_->Init(this->network_.get(), this->userMgr_.get(), this->roomMgr_.get(), this->serverConfig_.get(), this->logger_.get());

    this->isRun_ = true;
    this->logger_->Write(LOG_TYPE::L_INFO, "%s | Init Success. Server Run", __FUNCTION__);
    return ERROR_CODE::NONE;
}


void Server::Run()
{
    while (this->isRun_)
    {
        // 통신 시작
        this->network_->Run();

        while (true)
        {
            // TODO : 좀더 깊이
            auto packetInfo = this->network_->GetPacketInfo();

            if (packetInfo.PacketId == 0)
            {
                break;
            }
            else
            {
                this->packetProc_->Process(packetInfo);
            }
        }
    }
}

void Server::Stop()
{
    this->isRun_ = false;
}

void Server::Release()
{
    if (this->network_)
    {
        this->network_->Release();
    }
}

ERROR_CODE Server::LoadConfig()
{
    
    this->serverConfig_ = std::make_unique<NServerNetLib::ServerConfig>();

    this->serverConfig_->Port = 11021;
    this->serverConfig_->BackLogCount = 128;
    this->serverConfig_->MaxClientCount = 1000;

    this->serverConfig_->MaxClientSockOptRecvBufferSize = 10240;
    this->serverConfig_->MaxClientSockOptSendBufferSize = 10240;
    this->serverConfig_->MaxClientRecvBufferSize = 8192;
    this->serverConfig_->MaxClientSendBufferSize = 8192;

    this->serverConfig_->ExtraClientCount = 64;
    this->serverConfig_->MaxRoomCount = 20;
    this->serverConfig_->MaxRoomUserCount = 4;

    this->logger_->Write(NServerNetLib::LOG_TYPE::L_INFO, "%s | Port(%d), Backlog(%d)", __FUNCTION__, this->serverConfig_->Port, this->serverConfig_->BackLogCount);
    return ERROR_CODE::NONE;
}
