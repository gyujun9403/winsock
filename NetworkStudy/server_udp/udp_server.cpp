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
	// <<< UDP ���� ��� >>>
	// ���� �ý������� �������� ��Ŷ�� �����ϸ� ���̴�.
	// �׷��Ƿ� ���� sendto�� �ϰ� �Ǵ� ���� bind�� ���� ��Ʈ ��ȣ�� IP �ּҸ� �˾ƾ��ϰ�
	// ���� ����� ������ bind�� �ʿ��� ���� ��Ŷ�� �ް� �ȴ�.
	// ���⼭�� ���� �ް� �Ǵ� ���̹Ƿ�
	// ���� ���� -> bind -> recvfrom -> sendto�� ������ ���´�.

	// <<< ���� �ʱ�ȭ >>>
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// <<< Create Socket >>>
	// PF_INET : IPv4 protocol family
	// SOCK_DGRAM : UDP �������� ���� ���
	// IPPROTO_UDP : ������ ����� �������� ��� ��� ����(UDP)
	// ���������� IPPROTO_HOPOPTS ��(0)�� �����ϸ� TCP���� UDP���� �ڵ����� ������.
	SOCKET hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// <<< bind �غ� >>>
	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// <<< bind >>>
	bind(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

	// <<< ��Ŷ�� sendto�� ���� �ּҸ� ��� ���� �غ� >>>
	SOCKADDR_IN tDestAddr = {};
	int iDestLength = sizeof(tDestAddr);
	char Packet[PACKET_LENGTH] = {};

	// <<< recvfrom >>>
	// arg 1 : ���ε� �� ����
	// arg 2 : ��Ŷ�� ���� �׸�
	// arg 3 : ��Ŷ�� ���� �׸��� ������
	// arg 4 : flag ������ �پ��� �ɼǰ��� �� �� ������ ���� ����Ʈ�� 0�� ����
	// arg 5 : ��Ŷ�� ������ �ּҰ� ���� ����ü�� ���´�.
	// arg 6 : ��Ŷ�� ������ �ּ� ����ü�� ������
	recvfrom(hSocket, Packet, sizeof(Packet), 0, (SOCKADDR*)&tDestAddr, &iDestLength);

	cout << "I am Server : " << Packet << endl;

	// �ٽ� �����͸� �۽��ϱ� ���� ��Ŷ ����
	memset(Packet, 0, PACKET_LENGTH);

	// ��Ŷ �޽��� ����
	strcpy_s(Packet, "Hello udp client.");

	// <<< sendto >>>
	// strlen(Packet) : �������� �ϴ� ��Ŷ�� ���� ��ŭ ����
	// (SOCKADDR*)&tDestAddr : recvfrom�� ���ؼ� �ްԵ� ������ ���� �ּҷ� ��Ŷ ����
	sendto(hSocket, Packet, strlen(Packet), 0, (SOCKADDR*)&tDestAddr, iDestLength);

	// <<< close >>>
	// ����� ����� ���� �ڿ� ��ȯ
	closesocket(hSocket);

	// ���� ����
	WSACleanup();

	return 0;
}
