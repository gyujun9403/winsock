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
	int retval; // 반환값
	SOCKADDR_IN serveraddr; //서버 주소 정의
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	// INADDR_ANY : 
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// listen 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	// listen 소켓을 바인드(포트를 소켓과 연결)
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	// listen 소켓을 listen~~하게 
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 실제 통신할 소켓 생성
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	while (1)
	{
		// listen소켓 듣기
		addrlen = sizeof(clientaddr);	//반복문안에서 이걸 하는 이유는?
		// accept하기 위해 필요한건 리슨할 소켓 디스크립터, 클라이언트 주소를 받아올곳, 클러이언트 주소를 받을곳의 크기
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		// 이후는 listen소켓이 일하지 않고, client소켓이 일한다.
		//printf("\n[TCP 서버] 클라이언트 접속 : IP주소=%s, 포트번호=%s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		// inet_ntoa 문제 : 
		std::cout << "\n[TCP 서버] 클라이언트 접속 : IP주소=" << inet_ntoa(clientaddr.sin_addr) << "포트번호=" << ntohs(clientaddr.sin_port) << std::endl;
		// 클라이언트와 지속적으로 통신하는 반복문
		while (1)
		{
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			// 소켓 통신이 끝날때 -> 아무것도 없는 line입력시.
			else if (retval == 0)
				break;
			else
			{
				// 클라이언트로부터 받은 입력 출력
				buf[retval] = '\0'; // c++식으로 할수 있는지 않을까? string을 쓴다던가... wsa를 쓴다면?
				std::cout << "[TCP / " << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << "] " << buf << std::endl;

				// 받은거 그대로 보내기
				retval = send(client_sock, buf, retval, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				buf[0] = '\0';
			}
		}
		// 클라이언트와 소통하는 소켓 닫기
		closesocket(client_sock);
		std::cout << "\n[TCP 서버] 클라이언트 종료 : IP주소=" << inet_ntoa(clientaddr.sin_addr) << "포트번호=" << ntohs(clientaddr.sin_port) << std::endl;
	}
	//모든 통신이 끝난 곳. listen할일이 없으니 listen소켓을 닫는다.
	// 소켓을 닫는건 WSACleanup과 보통은 같이 되는....
	// listen sock : 번호표
	// client sock : 창구
	closesocket(listen_sock);
	// 소켓이 다 닫혔으니 윈속 메모리에서 내림
	WSACleanup();
	return 0;
}