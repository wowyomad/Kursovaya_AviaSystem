#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <locale>
#include <vector>

#include "FileClass.hpp"

class Ticket
{
protected:
	std::string id;
	std::string locDeparture;
	std::string locArrival;
	tm timeArrival;
	tm timeDeparture;
	int space;
};

class Flight : public Ticket
{
	int priceBsn;
	int priceEcn;
	int countBsn;
	int countEcn;

	static std::fstream Base_Flights;
	static std::vector<Flight> flightVector;

public:
	void ReadFileToVector();
	static void WriteVectorToFile();
	static int getFileInfo(size_t& fileSize);

	void setId(const std::string& id);
	void setLocDeparture(const std::string& location);
	void setLocArrival(const std::string& location);
	void setTimeDeparture(tm& time);
	void setDateDepatrue(tm& time);
	void setDateArrival(tm& time);
	void setPriceBusiness(const int price);
	void setPriceEconomy(const int price);
	void setCountBusiness(const int count);
	void setCountEconomy(const int count);
	


	void InputInfo();

	void PrintFlightInfo();
	void PrintFlightInfo_withTop();
	static void PrintTopRaw();
	static void PrintFlightInfo_vector();

	friend std::fstream& operator << (std::fstream& fs, Flight& flight);
	friend std::fstream& operator >> (std::fstream& fs, Flight& flight);
};