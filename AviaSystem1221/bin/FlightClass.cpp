#include "FlightClass.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <conio.h>
#include "Table.h"
#include "UserConsoleInput.h"

static const int MIN_PRICE = 1;
static const int MAX_PRICE = INT_MAX;
static const int MIN_SPACE = 1;
static const int MAX_SPACE = 1000;
static const int FLIGHT_ID_LENGTH = 8;

static const char ENTER_KEY = 13;
static const char ESC_KEY = 27;

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
static const char flight_col9[] = "Эконом, стоимость";
static const char flight_col10[] = "Бизнес, стоимость";

std::vector<Flight> Flight::vector;


Ticket::Ticket()
{
	type = 0;
	id = DEFAULT_STR;
	loc = { DEFAULT_STR, DEFAULT_STR };
}

Ticket::Ticket(const int type)
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

void Ticket::PrintInfo(const int mode, const int& count) const
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
	ClFomrat::PrintCenteredRow(row, CELL_WIDTH, ClFomrat::Border::Both, ClFomrat::Border::Bottom, ClFomrat::Border::Both);
}

void Ticket::PrintInfoWithTop(const int mode) const
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
	ClFomrat::PrintCenteredRow(topRow, CELL_WIDTH, ClFomrat::Border::NoBorder, ClFomrat::Border::Bottom, ClFomrat::Border::NoBorder);

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

Ticket& Ticket::operator=(Ticket&& other) noexcept
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
	ticketBusiness = { 0,0 };
	ticketEconomy = { 0,0 };
}

Flight::Flight(const Flight& source)
{
	*this = source;
}

