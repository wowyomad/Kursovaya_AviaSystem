#include "UserClass.h"
#include "UserConsoleInput.h"
#include "FileClass.hpp"
#include "Hash.h"

#include <fstream>
#include <vector>
#include <conio.h>
#include <iostream>
#include <cstdio>

extern const char PATH_FILE_USERS[] = "Files\\UserBase.dat";
extern const char PATH_FILE_CLIENTS[] = "Files\\ClientBase.dat";

extern const int MIN_PASSWORD = 8;
extern const int MAX_PASSWORD = 32;
extern const int MIN_LOGIN = 3;
extern const int MAX_LOGIN = 32;
static const int TICKET_VECTOR_BUFF = 64;

extern const int ENTER_KEY = 13;

std::vector<Entity> Entity::vectorUsers;
std::vector<Client> Client::vectorClients;

Entity::Entity()
{
	login.reserve(MAX_LOGIN);
	access = 0;
}

Entity::Entity(const Entity& source)
{
	login = source.login;
	hash = source.hash;
	salt = source.salt;
	access = source.access;
}

Entity::Entity(Entity&& source) noexcept
{
	login = std::move(source.login);
	hash = std::move(source.hash);
	salt = std::move(source.salt);
	access = source.access;
}

BaseClass::BaseClass() : name("default") {};

BaseClass::BaseClass(std::string& login) : name(login) {};

Entity Entity::operator=(const Entity& source)
{
	login = source.login;
	hash = source.hash;
	salt = source.salt;
	access = source.access;
	return *this;
}

Entity Entity::operator = (Entity&& source) noexcept
{
	login = std::move(source.login);
	hash = std::move(source.hash);
	salt = std::move(source.salt);
	access = source.access;
	return *this;
}

Entity Entity::InputUser(const int access)
{
	InputLogin(login, MIN_LOGIN, MAX_LOGIN);
	std::string password;
	InputPassword(password, MIN_PASSWORD, MAX_PASSWORD);
	salt = RNG::randomNumber();
	hash = RNG::hash(password, salt);
	this->access = access;
	return *this;
}

int Entity::LoginEntity()
{
	std::string password;
	std::string login;
	password.reserve(MAX_PASSWORD);
	login.reserve(MAX_LOGIN);
	InputLogin(login, MIN_LOGIN, MAX_LOGIN);
	InputPassword(password, MIN_PASSWORD, MAX_PASSWORD);
	int i = loginExist(login);
	if (i > -1)
	{
		if (RNG::hash(password, vectorUsers[i].salt) == vectorUsers[i].hash)
		{
			if (vectorUsers[i].access != AccessLevel::NoAcessLvl)
			{
				return vectorUsers[i].access;
			}
			else
			{
				return vectorUsers[i].access;
			}
		}
	}
	return -1;
}

int Entity::loginExist(std::string& newLogin)
{
	for (int i = 0; i < vectorUsers.size(); i++)
	{
		if (vectorUsers[i].login == newLogin)
			return i;
	}
	return -1;
}

void Entity::CreateFile()
{
	std::fstream file(PATH_FILE_CLIENTS);
	file.close();
}

bool Entity::ReadFile()
{
	if (GetFileStatus() == FileStatus::Opened)
		return File::ReadFile(PATH_FILE_CLIENTS, vectorUsers);
	else return false;
}

bool Entity::WriteToFile()
{
	return File::WriteToFile(PATH_FILE_USERS, vectorUsers);
}

void Entity::VectorReserve(const size_t size)
{
	vectorUsers.reserve(size);
}

std::string Entity::getLogin()
{
	return login;
}

int Entity::GetFileStatus()
{
	return File::GetFileStatus(PATH_FILE_CLIENTS, vectorUsers);
}

bool Entity::AddNewEntity(const int access)
{
	Entity temp;
	temp.InputUser(access);
	if (loginExist(temp.login) > -1)
		return false;
	else
		vectorUsers.emplace_back(std::move(temp));
	return true;
}


bool Entity::CheckFileForAdmin()
{
	for (auto& it : vectorUsers)
	{
		if (it.access == AccessLevel::AdminLvl)
			return true;
	}
	return false;
}

void BaseClass::ShowFlights(const int mode)
{

}

void BaseClass::SearchFlights()
{

}

void BaseClass::SortFligths()
{

}

std::fstream& operator << (std::fstream& fs, Entity& user)
{
	fs << user.login << ' ';
	fs << user.hash << ' ';
	fs << user.salt << ' ';
	fs << user.access;
	return fs;
}

std::fstream& operator >> (std::fstream& fs, Entity& user)
{
	fs >> user.login;
	fs >> user.hash;
	fs >> user.salt;
	fs >> user.access;
	return fs;
}

Client::Client()
{
	tickets.reserve(TICKET_VECTOR_BUFF);
	name = "default";
}

Client::Client(const std::string& login)
{
	this->name = login;
}

Client::Client(const Client& source)
{
	*this = source;
}

bool Client::BookTicket(Flight& flight, const int type)
{
	if (type == TicketType::Business || type == TicketType::Economy)
	{
		if (flight.TicketAvailable(type))
		{
			tickets.emplace_back(std::move(flight.GenerateTicketID(type)));
			flight.TakeTicket(type);
			return true;
		}
	}
	return false;
}

void Client::PrintTickets()
{
	if (tickets.size() == 0)
	{
		std::cout << "У вас нет билетов.\n";
	}

	std::vector<Ticket> ticketsInfo;
	ticketsInfo.reserve(tickets.size());
	Ticket ticket;
	for (auto& it : tickets)
	{
		ticket = Flight::LookUpForFlight(it);
		if (ticket.getType() != -1)
		{
			ticket.setType(std::atoi(&(it.back())));
			ticketsInfo.emplace_back(std::move(ticket));
		}
		else
			std::cout << "Билет " << it << " не найден.\n";
	}

	if (ticketsInfo.size() != 0)
	{
		Ticket::PrintTopRow();
		for (auto& it : ticketsInfo)
		{
			it.PrintInfo();
		}
	}
	else
	{
		std::cout << "Информации по вашим билетам не найдено.\n";
	}
}

void Client::CreateFile()
{
	std::fstream file(PATH_FILE_CLIENTS, std::ios::out);
	file.close();
}

int Client::GetFileStatus()
{
	return File::GetFileStatus(PATH_FILE_CLIENTS, vectorClients);
}

bool Client::ReadFile()
{
	if (GetFileStatus() == FileStatus::Opened)
		return File::ReadFile(PATH_FILE_CLIENTS, vectorClients);
	else return false;
}

bool Client::WriteToFile()
{
	return File::WriteToFile(PATH_FILE_CLIENTS, vectorClients);
}

void Client::VectorReserve(const size_t VECTOR_BUFF)
{
	vectorClients.reserve(VECTOR_BUFF);
}

std::fstream& operator >>(std::fstream& fs, Client& client)
{
	fs >> client.name;
	std::string str;
	while (fs.peek() != '\n')
	{
		fs >> str;
		client.tickets.emplace_back(str);
	}
	return fs;
}

std::fstream& operator << (std::fstream& fs, Client& client)
{
	fs << client.name << ' ';
	for (auto& it : client.tickets)
	{
		fs << it << ' ';
	}
	return fs;
}

void Admin::SortUsers()
{

}

void Admin::EditUsers()
{
}

void Admin::SearchUsers()
{

}