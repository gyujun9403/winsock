// 비주얼 스튜디오 상위 버전에서 error C4996 : inet_addr의 보안 오류가 발생하면
// 아래와 같은 매크로를 통해 해결할 수 있다. 
// 또는 <WS2tcpip.h> 헤더 추가 후 inet_addr 대신 inet_pton API를 사용한다.
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT             7890
#define PACKET_LENGTH    1024

// 서버쪽에서 INADDR_ANY로 잡아준 컴퓨터 IP에
// 접속하기 위해 "127.0.0.1"의 IP를 지정한다.
// (같은 컴퓨터의 IP 주소)
#define LOCAL_IP    "127.0.0.1"

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// <<< Create Socket >>>
	// 서버에 접속 요청을 위한 소켓을 생성한다.
	// TCP 전용 소켓을 생성했으므로 똑같이 맞춰준다.
	// 마지막 세 번째 인자는 IPPROTO_HOPOPTS(0)으로 전달해도 된다.
	// (서버코드 주석 참고)
	SOCKET hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = inet_addr(LOCAL_IP);

	// <<< Connect 요청 >>>
	// 운영체제를 통해 포트 번호와 IP 주소가 바인딩 된 서버 소켓에 접속을 요청한다.
	// 이 함수도 블로킹 방식으로 처리 된다.
	// 서버 소켓이 클라이언트의 연결 요청에 대한 성공, 거절, 또는 시관 초과 등에 대한
	// 결정이 되기 전까지 이 함수에서 빠져나올 수 없다.
	// (이 함수가 실행되면 곧 바로 리턴되지 않을 수 있다)
	connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

	// <<< recv >>>
	// 연결 요청이 성공되었을 때 서버로 부터 연결이 완료 되었다는 확인용 패킷을 받는다.
	// recv API는 패킷을 받을 때, send API는 패킷을 전송할 때 쓰는 함수이다.
	// send와 recv 모두 블로킹 방식으로 처리 된다.
	// 성공, 실패, 종료가 결정되기 전까지 함수에서 빠져나올 수 없다는 뜻. (언제끝날지 모름)

	// (추가 내용)
	// 보통 recv는 send와 달리 별도의 스레드에서 처리하는 방식을 활용한다.
	// send는 데이터를 보내는 주체가 자기 자신이기 때문에 얼마만큼의 패킷을 전송할 지
	// 언제 전송해야 할 지 알 수 있지만, recv는 대상이 어떤 데이터를 언제 수신할 지에 대한
	// 특정한 시점을 알 수 없다. 그리고 recv API는 실행되면 언제 끝날지 모르는 블로킹 상태로
	// 빠지게 된다. 이를 해결하기 위해 데이터 수신을 위한 recv API는 별도의 스레드에서 해결한다. 
	// 소켓과 연결이 완료되면 새로운 스레드를 생성하고 그곳에서 recv를 통해 
	// 데이터가 수신 되길 기다리는 방식으로 처리할 수 있다.
	char Packet[PACKET_LENGTH] = {};
	recv(hSocket, Packet, PACKET_LENGTH, 0);

	// 서버 소켓으로 부터 받은 패킷 메시지를 출력한다.
	cout << "Server Message : " << Packet << endl;

	// 서버 소켓에 패킷을 전송할 메시지를 입력
	strcpy_s(Packet, "접속 완료 !!");

	// <<< send >>>
	// 서버 소켓에 패킷 "접속 완료 !!" 메시지를 전송한다.
	send(hSocket, Packet, strlen(Packet), 0);

	// 종료시 사용이 끝난 소켓 리소스를 반환한다.
	closesocket(hSocket);

	// 윈속 초기화 해제
	WSACleanup();

	return 0;
}
