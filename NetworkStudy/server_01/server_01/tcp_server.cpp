// 윈속 헤더 추가
#include <WinSock2.h>
#include <iostream>

using namespace std;

// 윈속의 기능을 쓰기 위한 라이브러리 추가
#pragma comment(lib, "ws2_32.lib")

#define PORT             7890
#define PACKET_LENGTH    1024

int main()
{
    // 윈속 초기화를 위한 구조체 선언
    WSADATA wsaData;

    /*
      WSAStartup(윈속 초기화) ~ WSACleanup(윈속 종료) 사이에
      윈속 코드를 작성한다.
    */

    // <<< 윈속 초기화 >>>
    // arg 1 : WORD *wVersionRequested
    // 프로그램이 요구하는 winsock 버전 중 최상위 버전.
    // MAKEWORD 매크로를 통해 2.2버전을 쓰겠다고 설정.
    // arg 2 : LPWSADATA *lpWSAData
    // WSADATA 구조체 포인터가 들어간다.
    // 함수가 반환되면 윈속을 위한 세부 정보가 구조체에 포함된다.
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // <<< Create Socket >>>
    // PF_INET : Protocol Family를 설정한다(IPv4).
    // SOCK_STREAM : 새로 생성할 소켓의 타입을 정한다(TCP).
    // IPPROTO_TCP : 실제로 사용할 프로토콜을 최종적으로 결정한다(TCP).
    // 세 번째 인자로 IPPROTO_HOPOPTS을 전달해도 된다.
    // 이 값은 디폴트 0으로 잡혀있다. 이 값을 쓰면 자동으로
    // TCP 전용 소켓인지, UDP 전용 소켓인지 자동으로 결정해준다. 그래서 0으로 전달해도 된다.
    SOCKET hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 이제 위에서 생성한 소켓의 포트 번호와 IP 주소, Address Family를 정해야한다.
    // 이를 위해 SOCKADDR_IN 구조체를 생성한다. (bind를 하기 위한 준비)
    SOCKADDR_IN tListenAddr = {};

    // AF_INET : IPv4 Address Family를 쓰겠다. 
    tListenAddr.sin_family = AF_INET;

    // htons(PORT) : 위에서 매크로로 정의한 포트 번호를 설정한다.
    // htons : Host To Network Short의 약자로
    // Network로 short형 데이터를 보낼때 바이트 오더를 해주는 함수이다.
    // 네트워크 주소는 CPU 바이트 순서대로 저장되는데, 바이트 오더 방식은 2가지가 있다.

    // [빅 엔디안]
    // SPARC, RISC CPU 계열에서 사용하는 바이트 오더 방식.
    // 빅 엔디안 방식의 바이트 오더는 큰 단위부터 메모리에 적는다.
    // (ex) 0x12345678을 적는다고 하면 큰 단위가 1234 여기니까
    // 12, 34, 56, 78 이렇게 메모리를 적는다.
    // 이처럼 최고 단위가 앞에 있으므로 부호에 대한 비트 확인이 빨라서
    // 네트워크 통신에서 사용하는 방식이라고 한다.

    // [리틀 엔디안]
    // 인텔, 암드 계열에서 사용하는 바이트 오더 방식이다.
    // (대부분 컴퓨터는 인텔, 암드 계열을 사용하므로... 
    // 네트워크 통신을 위해 htons로 빅 엔디안 방식의 바이트 오더로 전환해야한다)
    // 이 방식은 작은 단위부터 메모리에 적는 방식이다.
    // (ex) 0x12345678을 적는다고 하면 5678 여기가 작은 단위니까
    // 78, 56, 34, 12 이런식으로 CPU에 메모리를 적는다.
    // 연산을 할 때 가장 뒷 자리부터 계산하므로 연산이 빠르다는 장점이 있다. 
    tListenAddr.sin_port = htons(PORT);

    // <<< bind 준비 >>>
    // htonl(INADDR_ANY) : Host To Network Long
    // long 타입 데이터의 바이트 오더를 빅 엔디안 바이트 오더 방식으로 바꿔준다.
    // INADDR_ANY : 컴퓨터에서 존재하는 랜카드 중에서 사용 가능한 IP 주소를 찾아서
    // 자동으로 대입해준다. 서버 입장에서 현재 동작되는 컴퓨터가 서버의 IP 주소이므로
    // INADDR_ANY로 설정해준다.
    tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // <<< bind >>>
    // 위에서 지정한 포트 번호, IP 주소를 생성한 Listen Socket에 바인딩한다.
    // 이렇게 바인딩하는 이유는 운영체제가 포트 번호를 관리하기 위함이다.
    // 만약, 서버 소켓의 포트 번호가 다른 소켓에서 중복되었다고 한다면
    // 운영체제는 이를 감지하여 에러를 리턴하게 된다.
    bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));

    // <<< Listen >>>
    // 귀를 기울인다. 즉, 위에서 포트 번호와 IP 주소를 바인딩한 서버 소켓을 통해서
    // 클라이언트의 연결 요청이 수신될 때 까지 기다린다.
    // 두 번째 인자는 클라이언트 연결 대기열의 크기이다. 여러 클라이언트가 한 번에 들어올 수 있다.
    // 보통 5로 설정한다고 하지만, 윈속2부터 SOMAXCONN이라는 상수값을 지정할 수 있다.
    // 이를 통해 소켓 지정자가 알아서 연결 대기열의 크기를 결정할 수 있게 되었다고 한다.
    // 그리고 listen api가 성공적으로 리턴했다고 해서 무조건 연결되어 있는 상태가 아니다.
    // 클라이언트의 최종적인 연결 요청을 받아들이는 함수는 아래에서 사용될 accept 함수이다.
    listen(hListen, SOMAXCONN);

    // <<< accept 준비 >>>
    // 클라이언트의 connect를 accept 하기 전에
    // 미리 새롭게 생성될 소켓의 구조체 정보를 생성해둔다.
    // 클라이언트와 연결이 성공되면 이 구조체의 정보를 이용해서
    // 클라이언트의 주소 정보를 알아낼 수 있다.
    SOCKADDR_IN tClntAddr = {};
    int iSize = sizeof(tClntAddr);

    // <<< accept >>>
    // accept는 연결지향형 소켓에서 사용하는 함수이다 !!
    // (비연결지향형 UDP에서는 사용하지 않는다)
    // 그리고 accept는 블로킹 방식으로 처리된다.
    // 블로킹 방식 : 요청이 올 때 까지 대기한다.
    // (요청이 오기 전 까지 이 함수에서 빠져나올 수 없다)
    // 클라이언트와 연결이 되면 서로 통신할 새로운 소켓을 반환한다.
    // 이 소켓을 바탕으로 서로 통신할 수 있게 되는 것이다.
    // 에러가 발생하면 -1(SOCKET_ERROR)을 리턴하고
    // 성공하면 0보다 큰 파일지정번호가 리턴된다.
    SOCKET hSocket = accept(hListen, (SOCKADDR*)&tClntAddr, &iSize);

    if (hSocket == SOCKET_ERROR)
    {
        // 에러가 발생했다면 사용한 소켓을 닫아주고 바로 종료한다.
        // 사용한 소켓의 리소스를 반환한다.
        closesocket(hListen);

        // 윈속 종료
        WSACleanup();

        return 0;
    }

    // 클라이언트에게 접속되었다는 메시지를 보낸다.
    // (캐릭터 배열로 구성)
    char Packet[PACKET_LENGTH] = {};
    strcpy_s(Packet, "Hello Client.");

    // <<< send >>>
    // 지정된 소켓으로 패킷을 전송한다.
    // send 또한 블로킹 방식으로 처리 되기 때문에
    // 실행 결과(성공, 실패, 종료)가 결정되기 전 까지 값을 리턴하지 않는다.
    // 마지막 인자는 0으로 설정하면 일반적인 패킷 데이터를 전송할 수 있다.
    send(hSocket, Packet, strlen(Packet), 0);

    // 클라이언트로 받을 패킷 메시지를 위해 깨끗하게 비워둔다.
    memset(Packet, 0, sizeof(Packet));

    // <<< recv >>>
    // 서버에서 응답받은 클라이언트로부터 패킷을 받는다.
    // 이 함수도 블로킹 방식으로 처리된다.
    recv(hSocket, Packet, PACKET_LENGTH, 0);

    cout << "Client Message : " << Packet << endl;

    // 종료시 사용이 끝난 소켓 리소스를 반환한다.
    closesocket(hSocket);
    closesocket(hListen);

    // 윈속을 종료한다.
    WSACleanup();

    return 0;
}