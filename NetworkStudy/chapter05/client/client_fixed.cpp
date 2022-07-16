#pragma comment(lib, "ws2_32")
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <error_handle.h>
#include <stdio.h>
#include <cstdlib>

//#define IP_SERVER	"127.0.0.1"
#define PORT_SERVER	9000
#define BUFSIZE		50

int main()
{
	int retval;
	char buf[BUFSIZE];
	const char* IP_SERVER = "127.0.0.1";
	const char* testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���õ��� �� �̾߱Ⱑ ���� �� ���׿�",
		"���� �׷��׿�."
	};
	SOCKET	sock;
	SOCKADDR_IN	addr_server;
	ZeroMemory(&addr_server, sizeof(addr_server));
	//addr_server.sin_addr.s_addr = inet_addr(IP_SERVER);
	retval = inet_pton(AF_INET, IP_SERVER, &addr_server.sin_addr);
	if (retval == -1) err_quit("inet_pton");
	//printf("%lu\n", addr_server.sin_addr);
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(PORT_SERVER);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	retval = connect(sock, (SOCKADDR *)&addr_server, sizeof(addr_server));
	if (retval == INVALID_SOCKET) err_quit("connect()");
	for (int i = 0; i < 4; i++)
	{
		memset(buf, '#', sizeof(buf));
		strncpy(buf, testdata[i], strlen(testdata[i]));

		retval = send(sock, buf, BUFSIZE, 0);
		if (retval == INVALID_SOCKET)
		{
			err_display("send()");
			break;
		}
		std::cout << "[TCP Ŭ���̾�Ʈ] " << retval << "byte�� ���½��ϴ�." << std::endl;
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}