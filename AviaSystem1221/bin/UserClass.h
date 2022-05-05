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
	virtual void ShowFlights(const int mode); 
	virtual void SearchFlights();
	virtual void SortFligths();
	BaseClass();
	BaseClass(std::string& login);
};


class Admin : public BaseClass
{
public:
	Admin() : BaseClass() {};
	
	void SortUsers();
	void SearchUsers();
	void ShowUser();
	void EditUsers();
};

class Client : public BaseClass
{
private:

public:
	Client() : BaseClass() {};

	bool BuyTicket();


};