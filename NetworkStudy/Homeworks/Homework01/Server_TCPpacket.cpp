#pragma comment(lib, "ws2_32")
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <sstream>
#include "error_handle.h"

//struct PacketData
//{
//	public Int16 DataSize;
//	public Int16 PacketID;
//	public SByte Type;
//	public byte[] BodyData;
//}
#pragma pack(1)
struct header
{
	short DataSize;
	short PacketID;
	// 설마 이거 유니코드 환경이라서 2byte가 된건가? ㅋㅋㅋ;
	char Type;
};

#pragma pack(1)
struct two
{
	int a;
	short op;
	int b;
};

int main()
{
	WSADATA wsa;
	SOCKET sock_listen, sock_client;
	SOCKADDR_IN addr_listen, addr_client;
	const wchar_t IP[] = L"127.0.0.1";
	const u_short PORT = 11021;
	int	len_addrClient;// = sizeof(addr_client);
	int recv_val;
	const int BUFFERSIZE = 512;
	char buffer[BUFFERSIZE];

	char* bufPointer;
	std::string strBuf;
	struct header h;
	struct two eq_two;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quitW(L"Server WSAStartup");
	ZeroMemory(&addr_listen, sizeof(addr_listen));
	addr_listen.sin_family = AF_INET;
	if (InetPtonW(AF_INET, IP, &addr_listen.sin_addr) == 0)
		err_quitW(L"Server SOCKADDR_IN init");
	addr_listen.sin_port = htons(PORT);
	sock_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_listen == INVALID_SOCKET)
		err_quitW(L"Server Socket()");
	if (bind(sock_listen, (SOCKADDR*)&addr_listen, sizeof(addr_listen)) == SOCKET_ERROR)
		err_quitW(L"Server bind()");
	if (listen(sock_listen, SOMAXCONN) == SOCKET_ERROR)
		err_quitW(L"Server listen()");
	len_addrClient = sizeof(addr_client);
	while (1)
	{
		// listen 안열고 했더니 accept에서 에러 발생. -> 잘못된 인수를 입력하였습니다.
		sock_client = accept(sock_listen, (SOCKADDR*)&addr_client, &len_addrClient);
		if (sock_client == INVALID_SOCKET)
			err_quitW(L"Server accept()");
		std::wcout << L"connected" << std::endl;
		while (1)
		{
			recv_val = recv(sock_client, buffer, BUFFERSIZE, 0);
			if (recv_val == SOCKET_ERROR)
				err_displayW(L"server recv()");
			else if (recv_val == 0)
				break;
			else
			{
				bufPointer = buffer;
				if (recv_val > sizeof(h))
					memcpy_s(&h, sizeof(h), bufPointer, sizeof(h));
				else
					memcpy_s(&h, sizeof(h), bufPointer, recv_val);
				bufPointer += (sizeof(header));
				std::cout << "DataSize :" << h.DataSize << ", PacketID :" << h.PacketID << ", Type :" << static_cast<short>(h.Type) << std::endl;
				memcpy_s(&eq_two, sizeof(eq_two), bufPointer, sizeof(eq_two));
				std::cout << "a : " << eq_two.a << ", op : " << eq_two.op << ", b : " << eq_two.b << std::endl;
			}
		}
		std::wcout << L"disconnected" << std::endl;
		closesocket(sock_client);
	}
	closesocket(sock_listen);
	WSACleanup();
}