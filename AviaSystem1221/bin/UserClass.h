#pragma once
#include <string>
#include <vector>

#include "Hash.h"

bool passwordIn(std::string& password);
bool loginIn(std::string& login);
bool is_russian(unsigned char& ch);
bool is_english(unsigned char& ch);

class User
{
	std::string login;
	std::string hash;
	std::string salt;
	int access;

	static std::fstream userBase;
	static std::vector<User> userArray;

	static bool loginExist(std::string& login);
	//Чтение данных из файла в объект
	static User ReadUser();
	//Запись данных из объекта в файл
	void WriteUser();

public:

	enum AccessLevel
	{
		NoAcess = -1,
		Client = 0,
		Admin = 1
	};

	User();
	User(const User& user);
	User(User&& user) noexcept;
	~User();
	//Вввод пользователем логина и пароля и сохранение в массив
	static void NewUser();
	static bool InitUserBase();
	static void ReadUserBase();
	//Создание файла пользовательской базы
	static void CreateUserBase();
	//Сохранение массива в пользовательскую базу
	static void SaveUserArray();

};

