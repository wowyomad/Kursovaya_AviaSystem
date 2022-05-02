#include "UserClass.h"
#include "FileHandle.h"

#include <fstream>
#include <vector>
#include <conio.h>
#include <iostream>
#include <cstdio>

extern const std::string PATH_USERBASE = "Files\\UserBase.dat";
static const int VECTOR_BUFF = 1024;
static const int MIN_PASSWORD = 8;
static const int MIN_LOGIN = 3;
static const int MAX_LOGIN = 32;
static const int MAX_PASSWORD = 32;
static const int STRING_RESERVE = 64;
static const int ENTER_KEY = 13;

std::fstream User::userBase;
std::vector<User> User::userArray;

User::User()
{
	login.reserve(MAX_LOGIN);
	access = 0;
}

User::User(const User& user)
{
	login = user.login;
	hash = user.hash;
	salt = user.salt;
	access = user.access;
}

User::User(User&& user) noexcept
{
	login = std::move(user.login);
	hash = std::move(user.hash);
	salt = std::move(user.salt);
	access = user.access;
}

User::~User()
{

}

User User::operator=(const User& source)
{
	this->login = source.login;
	this->hash = source.hash;
	this->salt = source.salt;
	this->access = source.access;
	return *this;
}

User User::UserIn(const int& access)
{
	loginIn(login);
	std::string password;
	passwordIn(password);
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
	loginIn(login);
	passwordIn(password);
	int i = loginExist(login);
	if (i > -1)
	{
		if (RNG::hash(password, userArray[i].salt) == userArray[i].hash)
		{
			if (userArray[i].access != AccessLevel::NoAcess)
			{
				*this = userArray[i];
				return userArray[i].access;
			}
			else
			{
				return userArray[i].access;
			}
		}
	}
	return -1;
}

int User::loginExist(std::string& newLogin)
{
	for (int i = 0; i < userArray.size(); i++)
	{
		if (userArray[i].login == newLogin)
				return i;
	}
	return -1;
}

User User::ReadUser()
{
	User user;
	userBase >> user.login;
	userBase >> user.hash;
	userBase >> user.salt;
	userBase >> user.access;
	return user;
}

void User::WriteUser()
{
	userBase << login << '\n';
	userBase << hash << '\n';
	userBase << salt << '\n';
	userBase << access;
}

void User::CreateUserBase()
{
	userBase.close();
	userBase.open(PATH_USERBASE, std::ios::out);
	userBase.close();
	userBase.open(PATH_USERBASE, std::ios::out | std::ios::in | std::ios::binary);
}

void User::SaveUserArray()
{
	CreateUserBase();
	for (auto& it : userArray)
	{
		it.WriteUser();
	}
}

bool User::NewUser(const int& access)
{
	User temp;
	temp.UserIn(access);
	if (loginExist(temp.login) > -1) 
		return false;
	userArray.emplace_back(std::move(temp));
	return true;
}

int User::InitUserBase()
{
	userArray.reserve(VECTOR_BUFF);
	userBase.open(PATH_USERBASE, std::ios::in | std::ios::out | std::ios::binary);
	switch (getFileStatus(userBase))
	{
	case FileStatus::NotOpened:
		return FileStatus::NotOpened;
	case FileStatus::Empty:
		return FileStatus::Empty;
	case FileStatus::Opened:
		break;
	default:
		return -1;
	}
	std::string t;
	while (!userBase.eof())
	{
		userArray.emplace_back(std::move(User::ReadUser()));
		wchar_t temp;
		userBase.read((char*)&temp, sizeof(wchar_t));
	}
	return FileStatus::Opened;
}

bool User::checkForAdmin()
{
	for (auto& it : userArray)
	{
		if (it.access == AccessLevel::Admin)
			return true;
	}
	return false;
}

void passwordIn(std::string& password)
{
	std::cout << "������ ����� ��������� ����� �������, �� ����������� ���������.\n"
		<< "������: ";
	password.clear();
	unsigned char ch = 0;
	do
	{
		ch = _getch();
		if (ch == ENTER_KEY)
		{
			if (password.length() < MIN_PASSWORD)
			{
				std::cout << "\n������. ������ ������� ��������\n\n"
					<< "������: ";
				password.clear();
			}
			else if (password.length() > MAX_PASSWORD)
			{
				std::cout << "\n������. ������ ������� �������\n\n"
					<< "������: ";
				password.clear();
			}
			else
			{
				printf("\n\n");
				break;
			}
		}
		else if (ch == '\b')
		{
			if (password.length() > 0)
			{
				printf("\b \b");
				password.pop_back();
			}
		}
		else if (is_russian(ch))
		{
			std::cout << "\n������. ������ ������� ������!\n\n"
				<< "������: ";
		}
		else
		{
			printf("*");
			password.push_back(ch);
		}
	} while (true);
}

void loginIn(std::string& login)
{
	std::cout << "����� ����� ��������� ������� ����������� ��������, ����� � ������ �������������.\n"
		<< "�����: ";
	login.clear();
	unsigned char ch = 0;
	do
	{
		ch = _getch();
		if (ch == ENTER_KEY)
		{
			if (login.length() < MIN_LOGIN)
			{
				std::cout << "\n������. ����� ������� ��������\n\n"
					<< "�����: ";
				login.clear();
			}
			else if (login.length() > MAX_LOGIN)
			{
				std::cout << "\n������. ����� ������� �������\n\n"
					<< "�����: ";
				login.clear();
			}
			else
			{
				printf("\n\n");
				break;
			}
		}
		else if (ch == '\b')
		{
			if (login.length() > 0)
			{
				printf("\b \b");
				login.pop_back();
			}
		}
		else if (ch == ' ')
		{
			continue;
		}
		else if (is_english(ch) || isdigit(ch) || ch == '_')
		{
			printf("%c", ch);
			login.push_back(ch);
		}
		else
		{
			std::cout << "\n������������ ������\n\n"
				<< "�����: ";
			login.clear();
		}
	} while (true);
}

bool is_russian(unsigned char& ch)
{
	return ch >= '�' && ch <= '�' || ch >= '�' && ch <= '�';
}

bool is_english(unsigned char& ch)
{
	return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}
