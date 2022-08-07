#include "UserManager.h"
#include "User.h"
#include <algorithm>

UserManager::UserManager()
{
}

UserManager::~UserManager()
{
}

// �ϴ� �ִ� ���� ������ ��ŭ�� ������ �̸� �����س���
void UserManager::Init(const int maxUserCount)
{
	for (int i = 0; i < maxUserCount; ++i)
	{
		User user;
		user.Init(static_cast<short>(i));

		this->userObjPool_.push_back(user);
		// �����ε���Ǯ�� ��� ������(�������� ����) �ε��� ���� ����
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
		// ���� ��û Ǯ���� ���� �Ҵ��� ������ �޾ƿ��� �Ҵ��Ѵ�.
		// ������ Ǯ�� ���°�� �ִ����� �ڷᱸ���� �ڵ����� �������°�, �װ� ������������ ������ ����� �ʿ�� ����.
		auto user = this->AllocUserObjPoolIndex();
		if (user == nullptr)
		{
			return ERROR_CODE::USER_MGR_MAX_USER_COUNT;
		}
		// �� �Ҵ��� ������ �޾ƿͼ� ���빰�� ä���
		user->Set(sessionIndex, pszID);
		// �Ҵ��� �������� Dic(�˻��� ����?)�� ������
		// TODO: Dic�� ���� �� ������ �˻��� �ڷᱸ���� ���� ����� ���ؼ�? � ������ ������?
		// {...}�� pair�� �ڵ���ȯ?
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
	auto index = user->GetIndex(); // ������ �������� �ε���
	auto pszID = user->GetId(); // ������ �������� id
	// �������� ����
	this->userSessionDic_.erase(sessionIndex);
	this->userIdDic_.erase(pszID.c_str()); // �̰� id�� �˻��ϴµ�
	// ���� �����Ϳ��� ����
	ReleaseUserObjPoolIndex(index);
	return ERROR_CODE::NONE;
}

std::tuple<ERROR_CODE, User*> UserManager::GetUser(const int sessionIndex)
{
	auto user = FindUser(sessionIndex);

	if (user == nullptr)
	{
		// tuple ���·� �����ڵ� ��ȯ
		return { ERROR_CODE::USER_MGR_INVALID_SESSION_INDEX, nullptr };
	}
	else
	{
		if (user->IsConfirm() == false)
		{
			// tuple ���·� �����ڵ� ��ȯ
			return { ERROR_CODE::USER_MGR_NOT_CONFIRM_USER, nullptr };
		}
		// ���ϴ� �����͸� ���������� + ������ ���·� tuple�� ��ȯ
		return { ERROR_CODE::NONE, user };
	}
}

// ���� �ε����� ��Ƴ��� Ǯ���� ���� ���� �����ͼ�, �� ������ Ǯ���� ������...?
// Ǯ�� �׿��ִ� ���� ���� ��û�� �����ͼ� �̸� �Ҵ��ϴ� ����?
User* UserManager::AllocUserObjPoolIndex()
{
	if (this->userObjPoolIndex_.empty())
	{
		return nullptr;
	}
	// �����ε���Ǯ���� ��� �����Ѱ� �ϳ� ������
	int index = this->userObjPoolIndex_.front();
	this->userObjPoolIndex_.pop_front();
	// ������ ��� ������ �ε����� ������ ��ȯ
	return &this->userObjPool_[index];
}

void UserManager::ReleaseUserObjPoolIndex(const int index)
{
	// userObjPoolIndex_(�����ε���Ǯ)���� ����� �� �ִ� ������ �ε��� ���� ����ִ�.
	// ��, ������ ����Ǯ���� release�ϸ�, �� ������ ��밡�������Ƿ� �����ε���Ǯ�� �ε����� �ִ´�.
	this->userObjPoolIndex_.push_back(index);
	this->userObjPool_.at(index).Clear();
}

// �������� �˻�
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

// ID�� �˻�
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
