#pragma once

#include <string>
#include <iostream>

//���� �� ������� � ������. ��� ������� � ������� ���������� '*'. ���� ��������� ���������(����� �������) � ����������� �������.
void InputPassword(std::string& password, const int min, const int max);
//���� �� ������� � ������. ���� ��������� ����������� ���������, ������� � �������� '_'(������ ����). 
void InputLogin(std::string& login, const int min, const int max);
//���� �� ������� � ������. ���� msg �� ����, �� � ������� ����� ��������� msg
void InputString(std::string& string, const char *msg = nullptr);
//���� �� ������� � ��������� tm. ����: tm_year, tm_mon, tm_mday. ���� msg �� ����, �� � ������� ����� ��������� msg
void InputDate(tm& date, const char* msg = nullptr);
//���� �� ������� � ��������� tm. ����: tm_hour, tm_min. ���� msg �� ����, �� � ������� ����� ��������� msg
void InputTime(tm& time, const char* msg = nullptr);
//������� ������ � ���� "{time.tm_year}.{time.tm_mon}.{time.tm_mday}"
std::string getDateString(const tm& time);
//������� ������ � ���� "{time.tm_hour}:{time.tm_min}"
std::string getTimeString(const tm& time);

bool is_russian(const unsigned char ch);
bool is_english(const unsigned char ch);

//���� �� ������� � ��������� ���������� var ����������� ����. ������� �������� ������� ������ ��� ��������� ���� char (char*, char[]) �������� ������.
template <class T>
void InputVar(T& var, const int MIN, const int MAX, const char msg[] = "����: ")
{
	while (true)
	{
		std::cout << msg;
		std::cin >> var;
		if (!(std::cin))
		{
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			std::cout << "������������ ����!\n\n";
		}
		else if (var < MIN || var > MAX)
		{
			std::cout << "\n�������: " << MIN << "\n��������: " << MAX << "\n\n";
		}
		else break;
	}
}