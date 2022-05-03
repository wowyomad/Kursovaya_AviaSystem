#include "UserConsoleInput.h"

#include <string>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <ctime>

const char ENTER_KEY = 13;

void InputPassword(std::string& password, const int min, const int max)
{
	std::cout << "Пароль может содеражть любые символы, за исключением кириллицы.\n"
		<< "Пароль: ";
	password.clear();
	unsigned char ch = 0;
	do
	{
		ch = _getch();
		if (ch == ENTER_KEY)
		{
			if (password.length() < min)
			{
				std::cout << "\nОшибка. Пароля слишком короткий\n\n"
					<< "Пароль: ";
				password.clear();
			}
			else if (password.length() > max)
			{
				std::cout << "\nОшибка. Пароль слишком длинный\n\n"
					<< "Пароль: ";
				password.clear();
			}
			else
			{
				printf("\n\n");
				break;
			}
		}
		else if (ch == '\b')
		{
			if (password.length() > 0)
			{
				printf("\b \b");
				password.pop_back();
			}
		}
		else if (is_russian(ch))
		{
			std::cout << "\nОшибка. Введен русский символ!\n\n"
				<< "Пароль: ";
		}
		else
		{
			printf("*");
			password.push_back(ch);
		}
	} while (true);
}

void InputLogin(std::string& login, const int min, const int max)
{
	std::cout << "Логин может содержать символы английского алфавита, цифры и нижнее подчеркивание.\n"
		<< "Логин: ";
	login.clear();
	unsigned char ch = 0;
	do
	{
		ch = _getch();
		if (ch == ENTER_KEY)
		{
			if (login.length() < min)
			{
				std::cout << "\nОшибка. Логин слишком короткий\n\n"
					<< "Логин: ";
				login.clear();
			}
			else if (login.length() > max)
			{
				std::cout << "\nОшибка. Логин слишком длинный\n\n"
					<< "Логин: ";
				login.clear();
			}
			else
			{
				printf("\n\n");
				break;
			}
		}
		else if (ch == '\b')
		{
			if (login.length() > 0)
			{
				printf("\b \b");
				login.pop_back();
			}
		}
		else if (ch == ' ')
		{
			continue;
		}
		else if (is_english(ch) || isdigit(ch) || ch == '_')
		{
			printf("%c", ch);
			login.push_back(ch);
		}
		else
		{
			std::cout << "\nНедопустимый символ\n\n"
				<< "Логин: ";
			login.clear();
		}
	} while (true);
}

void InputString(std::string& string, const char* msg)
{
	if (msg) std::cout << msg;
	std::getline(std::cin, string);
}

void InputDate(tm& date, const char* msg)
{
	std::cout << msg << "\n";
	time_t rawtime = std::time(NULL);
	tm currentTime;
	localtime_s(&currentTime, &rawtime);
	int minMonth = 1;
	int minDay = 1;
	int maxDay = 0;

	InputVar(date.tm_year, currentTime.tm_year + 1900, INT_MAX, "Год: ");
	date.tm_year -= 1900;
	if (date.tm_year == currentTime.tm_year)
		minMonth = currentTime.tm_mon;

	InputVar(date.tm_mon, minMonth, 12, "Месяц: ");
	if (date.tm_year == currentTime.tm_year)
		if (date.tm_mon == currentTime.tm_mon)
			minDay = currentTime.tm_mday;
	date.tm_mon--;

	int year = date.tm_year + 1900;
	switch (date.tm_mon)
	{
		//февраль
	case 1:
		if (year % 400 == 0)
			maxDay = 29;
		else if (year % 100 == 0)
			maxDay = 28;
		else if (year % 4 == 0)
			maxDay = 29;
		else
			maxDay = 28;
		break;
		//апрель, июнь, сентябрь, ноябрь
	case 3: case 5: case 8: case 10:
		maxDay = 30;
		break;
		//остальные месяцы
	default:
		maxDay = 31;
		break;
	}

	InputVar(date.tm_mday, minDay, maxDay, "Число: ");

}

void InputTime(tm& time, const char* msg)
{
	if (msg) std::cout << msg << "\n";
	InputVar(time.tm_hour, 0, 23, "Часы: ");
	InputVar(time.tm_min, 0, 59, "Минуты: ");
}

std::string getDateString(const tm& time)
{
	std::ostringstream os;
	os << std::put_time(&time, "%d.%m.%Y");
	return os.str();
}

std::string getTimeString(const tm& time)
{
	std::ostringstream os;
	os << std::put_time(&time, "%H:%M");
	return os.str();
}


bool is_russian(const unsigned char ch)
{
	return ch >= 'а' && ch <= 'я' || ch >= 'А' && ch <= 'Я';
}

bool is_english(const unsigned char ch)
{
	return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}