#pragma once

#include "PacketID.h"
#include "ErrorCode.h"

// ���� ���� ����
#pragma pack(push, 1)
struct PktHeader
{
	short TotalSize;
	short Id;
	unsigned char Reserve;
};

// �̰� �ٸ� ����ü���� ��ӹ޴°� ������?
// ��ӹ��� ����ü���� classó�� ErrorCode�� SetError�� �����Ѵ�.
struct PktBase
{
	short ErrorCode = static_cast<short>(ERROR_CODE::NONE);
	void SetError(ERROR_CODE error) { ErrorCode = static_cast<short>(error); }
};

// ID �� PASS ������
const int MAX_USER_ID_SIZE = 16;
const int MAX_USER_PASSWORD_SIZE = 16;
struct PktLogInReq
{
	// �迭 ���� 0 �ʱ�ȭ
	char szID[MAX_USER_ID_SIZE] = { 0, };
	char szPW[MAX_USER_PASSWORD_SIZE] = { 0, };
};

struct PktLogInRes : PktBase
{

};

// �� ���� ��û. Ÿ��Ʋ ũ��� �� �����Ѱ���?
const int MAX_ROOM_TITLE_SIZE = 16;
struct PktRoomEnterReq
{
	// ��û�ϴ� ���� �ִ���
	//bool IsCreate;
	short RoomIndex;
	//int RoomIndex;
};

struct PktRoomEnterRes : PktBase
{

};


// ���� ������ �ִ°͵� �ְ� ���°͵� �ִµ� ����
// ��� ������ ���°� ������

// �볪���� ��û...
struct PktRoomLeaveReq { };

struct PktRoomLeaveRes : PktBase
{
};

// �� ������ �������� �ϴ� �뺸(������ �κ� �ִ�).	
struct PktRoomLeaveUserInfoNtf
{
	char UserId[MAX_USER_ID_SIZE] = { 0, };
};

// �� ä�� �޼����� �ִ� ������.
const int MAX_ROOM_CHAT_MSG_SIZE = 256;
// �뿡�� ä���ϴ� request �ż����� ��� ����ü
struct PktRoomChatReq
{
	wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
};

struct PktRoomChatRes : PktBase
{

};

// �������� ������ �����°� ������...
struct PktRoomChatNtf
{
	char UserID[MAX_USER_ID_SIZE] = { 0, };
	wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
};
#pragma pack(pop)