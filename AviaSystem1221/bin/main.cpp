#include "Hash.h"
#include "UserClass.h"
#include "UserInterface.h"
#include "FlightClass.h"
#include "Table.h"
#include <conio.h>
#include <iostream>
#include <Windows.h>
#include "UserConsoleInput.h"
#include <iomanip>
#include <string>
#include <sstream>
#include <direct.h>
#include <algorithm>
#include <vector>
#include <list>
#include "common.h"

int CONSOLE_WIDTH;
int CELL_WIDTH;
int CELLS;
size_t VECTOR_BUFF;

struct Struct
{
	std::string str;
	std::string login;
};


void cls()
{
	HANDLE hOut;
	COORD Position;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

int main(int argc, char** argv)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode 650");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	CONSOLE_WIDTH = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	CELLS = 10;
	CELL_WIDTH = CONSOLE_WIDTH / CELLS;
	VECTOR_BUFF = 0x100000;
	_mkdir("Files");
	Flight flight;
	flight.PushToVector();
	flight.PushToVector();
	flight.PushToVector();
	Flight::PrintInfoWhole(1);
	/*Entity::VectorReserve();
	Client::VectorReserve();
	Flight::VectorReserve();*/
	UI::Start();

	std::cout << "THE END!\n";
	_getch();

	return 0;
}