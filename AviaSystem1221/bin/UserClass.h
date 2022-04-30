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
	//������ ������ �� ����� � ������
	static User ReadUser();
	//������ ������ �� ������� � ����
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
	//����� ������������� ������ � ������ � ���������� � ������
	static bool NewUser(const int& access = 0);
	static int InitUserBase();
	static void ReadUserBase();
	//�������� ����� ���������������� ����
	static void CreateUserBase();
	//���������� ������� � ���������������� ����
	static void SaveUserArray();

};

