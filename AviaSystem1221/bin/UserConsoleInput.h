#pragma once

#include <string>
#include <iostream>

//Ввод из консоли в строку. Все символы в консоли заменяются '*'. Ввод ограничен символами(кроме пробела) и английскими буквами.
void InputPassword(std::string& password, const int min, const int max);
//Ввод из консоли в строку. Ввод ограничен английскими символами, цифрами и символом '_'(нижний слэш). 
void InputLogin(std::string& login, const int min, const int max);
//Ввод из консоли в строку. Если msg не нуль, то в консоли будет выводится msg
void InputString(std::string& string, const char *msg = nullptr);
//Ввод из консоли в структуру tm. Ввод: tm_year, tm_mon, tm_mday. Если msg не нуль, то в консоли будет выводится msg
void InputDate(tm& date, const char* msg = nullptr);
//Ввод из консоли в структуру tm. Ввод: tm_hour, tm_min. Если msg не нуль, то в консоли будет выводится msg
void InputTime(tm& time, const char* msg = nullptr);
//Возврат строки в виде "{time.tm_year}.{time.tm_mon}.{time.tm_mday}"
std::string getDateString(const tm& time);
//Возврат строки в виде "{time.tm_hour}:{time.tm_min}"
std::string getTimeString(const tm& time);

bool is_russian(const unsigned char ch);
bool is_english(const unsigned char ch);

//Ввод из консоли в шаблонную переменную var встроенного типа. Попытка передечи объекта класса или указателя типа char (char*, char[]) вызывают ошибку.
template <class T>
void InputVar(T& var, const int MIN, const int MAX, const char msg[] = "Ввод: ")
{
	while (true)
	{
		std::cout << msg;
		std::cin >> var;
		if (!(std::cin))
		{
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			std::cout << "Некорректный ввод!\n\n";
		}
		else if (var < MIN || var > MAX)
		{
			std::cout << "\nМинимум: " << MIN << "\nМаксимум: " << MAX << "\n\n";
		}
		else break;
	}
}