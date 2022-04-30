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
	//������ ������ �� ����� � ������
	static User ReadUser();
	//������ ������ �� ������� � ����
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
	//����� ������������� ������ � ������ � ���������� � ������
	static void NewUser();
	static bool InitUserBase();
	static void ReadUserBase();
	//�������� ����� ���������������� ����
	static void CreateUserBase();
	//���������� ������� � ���������������� ����
	static void SaveUserArray();

};

