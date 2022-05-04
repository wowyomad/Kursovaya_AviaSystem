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



class Str
{
	std::string name;
	std::string something;
	int age;
public:
	std::string getName() { return name; }
	Str(std::string str, int age) : name(str), something(str), age(age) {};
};

int main(int argc, char** argv)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Flight flight;
	std::vector<Str> arr;
	arr.push_back(Str("wowow", 25));
	arr.push_back(Str("b00ba", 35));
	arr.push_back(Str("aboba", 10));
	for (auto& it : arr)
	{
		std::cout << it.getName() << '\n';
	}

	std::sort(arr.begin(), arr.end(), []( auto& left,  auto& right)
		{
			return left.getName() < right.getName();
		});
	for (auto& it : arr)
	{
		std::cout << it.getName() << '\n';
	}
	_getch();
	Flight::WriteVectorToFile();
	return 0;
}