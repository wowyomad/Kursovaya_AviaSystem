#include "UserClass.h"
#include "UserConsoleInput.h"
#include "FileClass.hpp"

#include <fstream>
#include <vector>
#include <conio.h>
#include <iostream>
#include <cstdio>

extern const char PATH_FILE_USERS[] = "Files\\UserBase.dat";
extern const char PATH_FILE_CLIENTS[] = "Files\\ClientBase.dat";

extern const int MIN_PASSWORD = 8;
extern const int MAX_PASSWORD = 32;
extern const int MIN_LOGIN = 3;
extern const int MAX_LOGIN = 32;
static const int TICKET_VECTOR_BUFF = 64;

extern const int ENTER_KEY = 13;

std::vector<User> User::vectorUsers;
std::vector<Client> Client::vectorClients;

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

int User::LoginUser()
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
		if (RNG::hash(password, vectorUsers[i].salt) == vectorUsers[i].hash)
		{
			if (vectorUsers[i].access != AccessLevel::NoAcess)
			{
				*this = vectorUsers[i];
				return vectorUsers[i].access;
			}
			else
			{
				return vectorUsers[i].access;
			}
		}
	}
	return -1;
}

int User::loginExist(std::string& newLogin)
{
	for (int i = 0; i < vectorUsers.size(); i++)
	{
		if (vectorUsers[i].login == newLogin)
			return i;
	}
	return -1;
}

bool User::ReadFile()
{
	return File::ReadFile(PATH_FILE_CLIENTS, vectorUsers);
}

bool User::WriteToFile()
{
	return File::WriteToFile(PATH_FILE_USERS, vectorUsers);
}

int User::GetFileStatus()
{
	return File::GetFileStatus(PATH_FILE_CLIENTS, vectorUsers);
}

bool User::NewUser(const int access)
{
	User temp;
	temp.InputUser(access);
	if (loginExist(temp.login) > -1)
		return false;
	vectorUsers.emplace_back(std::move(temp));
	return true;
}


bool User::checkForAdmin()
{
	for (auto& it : vectorUsers)
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



std::fstream& operator << (std::fstream& fs, User& user)
{
	fs << user.login << ' ';
	fs << user.hash << ' ';
	fs << user.salt << ' ';
	fs << user.access;
	return fs;
}

std::fstream& operator >> (std::fstream& fs, User& user)
{
	fs >> user.login;
	fs >> user.hash;
	fs >> user.salt;
	fs >> user.access;
	return fs;
}

Client::Client()
{
	tickets.reserve(TICKET_VECTOR_BUFF);
	login = "default";
}

int Client::GetFileStatus()
{
	return File::GetFileStatus(PATH_FILE_CLIENTS, vectorClients);
}

bool Client::ReadFile()
{
	return File::ReadFile(PATH_FILE_CLIENTS, vectorClients);
}

bool Client::WriteToFile()
{
	return File::WriteToFile(PATH_FILE_CLIENTS, vectorClients);
}

std::fstream& operator << (std::fstream& fs, const Client& client)
{
	fs << client.login << ' ';
	for (auto& it : client.tickets)
	{
		fs << it << ' ';
	}
	fs << '#';
	return fs;
}

std::fstream& operator >>(std::fstream& fs, Client& client)
{
	fs >> client.login;
	std::string str;
	while (fs.peek() != '#')
	{
		fs >> str;
		client.tickets.emplace_back(str);
	}
	return fs;
}

void Admin::SearchUsers()
{
}