Flight::Flight(Flight&& source) noexcept
{
	*this = std::move(source);
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

void Ticket::setId(const std::string& id)
{
	this->id = id;
}

std::string Ticket::getId() const
{
	return id;
}

void Ticket::setType(const int type)
{
	if (type != TicketType::Business && type != TicketType::Economy)
		return;
	this->type = type;
}

int Ticket::getType() const
{
	return type;
}

tm Ticket::getDateDep() const
{
	return time.dtDeprature;
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

int Flight::GetFlightIndex(std::string& id)
{
	int i;
	const int size = vector.size();
	for (i = 0; i < size; i++)
	{
		if (vector[i].id == id)
			return i;
	}
	return -1;

}

std::string Flight::GenerateTicketID(const int type) const
{
	std::string ticketId;
	std::stringstream ss;
	ss << id + '_' << std::setw(4) << std::setfill('0')
		<< std::to_string(ticketBusiness.count + ticketEconomy.count)
		<< '_' + std::to_string(type);
	ss >> ticketId;
	return ticketId;
}

void Flight::TakeTicket(const int type)
{
	if (type == TicketType::Business)
		ticketBusiness.count--;
	else if (type == TicketType::Economy)
		ticketEconomy.count--;
}

std::string Flight::GenerateTicketID(const int index, const int type)
{
	if (vector.size() < index + 1)
		throw std::invalid_argument("Попытка выхода за пределы массива.");

	return vector[index].GenerateTicketID(type);
}

void Flight::TakeTicket(const int index, const int type)
{
	if (vector.size() < index + 1)
		throw std::invalid_argument("Попытка выхода за пределы массива.");

	vector[index].TakeTicket(type);
}

bool Flight::TicketAvailable(const int index, const int type)
{
	if (vector.size() < index + 1)
		throw std::invalid_argument("Попытка выхода за пределы массива");

	return vector[index].TicketAvailable(type);
}

bool Flight::TicketAvailable(const int type) const
{
	if (type == TicketType::Economy)
		return ticketEconomy.count;
	else if (type == TicketType::Business)
		return ticketBusiness.count;
	else return false;
}

void Flight::PushToVector()
{
	std::stringstream ss;
	ss << std::setw(FLIGHT_ID_LENGTH) << std::setfill('0')
		<< vector.size();
	id = ss.str();
	vector.emplace_back(*this);
}

int Flight::getVectorSize()
{
	return vector.size();
}

void Flight::CreateNewFile()
{
	std::fstream file(PATH_FILE_FLIGHTS, std::ios::out);
	file.close();
}

bool Flight::ValidateInfo()
{
	return true;
}

void Flight::SortById()
{
	std::sort(vector.begin(), vector.end(), [](const Flight& l, const Flight& r) noexcept
		{
			return l.id < r.id;
		});
}

void Flight::SortByDate()
{
	std::sort(vector.begin(), vector.end(), [](const Flight& l, const Flight& r) noexcept
		{
			return l.time.dtDeprature < r.time.dtDeprature;
		});
}

void Flight::SortByDep()
{
	std::sort(vector.begin(), vector.end(), [](const Flight& l, const Flight& r) noexcept
		{
			return l.loc.departure < r.loc.departure;
		});
}

void Flight::SortByArr()
{
	std::sort(vector.begin(), vector.end(), [](const Flight& l, const Flight& r) noexcept
		{
			return l.loc.arrival < r.loc.arrival;
		});
}

void Flight::SearchById(std::vector<Flight>& vec, const std::string& input)
{
	std::string str_lower;
	std::string input_lower = input;
	for (int i = 0; i < input.size(); i++)
	{
		input_lower[i] = std::tolower(input_lower[i]);
	}
	for (auto& it : vector)
	{
		str_lower = it.id;
		for (int i = 0; i < str_lower.size(); i++)
		{
			str_lower[i] = std::tolower(str_lower[i]);
		}
		if (str_lower.find(input_lower) != std::string::npos)
		{
			vec.push_back(it);
		}
	}
}

void Flight::SearchByDep(std::vector<Flight>& vec, const std::string& input)
{
	std::string str_lower;
	std::string input_lower = input;
	for (int i = 0; i < input.size(); i++)
	{
		input_lower[i] = std::tolower(input[i]);
	}
	for (auto& it : vector)
	{
		str_lower = it.loc.departure;
		for (int i = 0; i < str_lower.size(); i++)
		{
			str_lower[i] = std::tolower(str_lower[i]);
		}
		if (str_lower.find(input_lower) != std::string::npos)
		{
			vec.push_back(it);
		}
	}
}

void Flight::SearchByArr(std::vector<Flight>& vec, const std::string& input)
{
	std::string str_lower;
	std::string input_lower = input;
	for (int i = 0; i < input.size(); i++)
	{
		input_lower[i] = std::tolower(input_lower[i]);
	}
	for (auto& it : vector)
	{
		str_lower = it.loc.arrival;
		for (int i = 0; i < str_lower.size(); i++)
		{
			str_lower[i] = std::tolower(str_lower[i]);
		}
		if (str_lower.find(input_lower) != std::string::npos)
		{
			vec.push_back(it);
		}
	}
}

bool Flight::Sort(const int type)
{
	bool isSorted = true;
	switch (type)
	{
	case FlightSortType::Type::Id:
		SortById();
		break;
	case FlightSortType::DateDeparture:
		SortByDate();
		break;
	case FlightSortType::LocDeparture:
		SortByDep();
		break;
	case FlightSortType::LocArrival:
		SortByArr();
		break;
	default:
		isSorted = false;
		break;
	}
	return isSorted;
}

std::vector<Flight> Flight::Search(const int type, const std::string& input)
{
	std::vector<Flight> output;
	switch (type)
	{
	case FlightSearchType::Id:
		SearchById(output, input);
		break;
	case FlightSearchType::LocDeparture:
		SearchByDep(output, input);
		break;
	case FlightSearchType::LocArrival:
		SearchByArr(output, input);
		break;
	default:
		break;
	}
	return output;
}



void Flight::CopyVector(std::vector<Flight>& destination)
{
	destination = vector;
}
Flight& Flight::operator = (const Flight& flight)
{
	id = flight.id;
	loc = flight.loc;
	time = flight.time;
	ticketBusiness = flight.ticketBusiness;
	ticketEconomy = flight.ticketEconomy;
	space = flight.space;
	return *this;
}

Flight& Flight::operator=(Flight&& flight) noexcept
{
	id = std::move(flight.id);
	loc = std::move(flight.loc);
	time = flight.time;
	ticketBusiness = flight.ticketBusiness;
	ticketEconomy = flight.ticketEconomy;
	space = flight.space;
	return *this;
}

void Flight::PrintInfo(const int mode, const int& count) const
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
	if (mode == InfoMode::AdminMode || mode == InfoMode::AdminMode_noCount)
		row.insert(row.end(), {
		std::to_string(ticketEconomy.count),
		std::to_string(ticketBusiness.count) });
	row.insert(row.end(), {
		std::to_string(ticketEconomy.price),
		std::to_string(ticketBusiness.price) });
	ClFomrat::PrintCenteredRow(row, CELL_WIDTH, ClFomrat::Border::Both, ClFomrat::Border::Bottom, ClFomrat::Border::Both);
}

void Flight::PrintInfoWithTop(const int mode) const
{
	PrintTopRow(mode);
	PrintInfo(mode);
}

void Flight::PrintInfoVector(const int mode)
{
	if (vector.size() < 1) return;

	PrintTopRow(mode);
	if (mode == UserMode || mode == AdminMode)
	{
		int count = 1;
		for (auto& it : vector)
			it.PrintInfo(mode, count++);
	}
	else for (auto& it : vector)
		it.PrintInfo(mode);
}

void Flight::PrintInfoVector(std::vector<Flight>& vec, const int mode)
{
	if (vec.size() < 1) return;

	PrintTopRow(mode);
	if (mode == UserMode || mode == AdminMode)
	{
		int count = 1;
		for (auto& it : vec)
			it.PrintInfo(mode, count++);

	}
	else for (auto& it : vector)
		it.PrintInfo(mode);
}

void Flight::PrintTopRow(const int mode)
{
	std::vector<std::string> row;
	row.reserve(CELLS);
	if (mode == UserMode || mode == AdminMode)
		row.emplace_back(flight_col0);
	row.insert(row.end(), {
		flight_col1,
		flight_col2,
		flight_col3,
		flight_col4,
		flight_col5,
		flight_col6 });
	if (mode == AdminMode || mode == AdminMode_noCount)
		row.insert(row.end(), { flight_col7, flight_col8 });
	row.insert(row.end(), { flight_col9, flight_col10 });
	ClFomrat::PrintCenteredRow(row, CELL_WIDTH, ClFomrat::Border::NoBorder, ClFomrat::Border::Bottom, ClFomrat::Border::NoBorder);
}

int Flight::GetFileStatus()
{
	return File::GetFileStatus(PATH_FILE_FLIGHTS, vector);
}

bool Flight::ReadFile()
{
	if (vector.size() > 0)
		vector.erase(vector.begin(), vector.end());
	VectorReserve();
	if (GetFileStatus() == FileStatus::Opened)
		return File::ReadFile(PATH_FILE_FLIGHTS, vector);
	else return false;
}

void Flight::VectorReserve(const size_t size)
{
	vector.reserve(size);
}

Ticket Flight::GetFlightTicket(const std::string& fullTicketId)
{
	std::string flightId = fullTicketId.substr(0, fullTicketId.find('_'));
	for (auto& it : vector)
	{
		if (it.id == flightId)
		{
			return Ticket(it, fullTicketId);
		}
	}
	return Ticket(-1);
}

Flight* Flight::GetFlight(const int index)
{
	return &vector[index];
}

bool Flight::WriteToFile()
{
	if (!std::is_sorted(vector.begin(), vector.end(), [](const Flight& l, const Flight& r) noexcept
		{
			return l.id < r.id;
		}))
	{
		SortById();
	}
		return File::WriteToFile(PATH_FILE_FLIGHTS, vector);
}

bool Flight::InputInfo()
{
	const char actionMsg[] = "Нажмите Enter, чтобы подтвердить или Esc чтобы вернуться назад";
	const char idMsg[] = "Номер рейса будет задан после добавления в список";
	char answer = 0;
	bool saveInput;
	do
	{
		InputString(loc.departure, "Место вылета: ");
		InputString(loc.arrival, "Место прибытия: ");
		if (loc.departure == loc.arrival)
			std::cout << "Место вылето и место прибытия идентичны.\n";
		else break;
	} while (true);

	InputDate(time.dtDeprature, "Дата вылета");
	InputTime(time.tmDeparture, "Время вылета");
	InputTime(time.tmArrival, "Время прибытия");

	InputVar(space, MIN_SPACE, MAX_SPACE, "Вместимость самолета: ");
	InputVar(ticketEconomy.count, 0, space, "Количество билетов эконом-класса: ");
	if (ticketEconomy.count != 0)
		InputVar(ticketBusiness.price, MIN_PRICE, MAX_PRICE, "Цена билета эконом-класса: ");
	if (ticketEconomy.count != space)
	{
		InputVar(ticketBusiness.count, 0, space - ticketEconomy.count, "Количество билетов бизнесс-класса: ");
		if (ticketBusiness.count != 0)
			InputVar(ticketBusiness.price, MIN_PRICE, MAX_PRICE, "Цена билета бизнес-класса: ");
	}
	else std::cout << "Места для посажиров бизнес-класса не осталось\n";



	PrintTopRow(InfoMode::AdminMode_noCount);
	this->PrintInfo(InfoMode::AdminMode_noCount);
	std::cout << '\n';
	ClFomrat::PrintCenteredLine(idMsg, CL_WIDTH_LINE, '-');
	ClFomrat::PrintCenteredNewLine(actionMsg, CL_WIDTH_LINE, '-');

	do answer = _getch();
	while (answer != ENTER_KEY && answer != ESC_KEY);
	switch (answer)
	{
	case ENTER_KEY:
		saveInput = true;
		break;
	case ESC_KEY:
		saveInput = false;
		break;
	}

	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	return saveInput;
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
	fs.get();

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

