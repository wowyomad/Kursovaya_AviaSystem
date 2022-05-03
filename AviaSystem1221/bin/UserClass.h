#pragma once
#include <string>
#include <vector>

#include "Hash.h"
#include "FlightClass.h"

class User
{
private:
	static std::ofstream UserFileOut;
	static std::ifstream UserFileIn;
	static std::fstream UserFile;
	static std::vector<User> userVector;
	std::string login;
	std::string hash;
	std::string salt;
	int access;
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

	friend std::fstream operator << (std::fstream& fs, User& user);
	friend std::fstream operator >> (std::fstream& fs, User& user);
	User operator = (const User& source);
	User operator = (User&& source) noexcept;
	User InputUser(const int access);
	int Login();
	static bool NewUser(const int access = AccessLevel::NoAcess);
	static bool checkForAdmin();
	static int loginExist(std::string& login);
};

class BaseClass
{
protected:
	std::string login;
public:
	virtual void Show() = 0;
	virtual void Search() = 0;
	virtual void Sort() = 0;
};

class Admin : public BaseClass
{
public:
	Admin() : BaseClass() {};
	void Sort() override;
	void Search() override;
	void Show() override;
	void Edit();
};

class Client : public BaseClass
{
private:

public:
	Client() : BaseClass() {};
	void Show() override;
	void Search() override;
	void Sort() override;
	bool BuyTicket();


};