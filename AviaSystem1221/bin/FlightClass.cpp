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
static const int FLIGHT_ID_LENGTH = 8;

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

std::vector<Flight> Flight::vectorFligths;

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
	*this = source;
}

Ticket::Ticket(const Ticket& source, const std::string& FullTicketId)
{
	*this = source;
	this->id = FullTicketId;
}

Ticket::Ticket(Ticket&& source) noexcept
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
	row.reserve(CELLS);
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
		ticketType = DEFAULT_STR;
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
	FormattedOutput::PrintCenteredRow(row, CELL_WIDTH, FormattedOutput::Border::Both, FormattedOutput::Border::Bottom, FormattedOutput::Border::Both);
}

void Ticket::PrintInfoWithTop(const int mode)
{
	PrintTopRow(mode);
	PrintInfo(mode);
}

void Ticket::PrintTopRow(const int mode)
{
	std::vector<std::string> topRow;
	topRow.reserve(CELLS);
	topRow.insert(topRow.end(), {
		ticket_col0,
		ticket_col1,
		ticket_col2,
		ticket_col3,
		ticket_col4,
		ticket_col5,
		ticket_col6
		});
	FormattedOutput::PrintCenteredRow(topRow, CELL_WIDTH, FormattedOutput::Border::NoBorder, FormattedOutput::Border::Bottom, FormattedOutput::Border::NoBorder);

}

Ticket& Ticket::operator=(const Ticket& other)
{
	type = other.type;
	id = other.id;
	time = other.time;
	loc = other.loc;
	space = other.space;
	return *this;
}

Ticket& Ticket::operator=(Ticket&& other)
{
	type = other.type;
	id = std::move(other.id);
	time = other.time;
	loc.departure = std::move(other.loc.departure);
	loc.arrival = std::move(other.loc.arrival);
	space = other.space;
	return *this;
}

Flight::Flight() : Ticket()
{
	ticketBusiness = { 1,1 };
	ticketEconomy = { 1,1 };
}

Flight::Flight(const Flight& source) : Ticket(source)
{
	ticketBusiness = source.ticketBusiness;
	ticketEconomy = source.ticketEconomy;
}

Flight::Flight(Flight&& source) : Ticket(std::move(source))
{
	std::cout << "Moved\n";
	ticketBusiness = std::move(source.ticketBusiness);
	ticketEconomy = std::move(source.ticketEconomy);
}


void Ticket::setId(const std::string& id)
{
	this->id = id;
}

std::string Ticket::getId()
{
	return id;
}

void Ticket::setLocDeparture(const std::string& location)
{
	loc.departure = location;
}

void Ticket::setLocArrival(const std::string& location)
{
	loc.arrival = location;
}

void Ticket::setDateDepatrue(tm& time)
{
	this->time.dtDeprature.tm_year = time.tm_year;
	this->time.dtDeprature.tm_mon = time.tm_mon;
	this->time.dtDeprature.tm_mday = time.tm_mday;
}

void Ticket::setTimeDeparture(tm& time)
{
	this->time.tmDeparture.tm_hour = time.tm_hour;
	this->time.tmDeparture.tm_min = time.tm_min;
}

void Ticket::setTimeArrival(tm& time)
{
	this->time.tmArrival.tm_hour = time.tm_hour;
	this->time.tmArrival.tm_min = time.tm_min;
}

void Ticket::setType(const int type)
{
	if (type != TicketType::Business && type != TicketType::Economy)
		return;
	this->type = type;
}

int Ticket::getType()
{
	return type;
}

void Flight::setPriceBusiness(const int price)
{
	ticketBusiness.price = price;
}

void Flight::setPriceEconomy(const int price)
{
	ticketEconomy.price = price;
}

void Flight::setCountBusiness(const int count)
{
	ticketBusiness.count = count;
}

void Flight::setCountEconomy(const int count)
{
	ticketEconomy.count = count;
}

std::string Flight::GenerateTicketID(const int type)
{
	std::string ticketId;
	std::stringstream ss;
	ss << id + '_' << std::setw(4) << std::setfill('0')
		<< std::to_string(ticketBusiness.count + ticketEconomy.count)
		<< '_' + std::to_string(type);
	ss >> ticketId;
	std::cout << id << '\n';
	std::cout << ticketId << '\n';
	return ticketId;
}

