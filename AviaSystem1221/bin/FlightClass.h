#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <locale>
#include <vector>
#include "FileClass.hpp"

namespace AviaLines
{
	enum InfoMode
	{
		Default,
		User,
		Admin
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

	class Ticket
	{
	protected:
		std::string id;
		Time time;
		Location loc;
		int space;

	public:
		Ticket();
		Ticket(const char* id, const Time& time, const Location& loc, const int space)
			: id(id), time(time), loc(loc), space(space) {};
		Ticket(const Ticket& source);
		Ticket(Ticket&& source);

	};

	class Flight : public Ticket
	{
		Ticket_Container ticketBusiness;
		Ticket_Container ticketEconomy;

		static std::fstream fileFlights;
		static std::vector<Flight> vectorFligths;

	public:
		Flight();

		Flight(const Flight& source);
		Flight(Flight&& source);
		 
		void setId(const std::string& id);
		void setLocDeparture(const std::string& location);
		void setLocArrival(const std::string& location);
		void setTimeDeparture(tm& time);
		void setDateDepatrue(tm& time);
		void setTimeArrival(tm& time);
		void setPriceBusiness(const int price);
		void setPriceEconomy(const int price);
		void setCountBusiness(const int count);
		void setCountEconomy(const int count);

		void InputInfo();

		void PrintFlightInfo(const int mode = InfoMode::Default, const int& count = 1);
		void PrintFlightInfo_withTop(const int mode = InfoMode::Default);
		static void PrintFlightInfoWhole(const int mode = InfoMode::Default);
		static void PrintTopRaw(const int mode = InfoMode::Default);

		void PushToVector();
		static bool SaveToFile();
		static bool ReadFromFile();

		bool ValidateInfo();

		friend std::fstream& operator << (std::fstream& fs, AviaLines::Flight& flight);
		friend std::fstream& operator >> (std::fstream& fs, AviaLines::Flight& flight);
	};

	void GenerateId(const std::string& leftPart, const int rightPart);
}