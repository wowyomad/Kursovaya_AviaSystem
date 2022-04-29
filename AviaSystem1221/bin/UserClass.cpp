#include "UserClass.h"
#include "FileHandle.h"

#include <fstream>
#include <vector>

#define PATH_USERBASE "Files/UserBase.bin"
#define VECTOR_BUFF 1024
#define STRING_BUFF 64

std::fstream User::file;
std::vector<User> User::userArray;

User& User::ReadUser()
{
	login = FileHandle::ReadString(file);
	hash = FileHandle::ReadString(file);
	salt = FileHandle::ReadString(file);
	access = FileHandle::ReadVar<int>(file);
	return *this;
}

User::User(const User& user)
{
	login = user.login;
	hash = user.hash;
	salt = user.salt;
	access = user.access;
}

User::User(User&& user)
{
	login = std::move(user.login);
	hash = std::move(user.hash);
	salt = std::move(user.salt);
	access = std::move(access);
}

bool User::InitUserBase()
{
	file.open(PATH_USERBASE, std::ios::in | std::ios::out | std::ios::binary);
	if (!file.is_open()) return false;
	if (FileHandle::file_is_empty(file)) return false;
	userArray.reserve(VECTOR_BUFF);
	do
	{
		User temp;
		userArray.emplace_back(std::move(temp.ReadUser()));
	} while (!file.eof());

}
