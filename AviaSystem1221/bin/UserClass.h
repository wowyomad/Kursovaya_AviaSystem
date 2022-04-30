#pragma once
#include <string>
#include <vector>

#include "Hash.h"
#include "FileHandle.h"

void passwordIn(std::string& password);
void loginIn(std::string& login);
bool is_russian(unsigned char& ch);
bool is_english(unsigned char& ch);


class User
{
private:
	std::string login;
	std::string hash;
	std::string salt;
	int access;

	static std::fstream userBase;
	static std::vector<User> userArray;

	static int loginExist(std::string& login);
	//Чтение данных из файла в объект
	static User ReadUser();
	//Запись данных из объекта в файл
	void WriteUser();

public:
	enum AccessLevel
	{
		NoAcess = 0,
		Client = 1,
		Admin = 2
	};

	User();
	User(const User& user);
	User(User&& user) noexcept;
	~User();

	User operator =(const User& source);

	User UserIn(const int& access = User::AccessLevel::NoAcess);
	int Login();
	//Вввод пользователем логина и пароля и сохранение в массив
	static bool NewUser(const int& access = 0);
	static int InitUserBase();
	static void ReadUserBase();
	//Создание файла пользовательской базы
	static void CreateUserBase();
	//Сохранение массива в пользовательскую базу
	static void SaveUserArray();

};

