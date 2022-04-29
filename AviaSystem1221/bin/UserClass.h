#pragma once
#include <string>
#include <vector>

#include "Hash.h"

class User
{
	std::string login;
	std::string hash;
	std::string salt;
	int access;

	static std::fstream file;
	static std::vector<User> userArray;

	User& ReadUser();
	static void WriteUser();

public:

	enum acessLevel
	{
		NoAcess = -1,
		Client = 0,
		Admin = 1
	};

	User();
	User(const User& user);
	User(User&& user);
	~User();
	static bool InitUserBase();
	static void ReadUserBase();

};

