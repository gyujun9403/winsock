﻿#pragma once


// 201 부터 시작한다.
enum class ERROR_CODE : short
{
	NONE = 0,

	UNASSIGNED_ERROR = 201,

	MAIN_INIT_NETWORK_INIT_FAIL = 206,

	USER_MGR_ID_DUPLICATION = 211,
	USER_MGR_MAX_USER_COUNT = 212,
	USER_MGR_INVALID_SESSION_INDEX = 213,
	USER_MGR_NOT_CONFIRM_USER = 214,
	USER_MGR_REMOVE_INVALID_SESSION = 221,

	//homework
	ROOM_MGR_INVALID_INDEX = 230,
	ROOM_FULL = 231,
	ROOM_ALREADY_ENTERED_ROOM = 232,
	ROOM_NOT_LOGIN_USER = 233,
	ROOM_NO_ROOM_TO_LEAVE = 234,
	ROOM_NO_ROOM_TO_CHAT = 235,
	OMOK_PLACE_OCCUFIED = 236,
	OMOK_CANT_READY = 237,
	OMOK_GAME_NOT_STARTED = 238,
	OMOK_NOT_VALIED_PLACE = 239,
};
