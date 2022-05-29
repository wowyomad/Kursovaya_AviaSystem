#pragma once
#include <string>
#include <vector>
#include "FlightClass.h"
#include "common.h"
#include <algorithm>
class Flight;

enum class AccessLevel
{
	NoAccess = 0,
	Client = 1,
	Admin = 2,
	SuperAdmin = 3
};

AccessLevel stringToEnum(std::string& s, const std::string*& enumName, const int size);

class User
{
private:
	std::string login;
	std::string hash;
	std::string salt;
	AccessLevel access;
	std::vector<std::string> tickets;

	static std::vector<User> vector;
public:
	User();
	User(const User& user);
	User(User&& user) noexcept;

	void ChangePassword(std::string& newPassword);

	static void CreateNewFile();
	static int GetFileStatus();
	static bool ReadFile();
	static bool WriteToFile();
	
	static void CopyVectorPtr(std::vector<User*>& destination);
	void PushToVector();
	static size_t getVectorSize();

	void PrintInfo(const int& count = 1);
	void PrintInfoWithTop();
	static void PrintInfoVector();
	static void PrintInfoVector(std::vector<User*>& userVector);
	static void PrintTopRow();

	static void UserErase(const int index);
	static User* GetUserPtr(const int index);
	static User* GetUserPTr(const std::string& login);
	static size_t getUserIndex(std::string& login);

	std::string getLogin();
	size_t getTicketCount();
	AccessLevel getAccessLevel();

	void setLogin(std::string login);
	void setAccessLevel(AccessLevel access);


	bool BookTicket(Flight* flight, const int type, const int count);

	void ShowTickets();



	static void AcceptAll();
	static User* LoginUser(std::string* loginPtr = nullptr);
	static bool CreateNewUser(const AccessLevel);

	User InputInfo(const AccessLevel);
	void RemoveUser(const int index);
	static bool CheckForSuperAdmin();
	static int loginExist(std::string& login);

	User operator = (const User& source);
	User operator = (User&& source) noexcept;

	friend std::fstream& operator << (std::fstream& fs, User& user);
	friend std::fstream& operator >> (std::fstream& fs, User& user);

	static void SortByLogin(std::vector<User*> userVector);
};