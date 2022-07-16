#pragma comment(lib, "ws2_32")
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <error_handle.h>
#include <stdio.h>
#include <cstdlib>
#include <string>

#define	IP_SERVER	"127.0.0.1"
#define PORT_SERVER	9000
#define BUFSIZE		1024

void	init_addr(SOCKADDR_IN &addr)
{
	int retval;

	ZeroMemory(&addr, sizeof(addr));
	retval = inet_pton(AF_INET, IP_SERVER, &addr.sin_addr);
	if (retval == -1) err_quit("inet_pton");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_SERVER);
}

int main()
{
	int retval;
	WSADATA wsa;
	SOCKET	sock;
	SOCKADDR_IN	addr_server;
	unsigned int	len_data;
	char buf[BUFSIZE];
	std::string filename;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	init_addr(addr_server);
	retval = connect(sock, (SOCKADDR*)&addr_server, sizeof(addr_server));
	if (retval == INVALID_SOCKET) err_quit("connect()");
	while (1)
	{
		// 파일명 입력받음
		std::cout << "fine name >";
		std::getline(std::cin, filename);
		if (filename.empty())
			break;
		std::fstream fstream(filename, std::fstream::in);
		if (!fstream.is_open())
		{
			std::cout << "file open error" << std::endl;
			return 0;
		}
		//파일명 전송
		len_data = filename.length();
		retval = send(sock, (char*)&len_data, sizeof(len_data), 0);
		if (retval == INVALID_SOCKET)
		{
			err_display("send()");
			closesocket(sock);
			WSACleanup();
			return 1;
		}
		retval = send(sock, (char*)filename.data(), len_data, 0);
		if (retval == INVALID_SOCKET)
		{
			err_display("send()");
			closesocket(sock);
			WSACleanup();
			return 1;
		}
		//파일 내용 전송
		while (fstream)
		{
			fstream.read(buf, BUFSIZE);
			len_data = fstream.gcount();
			retval = send(sock, (char*)&len_data, sizeof(len_data), 0);
			if (retval == INVALID_SOCKET)
			{
				err_display("send()");
				break;
			}
			retval = send(sock, buf, len_data, 0);
			if (retval == INVALID_SOCKET)
			{
				err_display("send()");
				break;
			}
			std::cout << retval << "byte를 전송." << std::endl;
		}
		break;
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}