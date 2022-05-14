#pragma once
#include <string>
#include <vector>
#include "FlightClass.h"
#include "common.h"
#include <algorithm>
class Flight;

enum AccessLevel
{
	NoAcessLvl = 0,
	ClientLvl = 1,
	AdminLvl = 2,
	SuperAdminLvl = 3
};

class User
{
private:
	static std::vector<User> vector;
	std::string login;
	std::string hash;
	std::string salt;
	int access;
public:
	User();
	User(std::string login, const int access);
	User(const User& user);
	User(User&& user) noexcept;

	static void CreateNewFile();
	static int GetFileStatus();
	static bool ReadFile();
	static bool WriteToFile();
	static void VectorReserve(const size_t size = VECTOR_BUFF);
	void PushToVector();
	static size_t getVectorSize();

	void PrintInfo(const int& count = 1);
	void PrintInfoWithTop();
	static void PrintInfoWhole();
	static void PrintTopRow();
	

	std::string getLogin();
	void setLogin(std::string login);

	User InputUser(const int access);
	static int LoginUser(std::string* loginPtr = nullptr);
	static bool CreateNewUser(const int access = AccessLevel::NoAcessLvl);
	static bool CheckForSuperAdmin();
	static int loginExist(std::string& login);
	User operator = (const User& source);
	User operator = (User&& source) noexcept;

	friend std::fstream& operator << (std::fstream& fs, User& user);
	friend std::fstream& operator >> (std::fstream& fs, User& user);

	friend class Admin;
	static void Sort(const int type);

private:
	static void SortByLogin();
};

class BaseUser
{
protected:
	std::string login;
public:
	BaseUser();
	BaseUser(std::string& login);
	BaseUser(const BaseUser& other);
	virtual void ShowFlights() = 0;
};

class Admin : public BaseUser
{
public:
	static void AcceptAll();
	void ShowFlights() override;
};

class SuperAdmin : public Admin
{
public:
	bool AddAdmin();
};

class Client : public BaseUser
{
private:
	static std::vector<Client> vector;
	std::vector<std::string> tickets;
public:
	Client();
	Client(const std::string& login);
	Client(const Client& source);

	bool BookTicket(const int index, const int type);
	bool PrintTickets();

	void ShowFlights() override;

	void PushToVector();

	static Client getClient(std::string& login);
	static void CreateNewFIle();
	static int GetFileStatus();
	static bool ReadFile();
	static bool WriteToFile();
	static void VectorReserve(const size_t size = VECTOR_BUFF);
	friend std::fstream& operator << (std::fstream& fs, Client& client);
	friend std::fstream& operator>>(std::fstream& fs, Client& client);

	Client operator = (const Client& other);
};