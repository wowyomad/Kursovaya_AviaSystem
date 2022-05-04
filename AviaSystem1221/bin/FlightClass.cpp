#include "FlightClass.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include "Table.h"
#include "UserConsoleInput.h"

static const int MIN_PRICE = 1;
static const int MAX_PRICE = INT_MAX;
static const int MIN_SPACE = 1;
static const int MAX_SPACE = 1000;

static const int FIELDS = 10;

static const char PATH_BASE_FLIGHTS[] = "FlightsBase.dat";

static const char ticket_col1[] = "Номер билета";
static const char ticket_col2[] = "Место вылета";
static const char ticket_col3[] = "Место прибытия";
static const char ticket_col4[] = "Дата вылета";
static const char ticket_col5[] = "Время вылета";
static const char ticket_col6[] = "Время прибытия";

static const char flight_col1[] = "Номер рейса";
static const char* flight_col2 = ticket_col2;
static const char* flight_col3 = ticket_col3;
static const char* flight_col4 = ticket_col4;
static const char* flight_col5 = ticket_col5;
static const char* flight_col6 = ticket_col6;

const int CELL_WIDTH = 20;

std::vector<Flight> Flight::flightVector;
std::fstream Flight::Base_Flights;

void Flight::ReadFileToVector()
{

	if (flightVector.capacity() < VECTOR_BUFF)
		flightVector.reserve(VECTOR_BUFF);
	Base_Flights.open(PATH_BASE_FLIGHTS, std::ios::in);
	if (Base_Flights.is_open())
	{
		while(!Base_Flights.eof())
		{
			
		}
	}
}

void Flight::WriteVectorToFile()
{
	Base_Flights.open(PATH_BASE_FLIGHTS, std::ios::out);
	size_t limiter = flightVector.size() - 1;
	for (size_t i = 0; i < limiter; i++)
	{
		Base_Flights << flightVector[i] << '\n';
	}
	Base_Flights << flightVector[limiter];
}

int Flight::getFileInfo(size_t& fileSize)
{
	fileSize = 0;
	std::streampos initialPos = 0;
	bool alreadyOpened = false;
	if (Base_Flights.is_open())
		alreadyOpened = true;
	else
		Base_Flights.open(PATH_BASE_FLIGHTS, std::ios::in);

	if (Base_Flights.is_open())
	{
		initialPos = Base_Flights.tellg();
		Base_Flights.seekg(0, std::ios::end);
		Base_Flights.seekg(initialPos);
		fileSize = Base_Flights.tellg();
		if (!alreadyOpened)
			Base_Flights.close();
		if (fileSize > 0)
			return FileStatus::Opened;
		else return FileStatus::Empty;

	}
	else return FileStatus::NotOpened;


}

void Flight::setId(const std::string& id)
{
	this->id = id;
}

void Flight::setLocDeparture(const std::string& location)
{
	locDeparture = location;
}

void Flight::setLocArrival(const std::string& location)
{
	locArrival = location;
}

void Flight::setDateDepatrue(tm& time)
{
	timeDeparture.tm_year = time.tm_year;
	timeDeparture.tm_mon = time.tm_mon;
	timeDeparture.tm_mday = time.tm_mday;
}

void Flight::setTimeDeparture(tm& time)
{
	timeDeparture.tm_hour = time.tm_hour;
	timeDeparture.tm_min = time.tm_min;
}

void Flight::setDateArrival(tm& time)
{
	timeArrival.tm_year = time.tm_year;
	timeArrival.tm_mon = time.tm_mon;
	timeArrival.tm_mday = time.tm_mday;
}

void Flight::setPriceBusiness(const int price)
{
	priceBsn = price;
}

void Flight::setPriceEconomy(const int price)
{
	priceEcn = price;
}

void Flight::setCountBusiness(const int count)
{
	countBsn = count;
}

void Flight::setCountEconomy(const int count)
{
	countEcn = count;
}

void Flight::InputInfo()
{
	do
	{
		InputString(locDeparture, "Место вылета: ");
		InputString(locArrival, "Место прибытия: ");
		if (locDeparture == locArrival)
			std::cout << "Место вылето и место прибытия идентичны.\n";
		else break;
	} while (true);

	InputDate(timeDeparture, "Дата вылета");
	do
	{
		InputTime(timeDeparture, "Время вылета");
		InputTime(timeArrival, "Время прибытия");
		if (getTimeString(timeDeparture) == getTimeString(timeArrival))
			std::cout << "Время вылета и время прибытия идентичны.\n";
		else break;
	} while (true);
	InputVar(priceEcn, MIN_PRICE, MAX_PRICE, "Цена билета эконом-класса: ");
	InputVar(priceBsn, MIN_PRICE, MAX_PRICE, "Цена билета бизнес-класса: ");
	InputVar(space, MIN_SPACE, MAX_SPACE, "Вместимость самолета: ");
	InputVar(countEcn, 0, space, "Количество билетов эконом-класса: ");
	if (countEcn != space)
		InputVar(countBsn, 0, space - countEcn, "Количество билетов бизнесс-класса: ");
	else
		std::cout << "Места для посажиров бизнес-класса не осталось\n";

	id = std::to_string(std::time(NULL));
}

void Flight::PrintFlightInfo()
{
	std::vector<std::string> row{ id, locDeparture, locArrival, getDateString(timeDeparture), getTimeString(timeDeparture), getTimeString(timeArrival) };
	Table::PrintRow(row, CELL_WIDTH, Table::Border::Both, Table::Border::Bottom, Table::Border::Both);
}

void Flight::PrintFlightInfo_withTop()
{
	PrintTopRaw();
	PrintFlightInfo();
}

void Flight::PrintFlightInfo_vector()
{
	PrintTopRaw();
	for (auto& it : flightVector)
	{
		it.PrintFlightInfo();
	}
}

void Flight::PrintTopRaw()
{
	std::vector<std::string> topRow{ flight_col1, flight_col2, flight_col3, flight_col4, flight_col5, flight_col6 };
	Table::PrintRow(topRow, CELL_WIDTH, Table::Border::NoBorder, Table::Border::Bottom, Table::Border::NoBorder);
}

std::fstream& operator<<(std::fstream& fs, Flight& flight)
{
	fs << flight.id << ' ';
	fs << flight.locDeparture << '#';
	fs << flight.locArrival << '#';

	fs << flight.timeDeparture.tm_year << ' ';
	fs << flight.timeDeparture.tm_mon << ' ';
	fs << flight.timeDeparture.tm_year << ' ';

	fs << flight.timeDeparture.tm_hour << ' ';
	fs << flight.timeDeparture.tm_min << ' ';

	fs << flight.timeArrival.tm_hour << ' ';
	fs << flight.timeArrival.tm_min << ' ';

	fs << flight.priceBsn << ' ';
	fs << flight.priceEcn << ' ';
	fs << flight.countBsn << ' ';
	fs << flight.countEcn;
	return fs;
}

std::fstream& operator>>(std::fstream& fs, Flight& flight)
{
	fs >> flight.id;
	fs >> flight.locDeparture;
	fs >> flight.locArrival;

	fs >> flight.timeDeparture.tm_year;
	fs >> flight.timeDeparture.tm_mon;
	fs >> flight.timeDeparture.tm_year;

	fs >> flight.timeDeparture.tm_hour;
	fs >> flight.timeDeparture.tm_min;

	fs >> flight.timeArrival.tm_hour;
	fs >> flight.timeArrival.tm_min;

	fs >> flight.priceBsn;
	fs >> flight.priceEcn;
	fs >> flight.countBsn;
	fs >> flight.countEcn;
	return fs;
}
