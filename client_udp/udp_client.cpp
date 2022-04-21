#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT             7891
#define PACKET_LENGTH    1024
#define LOCAL_IP    "127.0.0.1"

int main()
{
	// <<< 윈속 초기화 >>>
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// <<< Create Socket >>>
	SOCKET hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_TCP);

	// <<< sendto 준비 >>>
	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = inet_addr(LOCAL_IP);

	char Packet[PACKET_LENGTH] = {};
	strcpy_s(Packet, "Send from Client");

	// <<< sendto >>>
	sendto(hSocket, Packet, strlen(Packet), 0, (SOCKADDR*)&tAddr, sizeof(tAddr));

	// <<< recvfrom 준비 >>>
	// 여기서도 서로 송수신을 위해 응답 받을 Dest Address 준비.
	SOCKADDR_IN tDestAddr = {};
	int iDestSize = sizeof(tDestAddr);

	// <<< recvfrom >>>
	// (SOCKADDR*)&tDestAddr : 패킷을 전송한 주소가 여기로 들어온다.
	recvfrom(hSocket, Packet, PACKET_LENGTH, 0, (SOCKADDR*)&tDestAddr, &iDestSize);

	// 패킷 메시지 출력
	cout << "I am Client : " << Packet << endl;

	// <<< close >>>
	// 종료시 사용한 소켓 자원 반환
	closesocket(hSocket);

	// 윈속 종료
	WSACleanup();

	return 0;
}