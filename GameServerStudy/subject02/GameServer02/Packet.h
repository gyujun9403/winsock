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
	// TODO:
	//bool IsCreate;
	//short RoomIndex;
	int RoomIndex;
};

struct PktRoomEnterRes : PktBase
{
	long long UniqueId;
};

struct PktRoomEnterUserInfoNtf
{
	long long uniqueId;
	char idlen;
	char UserID[MAX_USER_ID_SIZE];
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
	// TODO:
	//char UserId[MAX_USER_ID_SIZE] = { 0, };
	long long uniqueId;
	char idlen;
	char UserID[MAX_USER_ID_SIZE];
};

// �� ä�� �޼����� �ִ� ������.
const int MAX_ROOM_CHAT_MSG_SIZE = 256;
// �뿡�� ä���ϴ� request �ż����� ��� ����ü
struct PktRoomChatReq
{
	// TODO:
	short Msglen;
	//wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
	char Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
};

struct PktRoomChatRes : PktBase
{

};

// �������� ������ �����°� ������...
struct PktRoomChatNtf
{
	//TODO:
	//char UserID[MAX_USER_ID_SIZE] = { 0, };
	long long uniqueId;
	short msgLen;
	wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
};
#pragma pack(pop)