#pragma once

#include <unordered_map>
#include <deque>
#include <string>
#include <vector>

#include "ErrorCode.h"

// TODO: #include "User.h"���� ������?? -> ����� ���漱�� �ϴ� ������ Ư�̻��� https://coding-restaurant.tistory.com/504
class User;

class UserManager
{
public:
	UserManager();
	virtual ~UserManager();
	void Init(const int maxUserCount);
	// ���� �߰� �� ����
	ERROR_CODE AddUser(const int sessionIndex, const char* pszID);
	ERROR_CODE RemeveUser(const int sessionIndex);
	std::tuple<ERROR_CODE, User*> GetUser(const int sessionIndex);

private:
	// ���� Ǯ�� ������ �߰�/���� -> ����Ǯ ����
	User* AllocUserObjPoolIndex();
	void ReleaseUserObjPoolIndex(const int index);
	// ���� Ǯ���� ������ ã�� �Լ�.
	User* FindUser(const int sessionIndex);
	User* FindUser(const char* pszID);
private:
	std::vector<User> userObjPool_;
	std::deque<int> userObjPoolIndex_;
	std::unordered_map<int, User*> userSessionDic_;
	std::unordered_map<const char*, User*> userIdDic_;
};

