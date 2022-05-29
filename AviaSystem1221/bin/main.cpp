#include "Hash.h"
#include "UserClass.h"
#include "UserInterface.h"
#include "FlightClass.h"
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
	//��������� �������� ����� � ������� ��� ����� � ������
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	//�������� ����������� ����� "Files", ���� � ���
	_mkdir("Files");

	//��������� ������� �� �������� ������
	system("mode 650");
	//��������� ������� �� ������ ������
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	//��������� ������ � ������� �������.
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	CL_WIDTH = csbi.srWindow.Right - csbi.srWindow.Left;
	CL_WIDTH_LINE = CL_WIDTH;
	CL_WIDTH += 8;
	CELLS = 12;
	CELL_WIDTH = CL_WIDTH / CELLS + 2;
	VECTOR_BUFF = 0x100000;
	UI::StartSystem();


	return 0;
}