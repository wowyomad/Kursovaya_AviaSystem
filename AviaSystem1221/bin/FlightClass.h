#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "FileClass.hpp"
#include "UserClass.h"
#include "common.h"

enum InfoMode
{
	Default,
	UserMode,
	AdminMode
};

enum TicketType
{
	Economy,
	Business
};

class Ticket
{
	struct Location
	{
		std::string departure;
		std::string arrival;
	};

	struct Time
	{
		tm tmArrival = { 1,1,1,1,1,1,1,1 };
		tm tmDeparture = { 1,1,1,1,1,1,1,1 };
		tm dtDeprature = { 1,1,1,1,1,1,1,1 };
	};
private:
	int type;
protected:
	std::string id;
	Time time;
	Location loc;
	int space;

public:

	Ticket();
	Ticket(int type);
	Ticket(const int type,
		const char* id,
		const Time& time,
		const Location& loc,
		const int space)
		: type(type), id(id), time(time), loc(loc), space(space) {};
	Ticket(const Ticket& source);
	Ticket(const Ticket& source, const std::string& fullTicketId);
	Ticket(Ticket&& source) noexcept;

	void setId(const std::string& id);
	std::string getId();
	void setLocDeparture(const std::string& location);
	void setLocArrival(const std::string& location);
	void setTimeDeparture(tm& time);
	void setDateDepatrue(tm& time);
	void setTimeArrival(tm& time);

	void setType(const int type);
	int getType();


	virtual void PrintInfo(const int mode = InfoMode::Default, const int& count = 0);
	virtual void PrintInfoWithTop(const int mode = InfoMode::Default);
	static void PrintTopRow(const int mode = InfoMode::Default);

	Ticket& operator =(const Ticket& other);
	Ticket& operator = (Ticket&& other);
};

class Flight : public Ticket
{
	struct Ticket_Container
	{
		int price = 0;
		int count = 0;
	};

private:
	Ticket_Container ticketBusiness;
	Ticket_Container ticketEconomy;

	static std::vector<Flight> vectorFligths;

public:
	Flight();

	Flight(const Flight& source);
	Flight(Flight&& source);

	void setPriceBusiness(const int price);
	void setPriceEconomy(const int price);
	void setCountBusiness(const int count);
	void setCountEconomy(const int count);

	std::string GenerateTicketID(const int type);
	void TakeTicket(const int type);
	bool TicketAvailable(const int ype);

	void InputInfo();
	void PushToVector();
	static void CreateFile();
	static int GetFileStatus();
	static bool WriteToFile();
	static bool ReadFile();
	static void VectorReserve(const size_t size = VECTOR_BUFF);
	static Ticket LookUpForFlight(std::string id);
	bool ValidateInfo();

	int getType() = delete;

	friend std::fstream& operator << (std::fstream& fs, const Flight& flight);
	friend std::fstream& operator >> (std::fstream& fs, Flight& flight);

	void PrintInfo(const int mode = InfoMode::Default, const int& count = 1);
	void PrintInfoWithTop(const int mode = InfoMode::Default);
	static void PrintInfoWhole(const int mode = InfoMode::Default);
	static void PrintTopRow(const int mode = InfoMode::Default);
};

