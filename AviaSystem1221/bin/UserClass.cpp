#include "UserClass.h"
#include "UserConsoleInput.h"
#include "FileClass.hpp"
#include "Hash.h"
#include "Table.h"

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

std::vector<User> User::vector;
std::vector<Client> Client::vector;

User::User()
{
	login.reserve(MAX_LOGIN);
	access = 0;
}

User::User(std::string login, const int access) : login(login), access(access) {}
User::User(const User& source)
{
	login = source.login;
	hash = source.hash;
	salt = source.salt;
	access = source.access;
}

User::User(User&& source) noexcept
{
	login = std::move(source.login);
	hash = std::move(source.hash);
	salt = std::move(source.salt);
	access = source.access;
}

BaseUser::BaseUser() : login("default") {};

BaseUser::BaseUser(std::string& login) : login(login) {}


User User::operator=(const User& source)
{
	login = source.login;
	hash = source.hash;
	salt = source.salt;
	access = source.access;
	return *this;
}

User User::operator = (User&& source) noexcept
{
	login = std::move(source.login);
	hash = std::move(source.hash);
	salt = std::move(source.salt);
	access = source.access;
	return *this;
}

void User::Sort(const int type)
{
	switch (type)
	{
	case UserSortType::Login:
		SortByLogin();
		break;
	default:
		break;
	}
}

void User::SortByLogin()
{
	std::sort(vector.begin(), vector.end(), [](const User& l, const User& r)
		{
			return l.login < r.login;
		});
}

User User::InputUser(const int access)
{
	InputLogin(login, MIN_LOGIN, MAX_LOGIN);
	std::string password;
	InputPassword(password, MIN_PASSWORD, MAX_PASSWORD);
	salt = RNG::randomNumber();
	hash = RNG::hash(password, salt);
	this->access = access;
	return *this;
}

int User::LoginUser(std::string* loginPtr)
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
		if (RNG::hash(password, vector[i].salt) == vector[i].hash)
		{
			if (loginPtr != nullptr)
				*loginPtr = login;
			return vector[i].access;
		}
	}
	return -1;
}

int User::loginExist(std::string& newLogin)
{
	for (int i = 0; i < vector.size(); i++)
	{
		if (vector[i].login == newLogin)
			return i;
	}
	return -1;
}

void User::CreateNewFile()
{
	std::fstream file(PATH_FILE_CLIENTS);
	file.close();
}

bool User::ReadFile()
{
	if (GetFileStatus() == FileStatus::Opened)
		return File::ReadFile(PATH_FILE_USERS, vector);
	else return false;
}

bool User::WriteToFile()
{
	return File::WriteToFile(PATH_FILE_USERS, vector);
}

void User::VectorReserve(const size_t size)
{
	vector.reserve(size);
}

void User::PushToVector()
{
	vector.emplace_back(*this);
}

size_t User::getVectorSize()
{
	return vector.size();
}

void User::PrintInfo(const int& count)
{
	std::vector<std::string> row;
	row.reserve(3);
	row.emplace_back(std::to_string(count));
	row.emplace_back(login);
	switch (access)
	{
	case AccessLevel::NoAcessLvl:
		row.emplace_back("Нет доступа");
		break;
	case AccessLevel::ClientLvl:
		row.emplace_back("Клиент");
		break;
	case AccessLevel::AdminLvl:
		row.emplace_back("Админ");
		break;
	case AccessLevel::SuperAdminLvl:
		row.emplace_back("Супер-админ");
		break;
	default:
		row.emplace_back("Неизвестно");
		break;
	}
	ClFomrat::PrintCenteredRow(row, CELL_WIDTH, ClFomrat::Border::Both, ClFomrat::Border::Bottom, ClFomrat::Border::Both);
}

void User::PrintInfoWithTop()
{
	PrintTopRow();
	PrintInfo();
}

void User::PrintInfoWhole()
{
	if (vector.size() < 1) return;

	PrintTopRow();
	int count = 1;
	for (auto& it : vector)
		it.PrintInfo(count++);
}


void User::PrintTopRow()
{
	std::vector<std::string> row;
	row.reserve(3);
	row.emplace_back("Порядковый номер");
	row.emplace_back("Логин");
	row.emplace_back("Доступ");
	ClFomrat::PrintCenteredRow(row, CELL_WIDTH, ClFomrat::Border::NoBorder, ClFomrat::Border::Bottom, ClFomrat::Border::NoBorder);
}



std::string User::getLogin()
{
	return login;
}

