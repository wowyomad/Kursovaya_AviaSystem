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
static const int CELL_WIDTH = 18;

extern const char PATH_FILE_FLIGHTS[] = "Files\\FlightsBase.dat";
static const char TICKET_TYPE_BUSINESS[] = "Бизнесс-класс";
static const char TICKET_TYPE_ECONOMY[] = "Эконом-класс";
static const char DEFAULT_STR[] = "default";

static const char ticket_col0[] = "Номер билета";
static const char ticket_col1[] = "Класс";
static const char ticket_col2[] = "Место вылета";
static const char ticket_col3[] = "Место прибытия";
static const char ticket_col4[] = "Дата вылета";
static const char ticket_col5[] = "Время вылета";
static const char ticket_col6[] = "Время прибытия";

static const char flight_col0[] = "Порядковый номер";
static const char flight_col1[] = "Номер рейса";
static const char* flight_col2 = ticket_col2;
static const char* flight_col3 = ticket_col3;
static const char* flight_col4 = ticket_col4;
static const char* flight_col5 = ticket_col5;
static const char* flight_col6 = ticket_col6;
static const char flight_col7[] = "Эконом, билеты";
static const char flight_col8[] = "Бизнес, билеты";

namespace AviaLines
{


	std::vector<AviaLines::Flight> AviaLines::Flight::vectorFligths;

	Ticket::Ticket()
	{
		type = 0;
		id = DEFAULT_STR;
		loc = { DEFAULT_STR, DEFAULT_STR };
	}

	Ticket::Ticket(int type)
	{
		this->type = type;
		id = DEFAULT_STR;
		loc = { DEFAULT_STR, DEFAULT_STR };
	}

	Ticket::Ticket(const Ticket& source)
	{
		type = source.type;
		id = source.id;
		time = source.time;
		loc = source.loc;
		space = source.space;
	}

	Ticket::Ticket(Ticket&& source)
	{
		type = source.type;
		time = source.time;
		space = source.space;
		id = std::move(source.id);
		loc = std::move(source.loc);
	}

	void Ticket::PrintInfo(const int mode, const int& count)
	{
		std::vector <std::string> row;
		row.reserve(FIELDS);
		std::string ticketType;
		switch (type)
		{
		case TicketType::Economy:
			ticketType = TICKET_TYPE_ECONOMY;
			break;
		case TicketType::Business:
			ticketType = TICKET_TYPE_BUSINESS;
			break;
		default:
			ticketType = "";
			break;
		}
		row.insert(row.end(), {
			id,
			ticketType,
			loc.departure,
			loc.arrival,
			getDateString(time.dtDeprature),
			getTimeString(time.tmDeparture),
			getTimeString(time.tmArrival)
			});
		Table::PrintRow(row, CELL_WIDTH, Table::Border::Both, Table::Border::Bottom, Table::Border::Both);
	}

	void Ticket::PrintInfoWithTop(const int mode)
	{
		PrintTopRow(mode);
		PrintInfo(mode);
	}

	void Ticket::PrintTopRow(const int mode)
	{
		std::vector<std::string> topRow;
		topRow.reserve(FIELDS);
		topRow.insert(topRow.end(), {
			ticket_col0,
			ticket_col1,
			ticket_col2,
			ticket_col3,
			ticket_col4,
			ticket_col5,
			ticket_col6
			});
		Table::PrintRow(topRow, CELL_WIDTH, Table::Border::NoBorder, Table::Border::Bottom, Table::Border::NoBorder);

	}

	Ticket& Ticket::operator=(const Ticket& other)
	{
		id = other.id;
		type = other.type;
		loc = other.loc;
		space = other.space;
		return *this;
	}

	Flight::Flight() : Ticket()
	{
		std::cout << "Constructed\n";
		ticketBusiness = { 0 };
		ticketEconomy = { 0 };
	}

	Flight::Flight(const Flight& source) : Ticket(source)
	{
		std::cout << "Copied\n";
		ticketBusiness = source.ticketBusiness;
		ticketEconomy = source.ticketEconomy;
	}

