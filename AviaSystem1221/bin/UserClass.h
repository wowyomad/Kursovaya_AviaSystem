#pragma once
#include <string>
#include <vector>
#include "FlightClass.h"
#include "common.h"
class Flight;

enum AccessLevel
{
	NoAcessLvl = 0,
	ClientLvl = 1,
	AdminLvl = 2
};

class Entity
{
private:
	static std::vector<Entity> vectorUsers;
	std::string login;
	std::string hash;
	std::string salt;
	int access;
public:
	Entity();
	Entity(const Entity& user);
	Entity(Entity&& user) noexcept;

	static void CreateFile();
	static int GetFileStatus();
	static bool ReadFile();
	static bool WriteToFile();
	static void VectorReserve(const size_t size = VECTOR_BUFF);

	std::string getLogin();

	Entity InputUser(const int access);
	static int LoginEntity();

	static bool AddNewEntity(const int access = AccessLevel::NoAcessLvl);
	static bool CheckFileForAdmin();
	static int loginExist(std::string& login);

	Entity operator = (const Entity& source);
	Entity operator = (Entity&& source) noexcept;

	friend std::fstream& operator << (std::fstream& fs, Entity& user);
	friend std::fstream& operator >> (std::fstream& fs, Entity& user);
};

class BaseClass
{
protected:
	std::string name;
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

	void EditFlights();
};

class Client : public BaseClass
{
private:
	static std::vector<Client> vectorClients;
	std::vector<std::string> tickets;
public:
	Client();
	Client(const std::string& login);
	Client(const Client& source);

	bool BookTicket(Flight& flight, const int type);
	void PrintTickets();

	static void CreateFile();
	static int GetFileStatus();
	static bool ReadFile();
	static bool WriteToFile();
	static void VectorReserve(const size_t size = VECTOR_BUFF);
	friend std::fstream& operator << (std::fstream& fs, Client& client);
	friend std::fstream& operator>>(std::fstream& fs, Client& client);
};