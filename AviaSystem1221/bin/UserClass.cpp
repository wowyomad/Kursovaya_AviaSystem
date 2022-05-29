#include "UserClass.h"
#include "UserConsoleInput.h"
#include "FileClass.hpp"
#include "Hash.h"
#include "PrintFormat.h"

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
static const int TICKET_VECTOR_BUFF = 16;

extern const int ENTER_KEY = 13;

std::vector<User> User::vector;
User::User()
{
	login = "undefined";
	hash = "undefined";
	salt = "undefined";
	access = AccessLevel::NoAccess;
}

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
	tickets = std::move(source.tickets);
}

User User::operator=(const User& source)
{
	login = source.login;
	hash = source.hash;
	salt = source.salt;
	access = source.access;
	tickets = source.tickets;
	return *this;
}

User User::operator = (User&& source) noexcept
{
	login = std::move(source.login);
	hash = std::move(source.hash);
	salt = std::move(source.salt);
	access = source.access;
	tickets = std::move(source.tickets);
	return *this;
}

void User::SortByLogin(std::vector<User*> userVector)
{
	std::sort(userVector.begin(), userVector.end(), [](const User* l, const User* r)
		{
			return l->login < r->login;
		});
}

User User::InputInfo(const AccessLevel access)
{
	InputLogin(login, MIN_LOGIN, MAX_LOGIN);
	std::string password;
	InputPassword(password, MIN_PASSWORD, MAX_PASSWORD);
	salt = RNG::randomNumber();
	hash = RNG::hash(password, salt);
	this->access = access;
	return *this;
}

User* User::LoginUser(std::string* loginPtr)
{
	std::string password;
	std::string login;
	password.reserve(MAX_PASSWORD);
	login.reserve(MAX_LOGIN);
	InputLogin(login, MIN_LOGIN, MAX_LOGIN);
	InputPassword(password, MIN_PASSWORD, MAX_PASSWORD);
	const int i = loginExist(login);
	if (i > -1)
	{
		if (RNG::hash(password, vector[i].salt) == vector[i].hash)
		{
			if (loginPtr != nullptr)
				*loginPtr = login;

			return &vector[i];
		}
	}
	throw std::exception("Пользователь не найден");
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
	if (vector.size() > 0)
		vector.erase(vector.begin(), vector.end());
	vector.reserve(VECTOR_BUFF);
	if (GetFileStatus() == FileStatus::Opened)
		return File::ReadFile(PATH_FILE_USERS, vector);
	else return false;
}

bool User::WriteToFile()
{
	return File::WriteToFile(PATH_FILE_USERS, vector);
}

void User::CopyVectorPtr(std::vector<User*>& destination)
{
	destination.clear();
	destination.reserve(vector.size());
	for (size_t i = 0; i < vector.size(); i++)
	{
		destination.emplace_back(&vector[i]);
	}
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
	case AccessLevel::NoAccess:
		row.emplace_back("Нет доступа");
		break;
	case AccessLevel::Client:
		row.emplace_back("Клиент");
		break;
	case AccessLevel::Admin:
		row.emplace_back("Админ");
		break;
	case AccessLevel::SuperAdmin:
		row.emplace_back("Супер-админ");
		break;
	default:
		row.emplace_back("Неизвестно");
		break;
	}
	ClFomrat::PrintRow(row, CELL_WIDTH, ClFomrat::Border::Both, ClFomrat::Border::Bottom, ClFomrat::Border::Both);
}

void User::PrintInfoWithTop()
{
	PrintTopRow();
	PrintInfo();
}

void User::PrintInfoVector()
{
	if (vector.size() < 1) return;

	PrintTopRow();
	int count = 1;
	for (auto& it : vector)
		it.PrintInfo(count++);
}

void User::PrintInfoVector(std::vector<User*>& userVector)
{
	if (userVector.empty()) return;

	PrintTopRow();
	int count = 1;
	for (auto& it : userVector)
	{
		it->PrintInfo(count++);
	}
}


void User::PrintTopRow()
{
	std::vector<std::string> row;
	row.reserve(3);
	row.emplace_back("Порядковый номер");
	row.emplace_back("Логин");
	row.emplace_back("Доступ");
	ClFomrat::PrintRow(row, CELL_WIDTH, ClFomrat::Border::NoBorder, ClFomrat::Border::Bottom, ClFomrat::Border::NoBorder);
}



