#include <thread>
#include <chrono>

#include "NetLib/Define.h"
#include "NetLib/ServerNetErrorCode.h"
#include "NetLib/TcpNetWork.h"
#include "ConsoleLogger.h"
#include "RoomManager.h"
#include "PacketProcess.h"
#include "UserManager.h"
#include "Server.h"

using NET_ERROR_CODE = NServerNetLib::NET_ERROR_CODE;
using LOG_TYPE = NServerNetLib::LOG_TYPE;

Server::Server()
{
}

Server::~Server()
{
    this->Release();
}

ERROR_CODE Server::Init()
{
    this->logger_ = std::make_unique<ConsoleLog>();
    this->LoadConfig();
    this->network_ = std::make_unique<NServerNetLib::TcpNetwork>();
    auto result = this->network_->Init(this->serverConfig_.get(), this->logger_.get());
    if (result != NET_ERROR_CODE::NONE)
    {
        this->logger_->Write(LOG_TYPE::L_ERROR, "%s | Init Fail. NetErrorCode(%s)", __FUNCTION__, (short)result);
        return ERROR_CODE::MAIN_INIT_NETWORK_INIT_FAIL;
    }
    this->userMgr_ = std::make_unique<UserManager>();
    this->userMgr_->Init(this->serverConfig_->MaxClientCount);
    this->roomMgr_ = std::make_unique<RoomManager>();
    this->roomMgr_->Init(this->serverConfig_->MaxRoomCount, this->serverConfig_->MaxRoomUserCount);
    this->roomMgr_->SetNetwork(this->network_.get(), this->logger_.get()); // 네트워크를 받아오고, 로거를 받아옴 객체지향 참고.
    this->packetProc_ = std::make_unique<PacketProcess>();
    this->packetProc_->Init(this->network_.get(), this->userMgr_.get(), this->roomMgr_.get(), this->serverConfig_.get(), this->logger_.get());

    this->isRun_ = true;
    this->logger_->Write(LOG_TYPE::L_INFO, "%s | Init Success. Server Run", __FUNCTION__);
    return ERROR_CODE::NONE;
}


void Server::Run()
{
    while (this->isRun_)
    {
        this->network_->Run();

        while (true)
        {
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

RoomManager* Server::getRoomMangerInServer()
{
    return roomMgr_.get();
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
