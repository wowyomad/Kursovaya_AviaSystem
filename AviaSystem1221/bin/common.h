#pragma once
#include <ctime>
#include <Windows.h>	
extern int CL_WIDTH;
extern int CL_WIDTH_LINE;
extern int CELL_WIDTH;
extern int CELLS;
extern size_t VECTOR_BUFF;

const char INVALID_TICKET_INDEX[] = "Обращение к несуществующему билету";
const char INVALID_FLIGHT_INDEX[] = "Обращение к несущетсвующему рейсу";
const char INVALID_USER_ACCESS[] = "Неизвестный идентификатор доступа";

namespace FlightSortType
{
	enum Type
	{
		Id,
		LocDeparture,
		LocArrival,
		DateDeparture,
	};
}

namespace UserSortType
{
	enum Type
	{
		Login
	};
}
namespace FlightSearchType
{
	enum Type
	{
		Id,
		LocDeparture,
		LocArrival,
	};
}

namespace UserSearchType
{
	enum Type
	{
		Login
	};
}
inline bool operator <(const tm& l, const tm& r)
{
	if (l.tm_year == r.tm_year)
		if (l.tm_mon == r.tm_mon)
			if (l.tm_mday == r.tm_mday)
				return false;
			else return l.tm_mday < r.tm_mday;
		else return l.tm_mon < r.tm_mon;
	else return l.tm_year < r.tm_year;
}

inline bool operator >(const tm& l, const tm& r)
{
	if (l.tm_year == r.tm_year)
		if (l.tm_mon == r.tm_mon)
			if (l.tm_mday == r.tm_mday)
				return false;
			else return l.tm_mday < r.tm_mday;
		else return l.tm_mon < r.tm_mon;
	else return l.tm_year < r.tm_year;
}

inline bool operator ==(const tm& l, const tm& r)
{
	if (l.tm_year == r.tm_year)
		if (l.tm_mon == r.tm_mon)
			if (l.tm_mday == r.tm_mday)
				return true;
	return false;
}

inline void ClearConsole()
{
	COORD startPos = { 0, 0 };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(hConsole, &screen);
	FillConsoleOutputCharacterA(hConsole, ' ', screen.dwSize.X * screen.dwSize.Y, startPos, &written);
	FillConsoleOutputAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, startPos, &written);
	SetConsoleCursorPosition(hConsole, startPos);
}