#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "FileClass.hpp"
#include "UserClass.h"
#include "common.h"

enum TicketType
{
	Economy = 1,
	Business = 2
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
	Ticket(const int type);
	Ticket(const int type,
		const char* id,
		const Time& time,
		const Location& loc,
		const int space)
		: type(type), id(id), time(time), loc(loc), space(space) {};
	Ticket(const Ticket& source);
	Ticket(const Ticket& source, const std::string& fullTicketId);
	Ticket(Ticket&& source) noexcept;

	void setLocDeparture(const std::string& location);
	void setLocArrival(const std::string& location);
	void setTimeDeparture(tm& time);
	void setDateDepatrue(tm& time);
	void setTimeArrival(tm& time);

	void setId(const std::string& id);
	std::string getId() const;
	void setType(const int type);
	int getType() const;

	tm getDateDep() const;

	virtual void PrintInfo(const int& count = 0) const;
	virtual void PrintInfoWithTop() const;
	static void PrintTopRow();

	Ticket& operator =(const Ticket& other);
	Ticket& operator = (Ticket&& other) noexcept;
};

class Flight : public Ticket
{
	struct TicketInfo
	{
		int price = 0;
		int count = 0;
	};

private:
	TicketInfo ticketBusiness;
	TicketInfo ticketEconomy;

	static std::vector<Flight> vector;
	std::vector<std::string> passangers;

public:
	Flight();
	Flight(const Flight& source);
	Flight(Flight&& source) noexcept;

	int getType() = delete;

	static void RemoveFlight(const size_t index);

	void setPriceBusiness(const int price);
	void setPriceEconomy(const int price);
	void setCountBusiness(const int count);
	void setCountEconomy(const int count);

	int getTicketPrice(const int type);
	int getTicketCount(const int type);
	size_t getPassangerCount();

	static std::string GenerateTicketID(const int index, const int type);
	std::string GenerateTicketID(const int type) const;

	void TakeTicket(const int type);
	void AddPasanger(std::string& login, const int type);

	bool TicketAvailable(const int type, const int count) const;

	void InputInfo();

	void PushToVector();
	static int getVectorSize();
	static void CreateNewFile();
	static int GetFileStatus();
	static bool WriteToFile();
	static bool ReadFile();

	static void VectorReserve(const size_t size = VECTOR_BUFF);
	static Ticket GetFlightTicket(const std::string& fullTicketID);
	static Flight* getFlightPtr(const int index);
	static int GetFlightIndex(std::string& id);
	static int GetFlightIndex(const Flight* flightPtr);
	static size_t getFlightCount();

	friend std::fstream& operator << (std::fstream& fs, const Flight& flight);
	friend std::fstream& operator >> (std::fstream& fs, Flight& flight);
	Flight& operator = (const Flight& flight);
	Flight& operator = (Flight&& flight) noexcept;

	void PrintInfo(const int& count = 1) const override;
	void PrintInfoWithTop() const override;
	static void PrintInfoVector();
	static void PrintInfoVector(std::vector<Flight*>& flightVector);
	static void PrintTopRow();

	void PrintPassangers() const;

	static void CopyVector(std::vector<Flight>& destination);
	static void CopyVectorPtr(std::vector<Flight*>& destination);
	static bool Sort(const int type);
	static std::vector<Flight> Search(const int type, const std::string& input);

	static void SortById(std::vector<Flight*>& flightVector);
	static void SortByDate(std::vector<Flight*>& flightVector);
	static void SortByDep(std::vector<Flight*>& flightVector);
	static void SortByArr(std::vector<Flight*>& flightVector);

	static void SearchById(const std::vector<Flight*>& soruce, std::vector<Flight*>& destination, const std::string& input);
	static void SearchByDep(const std::vector<Flight*>& soruce, std::vector<Flight*>& destination, const std::string& input);
	static void SearchByArr(const std::vector<Flight*>& soruce, std::vector<Flight*>& destination, const std::string& input);

private:
	static void SortById();
	static void SortByDate();
	static void SortByDep();
	static void SortByArr();

	static void SearchById(std::vector<Flight>& vec, const std::string& input);
	static void SearchByDep(std::vector<Flight>& vec, const std::string& input);
	static void SearchByArr(std::vector<Flight>& vec, const std::string& input);
};
