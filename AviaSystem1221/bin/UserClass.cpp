#include "UserClass.h"
#include "FileHandle.h"

#include <fstream>
#include <vector>
#include <conio.h>
#include <iostream>
#include <cstdio>

static const std::string PATH_USERBASE = "Files\\UserBase.dat";
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
	//hash и salt занимают по 8 байт, поэтмоу
	//резервировать место вручную не надо.
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
	std::string line;
	std::getline(userBase, user.login);
	std::getline(userBase, user.hash);
	std::getline(userBase, user.salt);
	std::getline(userBase, line);
	user.access = std::stoi(line);
	return user;
}

void User::WriteUser()
{
	userBase << '\n';
	userBase << login << '\n';
	userBase << hash << '\n';
	userBase << salt << '\n';
	userBase << access << '\n';
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
	while (std::getline(userBase, t))
	{
		userArray.emplace_back(std::move(User::ReadUser()));
	}
	return FileStatus::Opened;
}

void passwordIn(std::string& password)
{
	std::cout << "Пароль может содеражть любые символы, за исключением кириллицы.\n"
		<< "Пароль: ";
	password.clear();
	unsigned char ch = 0;
	do
	{
		ch = _getch();
		if (ch == ENTER_KEY)
		{
			if (password.length() < MIN_PASSWORD)
			{
				std::cout << "\nОшибка. Пароля слишком короткий\n\n"
					<< "Пароль: ";
				password.clear();
			}
			else if (password.length() > MAX_PASSWORD)
			{
				std::cout << "Ошибка. Пароль слишком длинный\n\n"
					<< "Пароль: ";
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
			std::cout << "\nОшибка. Введен русский символ!\n\n"
				<< "Пароль: ";
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
	std::cout << "Логин может содержать символы английского алфавита, цифры и нижнее подчеркивание.\n"
		<< "Логин: ";
	login.clear();
	unsigned char ch = 0;
	do
	{
		ch = _getch();
		if (ch == ENTER_KEY)
		{
			if (login.length() < MIN_LOGIN)
			{
				std::cout << "\nОшибка. Логин слишком короткий\n\n"
					<< "Логин: ";
				login.clear();
			}
			else if (login.length() > MAX_LOGIN)
			{
				std::cout << "Ошибка. Логин слишком короткий\n\n"
					<< "Логин: ";
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
			std::cout << "\nНедопустимый символ\n\n"
				<< "Логин: ";
			login.clear();
		}
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
