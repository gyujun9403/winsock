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

const int MAX_ROOM_TITLE_SIZE = 16;
struct PktRoomEnterReq
{
	int RoomIndex;
};

struct PktRoomEnterRes : PktBase
{
	int64_t UniqueId;
};

struct PktRoomEnterUserInfoNtf
{
	int64_t uniqueId;
	char idlen;
	char UserID[MAX_USER_ID_SIZE];
};

struct PktRoomUserListNtf
{
	char UserCount;
	char UserID1[MAX_USER_ID_SIZE];
	char UserID2[MAX_USER_ID_SIZE];
	char UserID3[MAX_USER_ID_SIZE];
	char UserID4[MAX_USER_ID_SIZE];
};

struct PktRoomLeaveReq { };

struct PktRoomLeaveRes : PktBase
{
};

struct PktRoomLeaveUserInfoNtf
{
	int64_t uniqueId;
	char idlen;
	char UserID[MAX_USER_ID_SIZE];
};

const int MAX_ROOM_CHAT_MSG_SIZE = 256;
struct PktRoomChatReq
{
	short Msglen;
	char Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
};

struct PktRoomChatRes : PktBase
{

};

struct PktRoomChatNtf
{
	int64_t uniqueId;
	short msgLen;
	wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
};

// ���� ��Ŷ��
struct PktPlaceStoneReq
{
	int x;
	int y;
};

struct PktPlaceStoneRes : PktBase
{

};

struct PktPlaceStoneNtf
{
	int x;
	int y;
	bool color;
};

struct PktGameResultNtf
{
	bool result;
};
#pragma pack(pop)