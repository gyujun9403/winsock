#pragma once

#include <unordered_map>
#include <deque>
#include <string>
#include <vector>

#include "ErrorCode.h"

// TODO: #include "User.h"와의 차이점?? -> 헤더에 전방선언 하는 이유와 특이사항 https://coding-restaurant.tistory.com/504
class User;

class UserManager
{
public:
	UserManager();
	virtual ~UserManager();
	void Init(const int maxUserCount);
	// 유저 추가 및 삭제
	ERROR_CODE AddUser(const int sessionIndex, const char* pszID);
	ERROR_CODE RemeveUser(const int sessionIndex);
	std::tuple<ERROR_CODE, User*> GetUser(const int sessionIndex);

private:
	// 유저 풀에 유저를 추가/삭제 -> 유저풀 관리
	User* AllocUserObjPoolIndex();
	void ReleaseUserObjPoolIndex(const int index);
	// 유저 풀에서 유저를 찾는 함수.
	User* FindUser(const int sessionIndex);
	User* FindUser(const char* pszID);
private:
	std::vector<User> userObjPool_;
	std::deque<int> userObjPoolIndex_;
	std::unordered_map<int, User*> userSessionDic_;
	std::unordered_map<const char*, User*> userIdDic_;
};

