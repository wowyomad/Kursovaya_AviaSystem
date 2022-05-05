#include "Hash.h"
#include "UserClass.h"
#include "UserInterface.h"
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
using namespace AviaLines;

int main(int argc, char** argv)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode 650");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	_mkdir("Files");


	UI::Start();
	
	Flight flight;

	return 0;
}