void Flight::TakeTicket(const int type)
{
	if (type == TicketType::Business)
		ticketBusiness.count--;
	else if (type == TicketType::Economy)
		ticketEconomy.count--;
}

bool Flight::TicketAvailable(const int type)
{
	if (type == TicketType::Business)
		return ticketBusiness.count;
	else if (type == TicketType::Economy)
		return ticketEconomy.count;
	else return false;
}

void Flight::PushToVector()
{
	vectorFligths.emplace_back(*this);
}

void Flight::CreateFile()
{
	std::fstream file(PATH_FILE_FLIGHTS, std::ios::out);
	file.close();
}

bool Flight::ValidateInfo()
{
	return true;
}

void Flight::PrintInfo(const int mode, const int& count)
{
	std::vector <std::string> row;
	row.reserve(CELLS);
	if (mode == InfoMode::UserMode || mode == InfoMode::AdminMode)
		row.emplace_back(std::to_string(count));
	row.insert(row.end(), {
		id,
		loc.departure,
		loc.arrival,
		getDateString(time.dtDeprature),
		getTimeString(time.tmDeparture),
		getTimeString(time.tmArrival) });
	if (mode == InfoMode::AdminMode)
		row.insert(row.end(), {
		std::to_string(ticketEconomy.count),
		std::to_string(ticketBusiness.count) });
	FormattedOutput::PrintCenteredRow(row, CELL_WIDTH, FormattedOutput::Border::Both, FormattedOutput::Border::Bottom, FormattedOutput::Border::Both);
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
	if (mode == UserMode || mode == AdminMode)
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
	topRow.reserve(CELLS);
	if (mode == UserMode || mode == AdminMode)
		topRow.emplace_back(flight_col0);
	topRow.insert(topRow.end(), {
		flight_col1,
		flight_col2,
		flight_col3,
		flight_col4,
		flight_col5,
		flight_col6 });
	if (mode == AdminMode)
		topRow.insert(topRow.end(), { flight_col7, flight_col8 });
	FormattedOutput::PrintCenteredRow(topRow, CELL_WIDTH, FormattedOutput::Border::NoBorder, FormattedOutput::Border::Bottom, FormattedOutput::Border::NoBorder);
}

int Flight::GetFileStatus()
{
	return File::GetFileStatus(PATH_FILE_FLIGHTS, vectorFligths);
}

bool Flight::ReadFile()
{
	VectorReserve();
	if (GetFileStatus() == FileStatus::Opened)
		return File::ReadFile(PATH_FILE_FLIGHTS, vectorFligths);
	else return false;
}

void Flight::VectorReserve(const size_t size)
{
	vectorFligths.reserve(size);
}

Ticket Flight::LookUpForFlight(std::string fullTicketId)
{
	std::string flightId = fullTicketId.substr(0, fullTicketId.find('_'));
	for (auto& it : vectorFligths)
	{
		if (it.id == flightId)
		{
			return Ticket(it, fullTicketId);
		}
	}
	return Ticket(-1);
}

bool Flight::WriteToFile()
{
	return File::WriteToFile(PATH_FILE_FLIGHTS, vectorFligths);
}

void Flight::InputInfo()
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
		if (ticketEconomy.count != 0)
			InputVar(ticketBusiness.price, MIN_PRICE, MAX_PRICE, "Цена билета бизнес-класса: ");
		InputVar(ticketBusiness.price, MIN_PRICE, MAX_PRICE, "Цена билета эконом-класса: ");
	}
	else std::cout << "Места для посажиров бизнес-класса не осталось\n";


	id = std::to_string(std::time(NULL));
	std::cout << id << '\n';
	id = id.substr(id.length() - FLIGHT_ID_LENGTH);
	std::cout << id << '\n';
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}
std::fstream& operator<<(std::fstream& fs, const Flight& flight)
{
	fs << flight.id << ' ';

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
	fs >> flight.id;

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