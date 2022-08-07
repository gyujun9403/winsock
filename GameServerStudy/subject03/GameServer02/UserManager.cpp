#include "UserManager.h"
#include "User.h"
#include <algorithm>

UserManager::UserManager()
{
}

UserManager::~UserManager()
{
}

// 일단 최대 생성 가능한 만큼의 유저를 미리 생성해놓음
void UserManager::Init(const int maxUserCount)
{
	for (int i = 0; i < maxUserCount; ++i)
	{
		User user;
		user.Init(static_cast<short>(i));

		this->userObjPool_.push_back(user);
		// 유저인덱스풀은 사용 가능한(점유되지 않은) 인덱스 들의 집합
		this->userObjPoolIndex_.push_back(i);
	}
}

ERROR_CODE UserManager::AddUser(const int sessionIndex, const char* pszID)
{
	if (FindUser(pszID) != nullptr) {
		return ERROR_CODE::USER_MGR_ID_DUPLICATION;
	}
	else
	{
		// 유저 요청 풀에서 새로 할당할 유저를 받아오고 할당한다.
		// 유저가 풀의 몇번째에 있는지는 자료구로조 자동으로 정해지는거, 그걸 구현과정에서 순서를 고려할 필요는 없다.
		auto user = this->AllocUserObjPoolIndex();
		if (user == nullptr)
		{
			return ERROR_CODE::USER_MGR_MAX_USER_COUNT;
		}
		// 그 할당한 유저를 받아와서 내용물을 채운다
		user->Set(sessionIndex, pszID);
		// 할당한 유저들을 Dic(검색용 사전?)에 저장함
		// TODO: Dic을 따로 둔 이유는 검색용 자료구조를 따로 만들기 위해서? 어떤 이점이 있을까?
		// {...}는 pair로 자동변환?
		this->userSessionDic_.insert({ sessionIndex, user });
		this->userIdDic_.insert({ pszID, user });
		return ERROR_CODE::NONE;
	}
}

ERROR_CODE UserManager::RemeveUser(const int sessionIndex)
{
	auto user = FindUser(sessionIndex);

	if (user == nullptr)
	{
		return ERROR_CODE::USER_MGR_REMOVE_INVALID_SESSION;
	}
	auto index = user->GetIndex(); // 삭제할 데이터의 인덱스
	auto pszID = user->GetId(); // 삭제할 데이터의 id
	// 사전에서 삭제
	this->userSessionDic_.erase(sessionIndex);
	this->userIdDic_.erase(pszID.c_str()); // 이건 id로 검색하는듯
	// 실제 데이터에서 삭제
	ReleaseUserObjPoolIndex(index);
	return ERROR_CODE::NONE;
}

std::tuple<ERROR_CODE, User*> UserManager::GetUser(const int sessionIndex)
{
	auto user = FindUser(sessionIndex);

	if (user == nullptr)
	{
		// tuple 형태로 에러코드 반환
		return { ERROR_CODE::USER_MGR_INVALID_SESSION_INDEX, nullptr };
	}
	else
	{
		if (user->IsConfirm() == false)
		{
			// tuple 형태로 에러코드 반환
			return { ERROR_CODE::USER_MGR_NOT_CONFIRM_USER, nullptr };
		}
		// 원하는 데이터를 에러가없음 + 데이터 형태로 tuple로 반환
		return { ERROR_CODE::NONE, user };
	}
}

// 유저 인덱스를 모아놓은 풀에서 앞의 것을 가져와서, 그 유저를 풀에서 가져옴...?
// 풀에 쌓여있던 유저 생성 요청을 가져와서 이를 할당하는 역할?
User* UserManager::AllocUserObjPoolIndex()
{
	if (this->userObjPoolIndex_.empty())
	{
		return nullptr;
	}
	// 유저인데스풀에서 사용 가능한걸 하나 가져옴
	int index = this->userObjPoolIndex_.front();
	this->userObjPoolIndex_.pop_front();
	// 가져온 사용 가능한 인덱스의 유저를 반환
	return &this->userObjPool_[index];
}

void UserManager::ReleaseUserObjPoolIndex(const int index)
{
	// userObjPoolIndex_(유저인덱스풀)에는 사용할 수 있는 유저의 인덱스 들이 들어있다.
	// 즉, 유저를 유저풀에서 release하면, 그 유저는 사용가능해지므로 유저인데스풀에 인덱스를 넣는다.
	this->userObjPoolIndex_.push_back(index);
	this->userObjPool_.at(index).Clear();
}

// 세션으로 검색
User* UserManager::FindUser(const int sessionIndex)
{
	auto findIter = this->userSessionDic_.find(sessionIndex);

	if (findIter == this->userSessionDic_.end())
	{
		return nullptr;
	}
	else
	{
		return static_cast<User*>(findIter->second);
	}
}

// ID로 검색
User* UserManager::FindUser(const char* pszID)
{
	auto findIter = this->userIdDic_.find(pszID);

	if (findIter == this->userIdDic_.end())
	{
		return nullptr;
	}
	else
	{
		return static_cast<User*>(findIter->second);
	}
}