	Flight::Flight(Flight&& source) : Ticket(std::move(source))
	{
		std::cout << "Moved\n";
		ticketBusiness = std::move(source.ticketBusiness);
		ticketEconomy = std::move(source.ticketEconomy);
	}


	void AviaLines::Ticket::setId(const std::string& id)
	{
		this->id = id;
	}

	void AviaLines::Ticket::setLocDeparture(const std::string& location)
	{
		loc.departure = location;
	}

	void AviaLines::Ticket::setLocArrival(const std::string& location)
	{
		loc.arrival = location;
	}

	void AviaLines::Ticket::setDateDepatrue(tm& time)
	{
		this->time.dtDeprature.tm_year = time.tm_year;
		this->time.dtDeprature.tm_mon = time.tm_mon;
		this->time.dtDeprature.tm_mday = time.tm_mday;
	}

	void AviaLines::Ticket::setTimeDeparture(tm& time)
	{
		this->time.tmDeparture.tm_hour = time.tm_hour;
		this->time.tmDeparture.tm_min = time.tm_min;
	}

	void AviaLines::Ticket::setTimeArrival(tm& time)
	{
		this->time.tmArrival.tm_hour = time.tm_hour;
		this->time.tmArrival.tm_min = time.tm_min;
	}

	void AviaLines::Flight::setPriceBusiness(const int price)
	{
		ticketBusiness.price = price;
	}

	void AviaLines::Flight::setPriceEconomy(const int price)
	{
		ticketEconomy.price = price;
	}

	void AviaLines::Flight::setCountBusiness(const int count)
	{
		ticketBusiness.count = count;
	}

	void AviaLines::Flight::setCountEconomy(const int count)
	{
		ticketEconomy.count = count;
	}

	Ticket Flight::GenerateTicket(const int type)
	{
		if (type == TicketType::Economy || type == TicketType::Business)
		{
			Ticket ticket(type);
			std::string ticketId = id + ' ' + std::to_string(ticketBusiness.count + ticketEconomy.count);
			ticket.setId(ticketId);
			ticket.setLocDeparture(loc.departure);
			ticket.setLocArrival(loc.arrival);
			ticket.setDateDepatrue(time.dtDeprature);
			ticket.setTimeDeparture(time.tmDeparture);
			ticket.setTimeArrival(time.tmArrival);
			return ticket;
		}
		else return Ticket(-1);
	}

	bool Flight::TakeTicketBusiness()
	{
		if (ticketBusiness.count > 0)
		{
			ticketBusiness.count--;
			return true;
		}
		else return false;
	}

	bool Flight::TakeTicketEconomy()
	{
		if (ticketEconomy.count > 0)
		{
			ticketBusiness.count--;
			return true;
		}
		else return false;
	}

	void Flight::PushToVector()
	{
		vectorFligths.emplace_back(*this);
	}

	bool Flight::ValidateInfo()
	{
		return true;
	}

	void Flight::PrintInfo(const int mode, const int& count)
	{
		std::vector <std::string> row;
		row.reserve(FIELDS);
		if (mode == User || mode == Admin)
			row.emplace_back(std::to_string(count));
		row.insert(row.end(), {
			id,
			loc.departure,
			loc.arrival,
			getDateString(time.dtDeprature),
			getTimeString(time.tmDeparture),
			getTimeString(time.tmArrival) });
		if (mode == Admin)
			row.insert(row.end(), {
			std::to_string(ticketEconomy.count),
			std::to_string(ticketBusiness.count) });
		Table::PrintRow(row, CELL_WIDTH, Table::Border::Both, Table::Border::Bottom, Table::Border::Both);
	}

	void Flight::PrintInfoWithTop(const int mode)
	{
		PrintTopRow(mode);
		PrintInfo(mode);
	}

	void Flight::PrintInfoWhole(const int mode)
	{
		if (vectorFligths.size() < 1) return;

		PrintTopRow(mode);
		if (mode == User || mode == Admin)
		{
			int count = 1;
			for (auto& it : vectorFligths)
				it.PrintInfo(mode, count++);
		}
		else for (auto& it : vectorFligths)
			it.PrintInfo(mode);
	}

