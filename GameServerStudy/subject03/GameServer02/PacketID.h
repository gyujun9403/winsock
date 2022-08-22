#pragma once

// enum class. �ڷ��� �⺻��(������)�� short
// NServerNetLib::PACKET_ID���� ���� �����ͼ� �� ��ŶID�� ���� ������ switch
// REQ�ǹ�?->������Ʈ. ��Ŷ�� ��û�� �ǹ�
// RES�ǹ�?->��������. ��Ŷ�� ������ �Ǥ�
// NTF -> notification ����
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