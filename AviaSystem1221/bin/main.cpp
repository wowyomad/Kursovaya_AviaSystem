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

int CL_WIDTH;
int CL_WIDTH_LINE;
int CELL_WIDTH;
int CELLS;
size_t VECTOR_BUFF;

int main(int argc, char** argv)
{
	//Установка русского языка в косноли для ввода и вывода
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	_mkdir("Files");

	system("mode 650");
	//Раскрытие консоли в полный экран
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	//Получение данных о размере консоли для.
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	CL_WIDTH = csbi.srWindow.Right - csbi.srWindow.Left;
	CL_WIDTH_LINE = CL_WIDTH;
	CL_WIDTH += 8;
	CELLS = 11;
	CELL_WIDTH = CL_WIDTH / CELLS;
	VECTOR_BUFF = 0x100000;
	Flight::ReadFile();
	
	_getch();
	ClearConsole();
	std::cout << "THE END!\n";

	return 0;
}