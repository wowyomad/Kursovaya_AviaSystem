#pragma once
#include <string>
#include <vector>

#include "Hash.h"
#include "FlightClass.h"
class User
{
private:
	static std::vector<User> vectorUsers;
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

	static int GetFileStatus();
	static bool ReadFile();
	static bool WriteToFile();

	friend std::fstream& operator << (std::fstream& fs, User& user);
	friend std::fstream& operator >> (std::fstream& fs, User& user);
	User operator = (const User& source);
	User operator = (User&& source) noexcept;
	User InputUser(const int access);
	int LoginUser();
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
	Admin();
	
	void SortUsers();
	void SearchUsers();
	void ShowUser();
	void EditUsers();
};

class Client : public BaseClass
{
private:
	static std::vector<Client> vectorClients;
	std::vector<std::string> tickets;
public:
	Client();
	bool BuyTicket();

	static int GetFileStatus();
	static bool ReadFile();
	static bool WriteToFile();

	friend class Admin;
	friend std::fstream& operator<<(std::fstream& fs,const Client& client);
	friend std::fstream& operator>>(std::fstream& fs, Client& client);
};