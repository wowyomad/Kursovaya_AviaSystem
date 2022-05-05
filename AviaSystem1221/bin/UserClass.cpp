#include "UserClass.h"
#include "UserConsoleInput.h"

#include <fstream>
#include <vector>
#include <conio.h>
#include <iostream>
#include <cstdio>

extern const char PATH_USERBASE[] = "Files\\UserBase.dat";
extern const int MIN_PASSWORD = 8;
extern const int MAX_PASSWORD = 32;
extern const int MIN_LOGIN = 3;
extern const int MAX_LOGIN = 32;
extern const int STRING_RESERVE = 64;
extern const int ENTER_KEY = 13;

std::fstream User::UserFile;
std::ifstream User::UserFileIn;
std::ofstream User::UserFileOut;
std::vector<User> User::userVector;

User::User()
{
	login.reserve(MAX_LOGIN);
	access = 0;
}

User::User(const User& source)
{
	login = source.login;
	hash = source.hash;
	salt = source.salt;
	access = source.access;
}

User::User(User&& source) noexcept
{
	login = std::move(source.login);
	hash = std::move(source.hash);
	salt = std::move(source.salt);
	access = source.access;
}

BaseClass::BaseClass() : login("default") {};

BaseClass::BaseClass(std::string& login) : login(login) {};


User User::operator=(const User& source)
{
	login = source.login;
	hash = source.hash;
	salt = source.salt;
	access = source.access;
	return *this;
}

User User::operator = (User&& source) noexcept
{
	login = std::move(source.login);
	hash = std::move(source.hash);
	salt = std::move(source.salt);
	access = source.access;
	return *this;
}

User User::InputUser(const int access)
{
	InputLogin(login, MIN_LOGIN, MAX_LOGIN);
	std::string password;
	InputPassword(password, MIN_PASSWORD, MAX_PASSWORD);
	salt = RNG::salt();
	hash = RNG::hash(password, salt);
	this->access = access;
	return *this;
}

int User::Login()
{
	std::string password;
	std::string login;
	password.reserve(MAX_PASSWORD);
	login.reserve(MAX_LOGIN);
	InputLogin(login, MIN_LOGIN, MAX_LOGIN);
	InputPassword(password, MIN_PASSWORD, MAX_PASSWORD);
	int i = loginExist(login);
	if (i > -1)
	{
		if (RNG::hash(password, userVector[i].salt) == userVector[i].hash)
		{
			if (userVector[i].access != AccessLevel::NoAcess)
			{
				*this = userVector[i];
				return userVector[i].access;
			}
			else
			{
				return userVector[i].access;
			}
		}
	}
	return -1;
}

int User::loginExist(std::string& newLogin)
{
	for (int i = 0; i < userVector.size(); i++)
	{
		if (userVector[i].login == newLogin)
			return i;
	}
	return -1;
}

bool User::NewUser(const int access)
{
	User temp;
	temp.InputUser(access);
	if (loginExist(temp.login) > -1)
		return false;
	userVector.emplace_back(std::move(temp));
	return true;
}


bool User::checkForAdmin()
{
	for (auto& it : userVector)
	{
		if (it.access == AccessLevel::Admin)
			return true;
	}
	return false;
}

void BaseClass::ShowFlights(const int mode)
{

}

void BaseClass::SearchFlights()
{

}

void BaseClass::SortFligths()
{
	
}



std::fstream operator<<(std::fstream& fs, User& user)
{
	return std::fstream();
}
