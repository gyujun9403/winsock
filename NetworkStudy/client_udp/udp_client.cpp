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
	// <<< ���� �ʱ�ȭ >>>
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// <<< Create Socket >>>
	SOCKET hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_TCP);

	// <<< sendto �غ� >>>
	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = inet_addr(LOCAL_IP);

	char Packet[PACKET_LENGTH] = {};
	strcpy_s(Packet, "Send from Client");

	// <<< sendto >>>
	sendto(hSocket, Packet, strlen(Packet), 0, (SOCKADDR*)&tAddr, sizeof(tAddr));

	// <<< recvfrom �غ� >>>
	// ���⼭�� ���� �ۼ����� ���� ���� ���� Dest Address �غ�.
	SOCKADDR_IN tDestAddr = {};
	int iDestSize = sizeof(tDestAddr);

	// <<< recvfrom >>>
	// (SOCKADDR*)&tDestAddr : ��Ŷ�� ������ �ּҰ� ����� ���´�.
	recvfrom(hSocket, Packet, PACKET_LENGTH, 0, (SOCKADDR*)&tDestAddr, &iDestSize);

	// ��Ŷ �޽��� ���
	cout << "I am Client : " << Packet << endl;

	// <<< close >>>
	// ����� ����� ���� �ڿ� ��ȯ
	closesocket(hSocket);

	// ���� ����
	WSACleanup();

	return 0;
}