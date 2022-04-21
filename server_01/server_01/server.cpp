#pragma comment(lib, "ws2_32")	// https://hizstory.tistory.com/45
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

void err_display(char* msg)
{
	LPVOID lpMsgBuf;	// https://docs.microsoft.com/ko-kr/windows/win32/winprog/windows-data-types
	FormatMessage		// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),	// https://docs.microsoft.com/ko-kr/windows/win32/winsock/windows-sockets-error-codes-2
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


int main(int ac, char *av[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	WSACleanup();
	return 0;
}

https://m.blog.naver.com/tkdldjs35/221838726823