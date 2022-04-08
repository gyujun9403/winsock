#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <iostream>
#include "error_handle.h"
#include <string>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

// ����� ���� ������ ���� �Լ�. �� ���� ���ϴ°��� �ѹ� �� �����غ���...
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}
	return (len - left);
}

int main()
{
	int retval;
	char buf[BUFSIZE + 1];
	int len;
	std::string str;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) == SOCKET_ERROR) return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connection()");
	
	while (1)
	{
		std::cout << "\n[���� ������]";
		std::getline(std::cin, str);
		if (str.size() == 0)
			break;

		retval = send(sock, str.data(), str.size(), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		//�Է¹��� ������ ������
		std::cout << "[TCP Ŭ���̾�Ʈ] " << retval << "byte�� ���½��ϴ�." << std::endl;
		//�����κ��� ������ �ޱ�
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}
		else
		{
			buf[retval] = '\0';
			std::cout << "[TCP Ŭ���̾�Ʈ] " << retval << "byte�� �޾ҽ��ϴ�." << std::endl;
			std::cout << "[���� ������] " << buf << std::endl;
		}
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}