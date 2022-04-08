#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define SERVERPORT 9000
#define BUFSIZE 512
#include "error_handle.h"

int main(int argc, char* argv[])
{
	int retval; // ��ȯ��
	SOCKADDR_IN serveraddr; //���� �ּ� ����
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	// INADDR_ANY : 
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// listen ���� ����
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	// listen ������ ���ε�(��Ʈ�� ���ϰ� ����)
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	// listen ������ listen~~�ϰ� 
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ���� ����� ���� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	while (1)
	{
		// listen���� ���
		addrlen = sizeof(clientaddr);	//�ݺ����ȿ��� �̰� �ϴ� ������?
		// accept�ϱ� ���� �ʿ��Ѱ� ������ ���� ��ũ����, Ŭ���̾�Ʈ �ּҸ� �޾ƿð�, Ŭ���̾�Ʈ �ּҸ� �������� ũ��
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		// ���Ĵ� listen������ ������ �ʰ�, client������ ���Ѵ�.
		//printf("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP�ּ�=%s, ��Ʈ��ȣ=%s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		// inet_ntoa ���� : 
		std::cout << "\n[TCP ����] Ŭ���̾�Ʈ ���� : IP�ּ�=" << inet_ntoa(clientaddr.sin_addr) << "��Ʈ��ȣ=" << ntohs(clientaddr.sin_port) << std::endl;
		// Ŭ���̾�Ʈ�� ���������� ����ϴ� �ݺ���
		while (1)
		{
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			// ���� ����� ������ -> �ƹ��͵� ���� line�Է½�.
			else if (retval == 0)
				break;
			else
			{
				// Ŭ���̾�Ʈ�κ��� ���� �Է� ���
				buf[retval] = '\0'; // c++������ �Ҽ� �ִ��� ������? string�� ���ٴ���... wsa�� ���ٸ�?
				std::cout << "[TCP / " << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << "] " << buf << std::endl;

				// ������ �״�� ������
				retval = send(client_sock, buf, retval, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				buf[0] = '\0';
			}
		}
		// Ŭ���̾�Ʈ�� �����ϴ� ���� �ݱ�
		closesocket(client_sock);
		std::cout << "\n[TCP ����] Ŭ���̾�Ʈ ���� : IP�ּ�=" << inet_ntoa(clientaddr.sin_addr) << "��Ʈ��ȣ=" << ntohs(clientaddr.sin_port) << std::endl;
	}
	//��� ����� ���� ��. listen������ ������ listen������ �ݴ´�.
	// ������ �ݴ°� WSACleanup�� ������ ���� �Ǵ�....
	// listen sock : ��ȣǥ
	// client sock : â��
	closesocket(listen_sock);
	// ������ �� �������� ���� �޸𸮿��� ����
	WSACleanup();
	return 0;
}