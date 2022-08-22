#pragma once

// enum class. 자료의 기본형(사이즈)는 short
// NServerNetLib::PACKET_ID에서 값을 가져와서 이 패킷ID를 통해 동작을 switch
// REQ의미?->리퀘스트. 패킷이 요청의 의미
// RES의미?->리스폰스. 패킷이 응답의 의ㅁ
// NTF -> notification 공고
enum class PACKET_ID : short
{
	LOGIN_IN_REQ = 21,
	LOGIN_IN_RES = 22,

	ROOM_ENTER_REQ = 61,
	ROOM_ENTER_RES = 62,
	ROOM_ENTER_NEW_USER_ = 63,

	ROOM_LEAVE_REQ = 66,
	ROOM_LEAVE_RES = 67,
	ROOM_LEAVE_USER_NTF = 68,

	ROOM_CHAT_REQ = 76,
	ROOM_CHAT_RES = 77,
	ROOM_CHAT_NTF = 78,
	
	OMOK_PLACE_STONE_REQ = 81,
	OMOK_PLACE_STONE_RES = 82,
	OMOK_PLACE_STONE_NTF = 83,
	OMOK_READY_REQ = 84,
	OMOK_READY_RES = 85,
	OMOK_READY_NTF = 86,
	OMOK_CLEAR_BOARD_NTF = 99,
	OMOK_RESULT_NTF = 100,

	// OMOK_SHIFTING_NTF

	MAX = 256
};