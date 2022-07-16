#pragma comment(lib, "ws2_32")
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <ws2tcpip.h>
#include "error_handle.h"

void	init_addr_listen(SOCKADDR_IN &sock)
{
	ZeroMemory(&sock, sizeof(sock));
	InetPton(AF_INET,"127.0.0.1", &sock.sin_addr);
	sock.sin_family = AF_INET;
	sock.sin_port = htons(9000);
}

int	recvn(SOCKET sock, char *buf, int len, int flgs)
{
	int len_recv = 0, retval;

	while (len > 0)
	{
		retval = recv(sock, buf, len, flgs);
		if (retval == INVALID_SOCKET)
		{
			err_display("recv()");
			return (INVALID_SOCKET);
		}
		else if (retval == 0)
			break;
		len -= retval;
		len_recv += retval;
	}
	return (len_recv);
}

int main()
{
	int	retval;
	SOCKET sock_listen;
	SOCKET sock_client;
	SOCKADDR_IN addr_listen;
	SOCKADDR_IN addr_client;
	int	len_socket_client = sizeof(addr_client);
	unsigned int len_data;
	const unsigned int BUFSIZE = 512;
	char data[BUFSIZE];
	std::string filename = "";

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	sock_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_listen == INVALID_SOCKET)
		err_quit("socket()");
	init_addr_listen(addr_listen);
	if (bind(sock_listen, (SOCKADDR*)&addr_listen, sizeof(addr_listen)) == SOCKET_ERROR)
		err_quit("bind()");
	if (listen(sock_listen, SOMAXCONN) == SOCKET_ERROR)
		err_quit("listen()");
	while (1)
	{
		sock_client = accept(sock_listen, (SOCKADDR *)&addr_client, &len_socket_client);
		if (sock_client == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}
		while (1)
		{
			// ***** 파일 이름 받고 엶. *****
			retval = recvn(sock_client, (char*)&len_data, sizeof(len_data), 0);
			if (retval == INVALID_SOCKET)
			{
				err_display("recvn()");
				break;
			}
			else if (retval == 0)
				break;
			std::cout << "name leng : " << len_data << std::endl;
			std::cout << filename << std::endl;
			while (len_data != 0)
			{
				ZeroMemory(data, BUFSIZE);
				if (len_data < BUFSIZE)
					retval = recvn(sock_client, data, len_data, 0);
				else
					retval = recvn(sock_client, data, BUFSIZE, 0);
				filename.append(data, retval);
				len_data -= retval;
			}
			std::fstream fstream(filename, std::fstream::out | std::fstream::trunc);
			if (!fstream.is_open())
			{
				std::cout << "file open fail!" << std::endl;
				return 1;
			}
			// ***** 파일 내용 읽고 쓺. *****
			while (1)
			{
				// len_data이 unsigned char로 전달되면 4byte크기의 char buf처럼 동작하게 되므로
				// 255이상의 숫자를 전달 받을수 있을것임(아마?) -> 성공
				retval = recvn(sock_client, (char*)&len_data, sizeof(len_data), 0);
				if (retval == INVALID_SOCKET)
				{
					err_display("recvn()");
					break;
				}
				else if (retval == 0)
					break;
				std::cout << " 길이 :" << len_data << std::endl;
				while (len_data != 0)
				{
					ZeroMemory(data, BUFSIZE);
					if (len_data < BUFSIZE)
						retval = recvn(sock_client, data, len_data, 0);
					else
						retval = recvn(sock_client, data, BUFSIZE, 0);
					fstream.write(data, retval);
					len_data -= retval;
				}
				fstream.flush();
			}
			fstream.close();
			filename.clear();
			std::cout << filename << " closed" << std::endl;
		}
		closesocket(sock_client);
	}
	closesocket(sock_listen);
	WSACleanup();
	return 0;
}