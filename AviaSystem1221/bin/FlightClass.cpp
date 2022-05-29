#include "FlightClass.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <conio.h>
#include "PrintFormat.h"
#include "UserConsoleInput.h"

static const int MIN_PRICE = 1;
static const int MAX_PRICE = INT_MAX;
static const int MIN_SPACE = 1;
static const int MAX_SPACE = 1000;
static const int FLIGHT_ID_LENGTH = 8;
static const int PASANGERS_BUFF = 8;

static const char ENTER_KEY = 13;
static const char ESC_KEY = 27;

extern const char PATH_FILE_FLIGHTS[] = "Files\\FlightsBase.dat";
static const char TICKET_TYPE_BUSINESS[] = "Бизнесс-класс";
static const char TICKET_TYPE_ECONOMY[] = "Эконом-класс";
static const char DEFAULT_STR[] = "default";

static const char TICKET_ID_COL[] = "Номер билета";
static const char TICKET_TYPE_COL[] = "Класс";
static const char TICKET_DEPARTURE_COL[] = "Место вылета";
static const char TICKET_ARRIVAL_COL[] = "Место прибытия";
static const char TICKET_DT_DEPARTURE_COL[] = "Дата вылета";
static const char TIKCET_TM_DEPARTURE_COL[] = "Время вылета";
static const char TICKET_TM_ARRIVAL_COL[] = "Время прибытия";

static const char NUMBER_COl[] = "Порядковый номер";
static const char FLIGHT_ID_COL[] = "Номер рейса";
static const char* FLIGHT_DEPARTURE_COL = TICKET_DEPARTURE_COL;
static const char* FLIGHT_ARRIVAL_COL = TICKET_ARRIVAL_COL;
static const char* FLIGHT_DT_DEPARTURE_COL = TICKET_DT_DEPARTURE_COL;
static const char* FLIGHT_TM_DEPARTURE_COL = TIKCET_TM_DEPARTURE_COL;
static const char* FLIGHT_TM_ARRIVAL_COL = TICKET_TM_ARRIVAL_COL;
static const char FLIGHT_ECONOMY_COUNT_COL[] = "Эконом, билеты";
static const char FLIGHT_BUSINESS_COUNT_COL[] = "Бизнес, билеты";
static const char FLIGHT_ECONOMY_PRICE_COL[] = "Эконом, стоимость";
static const char FLIGHT_BUSINESS_PRICE_COL[] = "Бизнес, стоимость";


enum InfoMode
{
	Count,
	NoCount
};

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

void Ticket::PrintInfo(const int& count) const
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
	ClFomrat::PrintRow(row, CELL_WIDTH, ClFomrat::Border::Both, ClFomrat::Border::Bottom, ClFomrat::Border::Both);
}

void Ticket::PrintInfoWithTop() const
{
	PrintTopRow();
	PrintInfo();
}

