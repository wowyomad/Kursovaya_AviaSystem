#include "FlightClass.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include "Table.h"
#include "UserConsoleInput.h"

namespace AviaLines
{

	static const int MIN_PRICE = 1;
	static const int MAX_PRICE = INT_MAX;
	static const int MIN_SPACE = 1;
	static const int MAX_SPACE = 1000;

	static const int FIELDS = 10;
	static const int CELL_WIDTH = 18;

	extern const char PATH_BASE_FLIGHTS[] = "Files\\FlightsBase.dat";

	static const char default_string[] = "default";

	static const char ticket_col1[] = "Номер билета";
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

	std::vector<AviaLines::Flight> AviaLines::Flight::vectorFligths;
	std::fstream AviaLines::Flight::fileFlights;

	Ticket::Ticket()
	{
		id = "default";
		loc = { "default", "default" };
	}

	Ticket::Ticket(const Ticket& source)
	{
		id = source.id;
		time = source.time;
		loc = source.loc;
		space = source.space;
	}

	Ticket::Ticket(Ticket&& source)
	{
		id = std::move(source.id);
		time = std::move(source.time);
		loc = std::move(source.loc);
		space = std::move(source.space);
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


	void AviaLines::Flight::setId(const std::string& id)
	{
		this->id = id;
	}

	void AviaLines::Flight::setLocDeparture(const std::string& location)
	{
		loc.departure = location;
	}

	void AviaLines::Flight::setLocArrival(const std::string& location)
	{
		loc.arrival = location;
	}

	void AviaLines::Flight::setDateDepatrue(tm& time)
	{
		this->time.dtDeprature.tm_year = time.tm_year;
		this->time.dtDeprature.tm_mon = time.tm_mon;
		this->time.dtDeprature.tm_mday = time.tm_mday;
	}

	void AviaLines::Flight::setTimeDeparture(tm& time)
	{
		this->time.tmDeparture.tm_hour = time.tm_hour;
		this->time.tmDeparture.tm_hour = time.tm_min;
	}

	void AviaLines::Flight::setTimeArrival(tm& time)
	{
		this->time.tmArrival.tm_hour = time.tm_hour;
		this->time.tmArrival.tm_min = time.tm_mon;
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

	void Flight::PushToVector()
	{
		vectorFligths.emplace_back(*this);
	}

	bool Flight::ValidateInfo()
	{
		return true;
	}

	bool Flight::SaveToFile()
	{
		fileFlights.open(PATH_BASE_FLIGHTS, std::ios::out);
		bool state = File::WriteVectorToFile(fileFlights, vectorFligths);
		fileFlights.close();
		return state;
	}

	bool Flight::ReadFromFile()
	{
		fileFlights.open(PATH_BASE_FLIGHTS, std::ios::in);
		bool state = File::ReadFileToVector(fileFlights, vectorFligths);
		fileFlights.close();
		return state;
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

	void AviaLines::Flight::PrintFlightInfo(const int mode, const int& count)
	{
		std::vector<std::string> row;
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

	void AviaLines::Flight::PrintFlightInfo_withTop(const int mode)
	{
		PrintTopRaw(mode);
		PrintFlightInfo(mode);
	}

	void AviaLines::Flight::PrintFlightInfoWhole(const int mode)
	{
		if (vectorFligths.size() > 0)
		{
			PrintTopRaw(mode);
			if (mode == User || mode == Admin)
			{
				int count = 1;
				for (auto& it : vectorFligths)
					it.PrintFlightInfo(mode, count++);
			}
			else for (auto& it : vectorFligths)
				it.PrintFlightInfo();

		}
	}

	void AviaLines::Flight::PrintTopRaw(const int mode)
	{
		std::vector<std::string> topRow;
		topRow.reserve(FIELDS);
		if (mode == User || mode == Admin)
			topRow.emplace_back(flight_col0);
		topRow.insert(topRow.end(), { flight_col1,
			flight_col2, flight_col3,
			flight_col4,
			flight_col5,
			flight_col6 });
		if (mode == Admin)
			topRow.insert(topRow.end(), { flight_col7, flight_col8 });
		Table::PrintRow(topRow, CELL_WIDTH, Table::Border::NoBorder, Table::Border::Bottom, Table::Border::NoBorder);
	}

	std::fstream& operator<<(std::fstream& fs, AviaLines::Flight& flight)
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