void User::UserErase(const int index)
{
	if (index > vector.size() - 1) throw std::invalid_argument("Попытка выхода за пределы массива");

	vector.erase(vector.begin() + index);
}

User* User::GetUserPtr(const int index)
{
	if (index > vector.size() - 1) throw std::invalid_argument("Выход за пределы массива пользователей");
	return &vector[index];
}

User* User::GetUserPTr(const std::string& login)
{
	for (auto& it : vector)
		if (it.login == login)
			return &it;
	throw std::exception("Не существует связи между пользователем и клиентом");
}

std::string User::getLogin()
{
	return login;
}

bool User::BookTicket(Flight* flight, const int type, const int count)
{
	if (flight->TicketAvailable(type, count) == false)
		return false;

	try
	{
		for (int i = 0; i < count; i++)
		{
			std::string ticketId = flight->GenerateTicketID(type);
			flight->TakeTicket(type);
			flight->AddPasanger(login, type);
			tickets.emplace_back(std::move(ticketId));
		}
		return true;
	}
	catch (std::exception& exc)
	{
		std::cout << exc.what() << '\n';
		exit(-1);
	}
}

void User::ShowTickets()
{
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

	if (!ticketsInfo.empty())
	{
		Ticket::PrintTopRow();
		for (auto& it : ticketsInfo)
		{
			it.PrintInfo();
		}
	}
}

size_t User::getTicketCount()
{
	return tickets.size();
}

void User::ChangePassword(std::string& newPassword)
{
	std::string salt = RNG::randomNumber();
	std::string hash = RNG::hash(newPassword, salt);
	this->salt = salt;
	this->hash = hash;
}


AccessLevel User::getAccessLevel()
{
	return access;
}

void User::setLogin(std::string login)
{
	this->login = login;
}

void User::setAccessLevel(AccessLevel access)
{
	this->access = access;
}

int User::GetFileStatus()
{
	return File::GetFileStatus(PATH_FILE_USERS, vector);
}

bool User::CreateNewUser(const AccessLevel access)
{
	User temp;
	temp.InputInfo(access);
	if (loginExist(temp.login) > -1)
		return false;
	else
		vector.emplace_back(std::move(temp));
	return true;
}

size_t User::getUserIndex(std::string& login)
{
	for (size_t i = 0; i < vector.size(); i++)
	{
		if (vector[i].login == login)
			return i;
	}
	return -1;
}

void User::RemoveUser(const int userIndex)
{
	if (userIndex >= vector.size()) throw std::exception("Попытка выхода за пределы массива пользователей");
	if (userIndex >= 0)
		vector.erase(vector.begin() + userIndex);
}

bool User::CheckForSuperAdmin()
{
	for (auto& it : vector)
		if (it.access == AccessLevel::SuperAdmin)
			return true;
	return false;
}

AccessLevel stringToEnum(std::string& s, const std::string*& enumName, const int size)
{
	for (int i = 0; i < size; i++)
	{
		if (s == enumName[i])
			return static_cast<AccessLevel>(i);
	}
	throw std::exception("Undefined enum variable name");
}

std::fstream& operator << (std::fstream& fs, User& user)
{
	fs << user.login << ' ';
	fs << user.hash << ' ';
	fs << user.salt << ' ';
	fs << (int)user.access << '\n';
	long long limiter = user.tickets.size() - 1;
	if (limiter > 0)
	{
		for (long long i = 0; i < limiter; i++)
		{
			fs << user.tickets[i] << '\n';
		}
	}
	if (limiter > -1)
		fs << user.tickets[limiter] << '\n';
	fs << '#';
	return fs;
}

std::fstream& operator >> (std::fstream& fs, User& user)
{
	fs >> user.login;
	fs >> user.hash;
	fs >> user.salt;
	int temp;
	fs >> temp;
	user.access = static_cast<AccessLevel>(temp);
	std::string str;
	fs >> str;
	while (str != "#")
	{
		user.tickets.emplace_back(str);
		fs >> str;
	}
	return fs;
}

void User::AcceptAll()
{
	for (auto& it : vector)
		if (it.access == AccessLevel::NoAccess)
			it.access = AccessLevel::Client;
}