void User::setLogin(std::string login)
{
	this->login = login;
}

int User::GetFileStatus()
{
	return File::GetFileStatus(PATH_FILE_USERS, vector);
}

bool User::CreateNewUser(const int access)
{
	User temp;
	temp.InputUser(access);
	if (loginExist(temp.login) > -1)
		return false;
	else
		vector.emplace_back(std::move(temp));
	return true;
}

bool User::CheckForSuperAdmin()
{
	for (auto& it : vector)
	{
		if (it.access == AccessLevel::AdminLvl)
			return true;
	}
	return false;
}


std::fstream& operator << (std::fstream& fs, User& user)
{
	fs << user.login << ' ';
	fs << user.hash << ' ';
	fs << user.salt << ' ';
	fs << user.access;
	return fs;
}

std::fstream& operator >> (std::fstream& fs, User& user)
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
	login = "default";
}

Client::Client(const std::string& login)
{
	tickets.reserve(TICKET_VECTOR_BUFF);
	this->login = login;
}

Client::Client(const Client& source)
{
	login = source.login;
	tickets = source.tickets;
}

bool Client::BookTicket(const int index, const int type)
{
	try
	{
		if (type == TicketType::Business || type == TicketType::Economy)
		{
			if (Flight::TicketAvailable(index, type))
			{
				std::string fullTicketId = Flight::GenerateTicketID(index, type);
				tickets.push_back(fullTicketId);
				Flight::TakeTicket(index, type);
				return true;
			}
		}
	}
	catch (std::invalid_argument& exc)
	{

		std::cout << exc.what() << '\n';
		return -1;
	}
	return false;
}

bool Client::PrintTickets()
{
	if (tickets.size() < 1) return false;

	std::vector<Ticket> ticketsInfo;
	ticketsInfo.reserve(tickets.size());
	Ticket ticket;
	for (auto& it : tickets)
	{
		ticket = Flight::GetFlightTicket(it);
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

void Client::ShowFlights()
{
	Flight::PrintInfoVector(InfoMode::UserMode);
}

void Client::PushToVector()
{
	vector.emplace_back(*this);
}

Client Client::getClient(std::string& login)
{
	for (auto& it : vector)
	{
		if (it.login == login)
			return it;
	}
	throw std::invalid_argument("Клиент не был найден в клиентской базе. Возможно, клиентская и пользовательские базы не синхронизированы");
}

void Client::CreateNewFIle()
{
	std::fstream file(PATH_FILE_CLIENTS, std::ios::out);
	file.close();
}

int Client::GetFileStatus()
{
	return File::GetFileStatus(PATH_FILE_CLIENTS, vector);
}

bool Client::ReadFile()
{
	if (GetFileStatus() == FileStatus::Opened)
		return File::ReadFile(PATH_FILE_CLIENTS, vector);
	else return false;
}

bool Client::WriteToFile()
{
	return File::WriteToFile(PATH_FILE_CLIENTS, vector);
}

void Client::VectorReserve(const size_t VECTOR_BUFF)
{
	vector.reserve(VECTOR_BUFF);
}

Client Client::operator=(const Client& other)
{
	login = other.login;
	tickets = other.tickets;
	return *this;
}

std::fstream& operator >>(std::fstream& fs, Client& client)
{
	fs >> client.login;
	std::string str;
	fs >> str;
	if (str != "#")
	{
		client.tickets.emplace_back(str);
		while (fs.peek() != '#')
		{
			fs.get();
			fs >> str;
			client.tickets.emplace_back(str);
		}
	}
	fs.get();
	return fs;
}

std::fstream& operator << (std::fstream& fs, Client& client)
{
	fs << client.login << '\n';
	long long limiter = client.tickets.size() - 1;
	if (limiter > -1)
	{
		for (long long i = 0; i < limiter; i++)
		{
			fs << client.tickets[i] << '\n';
		}
		fs << client.tickets[limiter] << '\n';
	}
	fs << '#';
	return fs;
}

bool SuperAdmin::AddAdmin()
{
	return User::CreateNewUser(AccessLevel::AdminLvl);
}

void Admin::AcceptAll()
{
	for (auto& it : User::vector)
	{
		if (it.access == AccessLevel::NoAcessLvl)
		{
			Client newClient(it.login);
			newClient.PushToVector();
			it.access = AccessLevel::ClientLvl;
		}
	}
}

void Admin::ShowFlights()
{
	Flight::PrintInfoVector(InfoMode::AdminMode);
}