void Ticket::PrintTopRow()
{
	std::vector<std::string> topRow;
	topRow.reserve(CELLS);
	topRow.insert(topRow.end(), {
		TICKET_ID_COL,
		TICKET_TYPE_COL,
		TICKET_DEPARTURE_COL,
		TICKET_ARRIVAL_COL,
		TICKET_DT_DEPARTURE_COL,
		TIKCET_TM_DEPARTURE_COL,
		TICKET_TM_ARRIVAL_COL
		});
	ClFomrat::PrintRow(topRow, CELL_WIDTH, ClFomrat::Border::NoBorder, ClFomrat::Border::Bottom, ClFomrat::Border::NoBorder);

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
	passangers.reserve(PASANGERS_BUFF);
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

void Flight::RemoveFlight(const size_t index)
{
	if (index > vector.size()) throw std::exception("Попытка выхода за пределы массива");
	vector.erase(vector.begin() + index);
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

int Flight::getTicketPrice(const int type)
{
	if (type == TicketType::Business)
		return ticketBusiness.price;
	else if (type == TicketType::Economy)
		return ticketEconomy.price;
	else throw std::exception("Несуществующий тип билета: " + type);
}

int Flight::getTicketCount(const int type)
{
	if (type == TicketType::Business)
		return ticketBusiness.count;
	else if (type == TicketType::Economy)
		return ticketEconomy.count;
	else throw std::exception("Несуществующий тип билета: " + type);
}

size_t Flight::getPassangerCount()
{
	return passangers.size();
}

size_t Flight::getFlightCount()
{
	return vector.size();
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

int Flight::GetFlightIndex(const Flight* flightPtr)
{
	if (flightPtr == nullptr) throw std::exception("В функцию GetFlightIndex был передан нулевой указатель");
	return (flightPtr - vector.data());
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
	if (ticketEconomy.count < 0 || ticketBusiness.count < 0)
		throw std::exception(INVALID_FLIGHT_INDEX);
}

std::string Flight::GenerateTicketID(const int index, const int type)
{
	if (vector.size() < index + 1)
		throw std::invalid_argument("Попытка выхода за пределы массива.");

	return vector[index].GenerateTicketID(type);
}

void Flight::AddPasanger(std::string& login, const int type)
{
	std::string passangerInfo = login + '_' + std::to_string(type);
	passangers.emplace_back(passangerInfo);
}


bool Flight::TicketAvailable(const int type, const int count) const
{
	if (type == TicketType::Economy)
		return ticketEconomy.count >= count ? true : false;
	else if (type == TicketType::Business)
		return ticketBusiness.count >= count ? true : false;
	else throw std::exception("Что-то пошло не так при получении типа билета");
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

void Flight::SortById(std::vector<Flight*>& flightVector)
{
	auto cmp = [](const Flight* l, const Flight* r) noexcept
	{
		return l->id < r->id;
	};

	std::sort(flightVector.begin(), flightVector.end(), cmp);
}

void Flight::SortByDate(std::vector<Flight*>& flightVector)
{
	auto cmp = [](const Flight* l, const Flight* r) noexcept
	{
		return  l->time.dtDeprature < r->time.dtDeprature;
	};

	std::sort(flightVector.begin(), flightVector.end(), cmp);
}

void Flight::SortByDep(std::vector<Flight*>& flightVector)
{
	auto cmp = [](const Flight* l, const Flight* r) noexcept
	{
		return l->loc.departure < r->loc.departure;
	};

	std::sort(flightVector.begin(), flightVector.end(), cmp);
}

void Flight::SortByArr(std::vector<Flight*>& flightVector)
{
	auto cmp = [](const Flight* l, const Flight* r)
	{
		return l->loc.arrival < r->loc.arrival;
	};

	std::sort(flightVector.begin(), flightVector.end(), cmp);
}

void Flight::SearchById(const std::vector<Flight*>& source, std::vector<Flight*>& destination, const std::string& input)
{
	std::string str_lower;
	std::string input_lower = input;
	for (int i = 0; i < input.size(); i++)
	{
		input_lower[i] = std::tolower(input_lower[i]);
	}
	for (auto& it : source)
	{
		str_lower = it->id;
		for (int i = 0; i < str_lower.size(); i++)
		{
			str_lower[i] = std::tolower(str_lower[i]);
		}
		if (str_lower.find(input_lower) != std::string::npos)
		{
			destination.push_back(it);
		}
	}
}

void Flight::SearchByDep(const std::vector<Flight*>& source, std::vector<Flight*>& destination, const std::string& input)
{
	std::string str_lower;
	std::string input_lower = input;
	for (int i = 0; i < input.size(); i++)
	{
		input_lower[i] = std::tolower(input_lower[i]);
	}
	for (auto& it : source)
	{
		str_lower = it->loc.departure;
		for (int i = 0; i < str_lower.size(); i++)
		{
			str_lower[i] = std::tolower(str_lower[i]);
		}
		if (str_lower.find(input_lower) != std::string::npos)
		{
			destination.push_back(it);
		}
	}
}

void Flight::SearchByArr(const std::vector<Flight*>& source, std::vector<Flight*>& destination, const std::string& input)
{
	std::string str_lower;
	std::string input_lower = input;
	for (int i = 0; i < input.size(); i++)
	{
		input_lower[i] = std::tolower(input_lower[i]);
	}
	for (auto& it : source)
	{
		str_lower = it->loc.arrival;
		for (int i = 0; i < str_lower.size(); i++)
		{
			str_lower[i] = std::tolower(str_lower[i]);
		}
		if (str_lower.find(input_lower) != std::string::npos)
		{
			destination.push_back(it);
		}
	}
}

void Flight::SortById()
{
	auto cmp = [](const Flight& l, const Flight& r) noexcept
	{
		return l.id < r.id;
	};

	if (!std::is_sorted(vector.begin(), vector.end(), cmp))
		std::sort(vector.begin(), vector.end(), cmp);
}


void Flight::SortByDate()
{
	auto cmp = [](const Flight& l, const Flight& r) noexcept
	{
		return  l.time.dtDeprature < r.time.dtDeprature;
	};

	if (!std::is_sorted(vector.begin(), vector.end(), cmp))
		std::sort(vector.begin(), vector.end(), cmp);
}

void Flight::SortByDep()
{
	auto cmp = [](const Flight& l, const Flight& r) noexcept
	{
		return l.loc.departure < r.loc.departure;
	};

	if (!std::is_sorted(vector.begin(), vector.end(), cmp))
		std::sort(vector.begin(), vector.end(), cmp);

}

void Flight::SortByArr()
{
	auto cmp = [](const Flight& l, const Flight& r) noexcept
	{
		return l.loc.arrival < r.loc.arrival;
	};

	if (!std::is_sorted(vector.begin(), vector.end(), cmp))
		std::sort(vector.begin(), vector.end(), cmp);
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
void Flight::CopyVectorPtr(std::vector<Flight*>& destination)
{
	destination.clear();
	destination.reserve(vector.size());
	for (size_t i = 0; i < vector.size(); i++)
	{
		destination.emplace_back(&vector[i]);
	}
}
Flight& Flight::operator = (const Flight& flight)
{
	id = flight.id;
	loc = flight.loc;
	passangers = flight.passangers;
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
	passangers = std::move(flight.passangers);
	time = flight.time;
	ticketBusiness = flight.ticketBusiness;
	ticketEconomy = flight.ticketEconomy;
	space = flight.space;
	return *this;
}

void Flight::PrintInfo(const int& count) const
{
	std::vector <std::string> row;
	row.reserve(CELLS);
	row.insert(row.end(), {
		std::to_string(count),
		id,
		loc.departure,
		loc.arrival,
		getDateString(time.dtDeprature),
		getTimeString(time.tmDeparture),
		getTimeString(time.tmArrival),
		std::to_string(ticketEconomy.count),
		std::to_string(ticketBusiness.count),
		std::to_string(ticketEconomy.price),
		std::to_string(ticketBusiness.price) });
	ClFomrat::PrintRow(row, CELL_WIDTH, ClFomrat::Border::Both, ClFomrat::Border::Bottom, ClFomrat::Border::Both);
}

void Flight::PrintInfoWithTop() const
{
	PrintTopRow();
	PrintInfo();
}

void Flight::PrintInfoVector()
{
	if (vector.size() < 1) return;

	PrintTopRow();
	int count = 1;
	for (auto& it : vector)
	{
		it.PrintInfo(count++);
	}
}

void Flight::PrintInfoVector(std::vector<Flight*>& flightVEector)
{
	if (flightVEector.empty()) return;

	PrintTopRow();
	int count = 1;
	for (auto& it : flightVEector)
	{
		it->PrintInfo(count++);
	}
}

void Flight::PrintTopRow()
{
	std::vector<std::string> row;
	row.reserve(CELLS);
	row.insert(row.end(), {
		NUMBER_COl,
		FLIGHT_ID_COL,
		FLIGHT_DEPARTURE_COL,
		FLIGHT_ARRIVAL_COL,
		FLIGHT_DT_DEPARTURE_COL,
		FLIGHT_TM_DEPARTURE_COL,
		FLIGHT_TM_ARRIVAL_COL,
		FLIGHT_ECONOMY_COUNT_COL,
		FLIGHT_BUSINESS_COUNT_COL,
		FLIGHT_ECONOMY_PRICE_COL,
		FLIGHT_BUSINESS_PRICE_COL });
	ClFomrat::PrintRow(row, CELL_WIDTH, ClFomrat::Border::NoBorder, ClFomrat::Border::Bottom, ClFomrat::Border::NoBorder);
}

void Flight::PrintPassangers() const
{
	if (passangers.size() < 1) return;

	std::string login;
	std::string type;
	for (auto& it : passangers)
	{
		std::vector<std::string> row; //Строка таблицы
		row.insert(row.begin(), {
			it.substr(0, it.length() - 2),	//Имя пользователя
			it.back() - '0' == TicketType::Economy ? "Эконом-класс" : "Бизнес-класс" //Класс билета
			});
		ClFomrat::PrintRow(row, CELL_WIDTH, ClFomrat::Border::Both, ClFomrat::Border::Both, ClFomrat::Border::Both);
	}
}


int Flight::GetFileStatus()
{
	return File::GetFileStatus(PATH_FILE_FLIGHTS, vector);
}

bool Flight::ReadFile()
{
	if (vector.size() > 0)
		vector.erase(vector.begin(), vector.end());
	vector.reserve(VECTOR_BUFF);
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

Flight* Flight::getFlightPtr(const int index)
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

void Flight::InputInfo()
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
		InputVar(ticketEconomy.price, MIN_PRICE, MAX_PRICE, "Цена билета эконом-класса: ");
	if (ticketEconomy.count != space)
	{
		InputVar(ticketBusiness.count, 0, space - ticketEconomy.count, "Количество билетов бизнесс-класса: ");
		if (ticketBusiness.count != 0)
			InputVar(ticketBusiness.price, MIN_PRICE, MAX_PRICE, "Цена билета бизнес-класса: ");
	}
	else std::cout << "Места для посажиров бизнес-класса не осталось\n";


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
	fs << flight.ticketEconomy.count << '\n';

	long long limiter = flight.passangers.size() - 1;
	if (limiter > 0)
	{
		for (long long i = 0; i < limiter; i++)
		{
			fs << flight.passangers[i] << '\n';
		}
	}
	if (limiter > -1)
		fs << flight.passangers[limiter] << '\n';
	fs << '#';

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

	int i = 0;

	std::string str;
	fs >> str;
	while (str != "#")
	{
		if (flight.passangers.capacity() < i)
			flight.passangers.reserve(flight.passangers.capacity() + PASANGERS_BUFF);
		flight.passangers.emplace_back(str);
		fs >> str;
		i++;
	}

	return fs;
}

