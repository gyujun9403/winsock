#pragma once

#include "PacketID.h"
#include "ErrorCode.h"

// 아직 참조 없음
#pragma pack(push, 1)
struct PktHeader
{
	short TotalSize;
	short Id;
	unsigned char Reserve;
};

// 이걸 다른 구조체들이 상속받는거 같은데?
// 상속받은 구조체들은 class처럼 ErrorCode나 SetError를 포함한다.
struct PktBase
{
	short ErrorCode = static_cast<short>(ERROR_CODE::NONE);
	void SetError(ERROR_CODE error) { ErrorCode = static_cast<short>(error); }
};

// ID 및 PASS 사이즈
const int MAX_USER_ID_SIZE = 16;
const int MAX_USER_PASSWORD_SIZE = 16;
struct PktLogInReq
{
	// 배열 전부 0 초기화
	char szID[MAX_USER_ID_SIZE] = { 0, };
	char szPW[MAX_USER_PASSWORD_SIZE] = { 0, };
};

struct PktLogInRes : PktBase
{

};

// 방 입장 요청. 타이틀 크기는 왜 지정한거지?
const int MAX_ROOM_TITLE_SIZE = 16;
struct PktRoomEnterReq
{
	// 요청하는 방이 있는지
	//bool IsCreate;
	short RoomIndex;
	//int RoomIndex;
};

struct PktRoomEnterRes : PktBase
{

};


// 유저 정보가 있는것도 있고 없는것도 있는데 뭐지
// 헤더 정보도 없는거 같은데

// 룸나가기 요청...
struct PktRoomLeaveReq { };

struct PktRoomLeaveRes : PktBase
{
};

// 룸 나가는 유저에게 하는 통보(아직은 로비에 있는).	
struct PktRoomLeaveUserInfoNtf
{
	char UserId[MAX_USER_ID_SIZE] = { 0, };
};

// 룸 채팅 메세지의 최대 사이즈.
const int MAX_ROOM_CHAT_MSG_SIZE = 256;
// 룸에서 채팅하는 request 매세지를 담는 구조체
struct PktRoomChatReq
{
	wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
};

struct PktRoomChatRes : PktBase
{

};

// 유저에게 공지를 보내는거 같은데...
struct PktRoomChatNtf
{
	char UserID[MAX_USER_ID_SIZE] = { 0, };
	wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
};
#pragma pack(pop)