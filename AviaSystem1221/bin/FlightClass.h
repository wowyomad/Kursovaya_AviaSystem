#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <locale>
#include <vector>

#include "FileClass.h"

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

class Flight : public Ticket, public BaseFile
{
	int priceBsn;
	int priceEcn;
	int countBsn;
	int countEcn;

	static std::ifstream FlightsFileIn;
	static std::ofstream FlightsFileOut;
	static std::fstream FlightsFile;
	static std::vector<Flight> fligthsVector;

public:

	void ReadFileToVector() override;
	void WriteVectorToFile() override;

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