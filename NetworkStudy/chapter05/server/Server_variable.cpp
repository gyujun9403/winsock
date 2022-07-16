#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define SERVERPORT 9000
#define BUFSIZE 512	//
#include "error_handle.h"

int _recv_ahead(SOCKET socket, char* p)
{
	static int nbytes = 0;
	static char buf[1024];
	static char* ptr;

	if (nbytes == 0 || nbytes == SOCKET_ERROR)
	{
		nbytes = recv(socket, buf, sizeof(buf), 0);

		if (nbytes == SOCKET_ERROR || nbytes == 0)
			return nbytes;

		ptr = buf;
	}

	--nbytes;
	*p = *ptr++;
	return 1;
}

int recvline(SOCKET socket, char* buf, int maxLen)
{
	int n, nbytes;
	char c, * ptr = buf;

	for (n = 1; n < maxLen; n++)
	{
		nbytes = _recv_ahead(socket, &c);

		if (nbytes == 1)
		{
			*ptr++ = c;
			if (c == '\n')
				break;
		}
		else if (nbytes == 0)
		{
			*ptr = 0;
			return n - 1;
		}
		else
			return SOCKET_ERROR;
	}

	*ptr = 0;
	return n;
}

int main(int argc, char* argv[])
{
	int retval;
	int addrlen;
	char buf[BUFSIZE + 1];	//
	
	SOCKET listen_sock;
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}
		std::cout << "\n[TCP 서버] 클라이언트 접속 : IP주소=" << inet_ntoa(clientaddr.sin_addr) << "포트번호=" << ntohs(clientaddr.sin_port) << std::endl;
		while (1)
		{
			retval = recvline(client_sock, buf, BUFSIZE + 1);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			else if (retval == 0)
				break;
			else
			{
				//buf[retval] = '\0';
				std::cout << "[TCP / " << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << "] " << buf;// << std::endl;
			}
		}
		closesocket(client_sock);
		std::cout << "\n[TCP 서버] 클라이언트 종료 : IP주소=" << inet_ntoa(clientaddr.sin_addr) << "포트번호=" << ntohs(clientaddr.sin_port) << std::endl;
	}
	closesocket(listen_sock);
	WSACleanup();
	return 0;
}