#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <locale>
#include <vector>
#include "FileClass.hpp"
#include "Table.h"
#include "UserClass.h"

namespace AviaLines
{
	enum InfoMode
	{
		Default,
		User,
		Admin
	};

	enum TicketType
	{
		Economy,
		Business
	};
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

	struct Ticket_Container
	{
		int price = 0;
		int count = 0;
	};

	class Ticket : public Table::TableInterface
	{
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
		Ticket(Ticket&& source);

		void setId(const std::string& id);
		void setLocDeparture(const std::string& location);
		void setLocArrival(const std::string& location);
		void setTimeDeparture(tm& time);
		void setDateDepatrue(tm& time);
		void setTimeArrival(tm& time);

		//Методы из интерфейса TableInterface
		void PrintInfo(const int mode = InfoMode::Default, const int& count = 0) override;
		void PrintInfoWithTop(const int mode = InfoMode::Default) override;
		void PrintTopRow(const int mode = InfoMode::Default) override;
		void PrintInfoWhole(const int mode = InfoMode::Default) override {};

		Ticket& operator =(const Ticket& other);

		friend class Client;

	};

	class Flight : public Ticket, public Table::TableInterface
	{
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

		Ticket GenerateTicket(const int type);
		bool TakeTicketBusiness();
		bool TakeTicketEconomy();

		void InputInfo();
		void PushToVector();
		static int GetFileStatus();
		static bool WriteToFile();
		static bool ReadFile();

		bool ValidateInfo();

		friend std::fstream& operator << (std::fstream& fs,const Flight& flight);
		friend std::fstream& operator >> (std::fstream& fs,Flight& flight);

		//Методы из интерфейса TableInterface
		void PrintInfo(const int mode = InfoMode::Default, const int& count = 1) override;
		void PrintInfoWithTop(const int mode = InfoMode::Default) override;
		void PrintInfoWhole(const int mode = InfoMode::Default) override;
		void PrintTopRow(const int mode = InfoMode::Default) override;
	};

	void GenerateId(const std::string& leftPart, const int rightPart);
}