	void Flight::PrintTopRow(const int mode)
	{
		std::vector<std::string> topRow;
		topRow.reserve(FIELDS);
		if (mode == User || mode == Admin)
			topRow.emplace_back(flight_col0);
		topRow.insert(topRow.end(), {
			flight_col1,
			flight_col2,
			flight_col3,
			flight_col4,
			flight_col5,
			flight_col6 });
		if (mode == Admin)
			topRow.insert(topRow.end(), { flight_col7, flight_col8 });
		Table::PrintRow(topRow, CELL_WIDTH, Table::Border::NoBorder, Table::Border::Bottom, Table::Border::NoBorder);
	}

	int Flight::GetFileStatus()
	{
		return File::GetFileStatus(PATH_FILE_FLIGHTS, vectorFligths);
	}

	bool Flight::ReadFile()
	{
		return File::ReadFile(PATH_FILE_FLIGHTS, vectorFligths);
	}

	bool Flight::WriteToFile()
	{
		return File::WriteToFile(PATH_FILE_FLIGHTS, vectorFligths);
	}

	void AviaLines::Flight::InputInfo()
	{
		do
		{
			InputString(loc.departure, "Место вылета: ");
			InputString(loc.arrival, "Место прибытия: ");
			if (loc.departure == loc.arrival)
				std::cout << "Место вылето и место прибытия идентичны.\n";
			else break;
		} while (true);

		InputDate(time.dtDeprature, "Дата вылета");
		do
		{
			InputTime(time.tmDeparture, "Время вылета");
			InputTime(time.tmArrival, "Время прибытия");
			if (getTimeString(time.tmDeparture) == getTimeString(time.tmArrival))
				std::cout << "Время вылета и время прибытия идентичны.\n";
			else break;
		} while (true);

		InputVar(space, MIN_SPACE, MAX_SPACE, "Вместимость самолета: ");
		InputVar(ticketEconomy.count, 0, space, "Количество билетов эконом-класса: ");
		if (ticketEconomy.count != space)
		{
			InputVar(ticketBusiness.count, 0, space - ticketEconomy.count, "Количество билетов бизнесс-класса: ");
			InputVar(ticketBusiness.price, MIN_PRICE, MAX_PRICE, "Цена билета бизнес-класса: ");
		}
		else std::cout << "Места для посажиров бизнес-класса не осталось\n";
		InputVar(ticketEconomy.price, MIN_PRICE, MAX_PRICE, "Цена билета эконом-класса: ");

		id = std::to_string(std::time(NULL));
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}

	std::fstream& operator<<(std::fstream& fs,const Flight& flight)
	{
		fs << flight.id << '#';
		fs << flight.loc.departure << '#';
		fs << flight.loc.arrival << '#';

		fs << flight.time.dtDeprature.tm_year << ' ';
		fs << flight.time.dtDeprature.tm_mon << ' ';
		fs << flight.time.dtDeprature.tm_mday << ' ';

		fs << flight.time.tmDeparture.tm_hour << ' ';
		fs << flight.time.tmDeparture.tm_min << ' ';

		fs << flight.time.tmArrival.tm_hour << ' ';
		fs << flight.time.tmArrival.tm_min << ' ';

		fs << flight.ticketBusiness.price << ' ';
		fs << flight.ticketEconomy.price << ' ';
		fs << flight.ticketBusiness.count << ' ';
		fs << flight.ticketEconomy.count;
		return fs;
	}

	std::fstream& operator>>(std::fstream& fs, Flight& flight)
	{
		std::getline(fs, flight.id, '#');
		std::getline(fs, flight.loc.departure, '#');
		std::getline(fs, flight.loc.arrival, '#');

		fs >> flight.time.dtDeprature.tm_year;
		fs >> flight.time.dtDeprature.tm_mon;
		fs >> flight.time.dtDeprature.tm_mday;

		fs >> flight.time.tmDeparture.tm_hour;
		fs >> flight.time.tmDeparture.tm_min;

		fs >> flight.time.tmArrival.tm_hour;
		fs >> flight.time.tmArrival.tm_min;

		fs >> flight.ticketBusiness.price;
		fs >> flight.ticketEconomy.price;
		fs >> flight.ticketBusiness.count;
		fs >> flight.ticketEconomy.count;
		return fs;
	}
}