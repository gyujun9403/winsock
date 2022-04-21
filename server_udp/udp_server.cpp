#include <WinSock2.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT             7891
#define PACKET_LENGTH    1024

void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCUTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCUTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main()
{
	// <<< UDP 소켓 통신 >>>
	// 목적 시스템으로 목적지로 패킷을 전송하면 끝이다.
	// 그러므로 먼저 sendto를 하게 되는 쪽이 bind한 쪽의 포트 번호와 IP 주소를 알아야하고
	// 서로 통신할 소켓을 bind한 쪽에서 먼저 패킷을 받게 된다.
	// 여기서는 먼저 받게 되는 쪽이므로
	// 소켓 생성 -> bind -> recvfrom -> sendto의 순서를 갖는다.

	// <<< 윈속 초기화 >>>
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// <<< Create Socket >>>
	// PF_INET : IPv4 protocol family
	// SOCK_DGRAM : UDP 프로토콜 전송 방식
	// IPPROTO_UDP : 실제로 사용할 프로토콜 통신 방식 결정(UDP)
	// 마찬가지로 IPPROTO_HOPOPTS 값(0)을 설정하면 TCP인지 UDP인지 자동으로 잡힌다.
	SOCKET hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// <<< bind 준비 >>>
	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// <<< bind >>>
	bind(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

	// <<< 패킷을 sendto한 쪽의 주소를 얻기 위한 준비 >>>
	SOCKADDR_IN tDestAddr = {};
	int iDestLength = sizeof(tDestAddr);
	char Packet[PACKET_LENGTH] = {};

	// <<< recvfrom >>>
	// arg 1 : 바인딩 한 소켓
	// arg 2 : 패킷을 받을 그릇
	// arg 3 : 패킷을 받을 그릇의 사이즈
	// arg 4 : flag 값으로 다양한 옵션값을 줄 수 있지만 보통 디폴트로 0을 전달
	// arg 5 : 패킷을 전송한 주소가 여기 구조체로 들어온다.
	// arg 6 : 패킷을 전송한 주소 구조체의 사이즈
	recvfrom(hSocket, Packet, sizeof(Packet), 0, (SOCKADDR*)&tDestAddr, &iDestLength);

	cout << "I am Server : " << Packet << endl;

	// 다시 데이터를 송신하기 전에 패킷 비우기
	memset(Packet, 0, PACKET_LENGTH);

	// 패킷 메시지 설정
	strcpy_s(Packet, "Hello udp client.");

	// <<< sendto >>>
	// strlen(Packet) : 보내고자 하는 패킷의 길이 만큼 전송
	// (SOCKADDR*)&tDestAddr : recvfrom을 통해서 받게된 전송한 쪽의 주소로 패킷 전송
	sendto(hSocket, Packet, strlen(Packet), 0, (SOCKADDR*)&tDestAddr, iDestLength);

	// <<< close >>>
	// 종료시 사용한 소켓 자원 반환
	closesocket(hSocket);

	// 윈속 종료
	WSACleanup();

	return 0;
}
