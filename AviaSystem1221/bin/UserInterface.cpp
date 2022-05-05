#include "UserClass.h"
#include <iostream>
#include "UserInterface.h"
#include "Table.h"
#include <iomanip>


extern const char PATH_FILE_USERS[];
extern const char PATH_FILE_FLIGHTS[];
extern const int CONSOLE_WIDTH;
extern const int CELL_WIDTH;

const char noFileMsg[] = "�� ���������� ����� �� ����: ";
const char newFileMsg[] = "����� ������ ����� ����.";
const char noAdministratorMsg[] = "������������ � ������� �������������� ������ �� ���.";
const char newAdminMsg[] = "��������� ���� ������ ����� �������������� ��� ����� ��� �������������";
const char unknownErrFileMsg[] = "�������������� ������ ��� �������� ���������������� ����";
const char registrationFailMsg[] = "������������ � ����� ������� ��� ����������.";

void UI::Start()
{
	const char helloMsg1[] = "����� ���������� � �������������� ������� �� ���������";
	const char helloMsg2[] = "AviaSystem1221";

	Table::PrintCenteredLine(helloMsg1, CONSOLE_WIDTH, '-');
	Table::PrintCenteredLine(helloMsg2, CONSOLE_WIDTH, '-');

}

void UI::Register()
{
	
}
