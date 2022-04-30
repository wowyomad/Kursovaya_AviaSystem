#include "UserClass.h"
#include "FileHandle.h"

#include <fstream>
#include <vector>
#include <conio.h>
#include <iostream>
#include <cstdio>

#define PATH_USERBASE	"Files\\UserBase.bin"
#define VECTOR_BUFF		1024
#define MIN_PASSWORD	8
#define MIN_LOGIN		3
#define MAX_LOGIN		32
#define MAX_PASSWORD	32
#define STRING_BUFF		32
#define ENTER			13

std::fstream User::userBase;
std::vector<User> User::userArray;

User::User()
{
	login.reserve(STRING_BUFF);
	access = -1;

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

bool User::loginExist(std::string& newLogin)
{
	for (auto& it : userArray)
	{
		if (it.login == newLogin)
			return true;
	}
	return false;
}

User User::ReadUser()
{
	User temp;
	temp.login = FileHandle::ReadString(userBase);
	temp.hash = FileHandle::ReadString(userBase);
	temp.salt = FileHandle::ReadString(userBase);
	temp.access = FileHandle::ReadVar<int>(userBase);
	return temp;
}

void User::WriteUser()
{
	FileHandle::WriteString(userBase, login);
	FileHandle::WriteString(userBase, salt);
	FileHandle::WriteString(userBase, hash);
	FileHandle::WriteVar(userBase, access);
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

void User::NewUser()
{
	User temp;
	//Логин
	while (true)
	{
		std::cout << "Логин может содержать символы английского алфавита, цифры и нижнее подчеркивание.\n"
			<< "Логин: ";
		if (loginIn(temp.login))
		{
			if (loginExist(temp.login))
			{
				std::cout << "Логин уже существует.\n\n";
			}
			else break;
		}
		else
		{
			std::cout << "\nНекорректный ввод!\n\n";
		}
	}
	//Пароль
	while (true)
	{
		std::string password;
		password.reserve(32);
		std::cout << "Пароль может содеражть любые символы, за исключением кириллицы.\n"
			<< "Пароль: ";
		if (passwordIn(password))
		{
			temp.salt = RNG::salt();
			temp.hash = RNG::hash(password, temp.salt);
			break;
		}
		else
		{
			std::cout << "\nНекорректный ввод!\n\n";
		}
	}
	temp.access = AccessLevel::NoAcess;
	userArray.emplace_back(std::move(temp));
}

bool User::InitUserBase()
{
	userArray.reserve(VECTOR_BUFF);
	userBase.open(PATH_USERBASE, std::ios::in | std::ios::out | std::ios::binary);
	if (!userBase.is_open()) return false;
	else if (FileHandle::file_is_empty(userBase)) return true;
	do
	{
		userArray.emplace_back(std::move(User::ReadUser()));
	} while (!userBase.eof());
	return true;
}

bool passwordIn(std::string& password)
{
	password.clear();
	unsigned char ch = 0;
	do
	{
		ch = _getch();
		if (ch == ENTER)
		{
			printf("\n\n");
			if (password.length() < MIN_PASSWORD) return false;
			else if (password.length() > MAX_PASSWORD) return false;
			else return true;
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
			return false;
		}
		else
		{
			printf("*");
			password.push_back(ch);
		}
	} while (true);
}

bool loginIn(std::string& login)
{
	login.clear();
	unsigned char ch = 0;
	do
	{
		ch = _getch();
		if (ch == ENTER)
		{
			printf("\n\n");
			if (login.length() < MIN_LOGIN) return false;
			else if (login.length() > MAX_LOGIN) return false;
			else return true;
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
		else return false;
	} while (true);
}

bool is_russian(unsigned char& ch)
{
	return ch >= 'а' && ch <= 'я' || ch >= 'А' && ch <= 'Я';
}

bool is_english(unsigned char& ch)
{
	return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}
