#include "UserInterface.h"
#include "FlightClass.h"
#include "UserConsoleInput.h"
#include "Table.h"
#include <iomanip>
#include <iostream>
#include <conio.h>

extern const char PATH_FILE_USERS[];
extern const char PATH_FILE_FLIGHTS[];

static const char ENTER_KEY = 13;
static const char ESC_KEY = 27;

const char noFileMsg[] = "�� ���������� ����� �� ����: ";
const char newAdminMsg[] = "��������� ���� ������ ����� �������������� ��� ����� ��� �������������";
const char unknownErrFileMsg[] = "�������������� ������ ��� �������� ���������������� ����";
const char registrationFailMsg[] = "������������ � ����� ������� ��� ����������.";
const char pressKeyMsg[] = "������� ����� ������ ����� ����������.";

const char fillCh = '-';

void UI::Start()
{
	const char helloMsg1[] = "����� ���������� � �������������� ������� �� ���������";
	const char helloMsg2[] = "AviaSystem1221";
	const char noUserFileMsg[] = "���� � ����������������� ������� �� ���������";
	const char noFlightFileMsg[] = "���� � ������� �� ���������� �� ���������";
	const char noClientFileMsg[] = "���� � ���������� ����� �� ���������";
	const char emptyUserFIleMsg[] = "���� � ����������������� ������� ������.";
	const char emptyFlightFIleMsg[] = "���� � ������� �� ���������� ������.";
	const char emptyClientFileMsg[] = "���� � ���������� ����� ������.";
	const char newFileMsg[] = "����� ������ ����� ����.";
	const char noAdminMsg[] = "������������ � ������� �����-������ ������ �� ���.";
	const char adminReqMsg[] = "������������� ����� ��� �����.";
	const char goodFileMsg[] = "��� ����� ���� ������� ����������������.";


	ClFomrat::PrintCenteredLine(helloMsg1, CL_WIDTH, fillCh);
	ClFomrat::PrintCenteredLine(helloMsg2, CL_WIDTH, fillCh);

	int status = User::GetFileStatus();
	if (status == FileStatus::NotOpened)
	{
		ClFomrat::PrintCenteredLine(noUserFileMsg, CL_WIDTH, fillCh);
		ClFomrat::PrintCenteredLine(newFileMsg, CL_WIDTH, fillCh);
		ClFomrat::PrintCenteredLine(newAdminMsg, CL_WIDTH, fillCh);
		User::CreateNewUser(AccessLevel::SuperAdminLvl);
	}
	else if (status == FileStatus::Empty)
	{
		ClFomrat::PrintCenteredLine(emptyUserFIleMsg, CL_WIDTH, fillCh);
		ClFomrat::PrintCenteredLine(newAdminMsg, CL_WIDTH, fillCh);
		User::CreateNewUser(AccessLevel::SuperAdminLvl);
	}
	else
	{
		User::ReadFile();
		bool superAdminExist = User::CheckForSuperAdmin;
		if (!superAdminExist)
		{
			ClFomrat::PrintCenteredLine(noAdminMsg, CL_WIDTH, fillCh);
			ClFomrat::PrintCenteredLine(newAdminMsg, CL_WIDTH, fillCh);
			User::CreateNewUser(AccessLevel::SuperAdminLvl);
		}
	}

	status = Client::GetFileStatus();
	if (status == FileStatus::NotOpened)
		Client::CreateNewFIle();
	else if (status == FileStatus::Opened)
		Client::ReadFile();

	status = Flight::GetFileStatus();
	if (status == FileStatus::NotOpened)
		Flight::CreateNewFile();
	else if (status == FileStatus::Opened)
		Flight::ReadFile();

	ClFomrat::PrintCenteredLine(pressKeyMsg, CL_WIDTH, fillCh);
	_getch();

	Main();
}

void UI::Main()
{
	ClearConsole();

	const char mainMsg[] = "������� ����";
	const char option1Msg[] = "1.�����������";
	const char option2Msg[] = "2.�����";
	const char option3Msg[] = "Esc.�����";

	ClFomrat::PrintCenteredLine(mainMsg, CL_WIDTH, fillCh);
	std::cout << '\n';
	ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH, fillCh);
	ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH, fillCh);
	ClFomrat::PrintCenteredLine(option3Msg, CL_WIDTH, fillCh);

	bool success = false;
	while (!success)
	{
		char ch = _getch();
		switch (ch)
		{
		case '1':
			Register();
			break;
		case '2':
			Login();
			break;
		case ESC_KEY:
			success = true;
			break;
		default:
			break;
		}
	}
}

void UI::Register(const int accessLevel)
{
	ClearConsole();

	const char rigestserMsg[] = "�����������";
	const char successMsg[] = "������������ ������� ���������������.";
	const char userExistMsg[] = "������������ � ����� ������� ��� ���������������.";
	const char waitMsg[] = "�������� ������������� �� ��������������.";

	ClFomrat::PrintCenteredLine(rigestserMsg, CL_WIDTH, '*');
	std::cout << '\n';

	if (User::CreateNewUser())
	{
		std::cout << successMsg << '\n';
		std::cout << waitMsg << '\n';
	}
	else
	{
		std::cout << userExistMsg << '\n';
	}
	std::cout << '\n';
	std::cout << pressKeyMsg << '\n';
	_getch();
	Main();
}

void UI::Login()
{
	ClearConsole();

	const char loginMsg[] = "����";
	const char noUserMsg[] = "������������ �� ������.";
	const char retryMsg[] = "������ ��������� �������?";
	const char successMsg[] = "�� ������� ����� � �������.";
	const char adminMsg[] = "�� ������ �������������� � ���� ��������������.";
	const char clientMsg[] = "�� ������ �������������� � ���� �������";

	ClFomrat::PrintCenteredLine(loginMsg, CL_WIDTH, '*');
	do
	{
		int accessLevel = User::LoginUser();
		if (accessLevel > -1)
		{
			ClFomrat::PrintCenteredLine(successMsg, CL_WIDTH, '*');
			if (accessLevel == AccessLevel::AdminLvl)
			{
				ClFomrat::PrintCenteredLine(adminMsg, CL_WIDTH, '*');
				ClFomrat::PrintCenteredLine(pressKeyMsg, CL_WIDTH, fillCh);
				_getch();
				UI::AdminMain();
			}
			else
			{
				ClFomrat::PrintCenteredLine(clientMsg, CL_WIDTH, '*');
			}
		}
		else
		{
			std::cout << noUserMsg << '\n';
			std::cout << retryMsg << '\n';

			bool correctInput = true;
			while (!correctInput)
			{
				char ch = _getch();
				switch (ch)
				{
				case '1':
					correctInput = true;
					break;
				case '0':
					correctInput = true;
					UI::Main();
				default:
					correctInput = false;
					break;
				}
			}
		}
	} while (true);
}

void UI::AdminMain()
{
	ClearConsole();
	const char mainMsg[] = "���� ��������������";
	const char op1ion1Msg[] = "1.��������� ������";
	const char option2Msg[] = "2.��������� �������������";
	const char option3Msg[] = "2.�������� ��������";

	ClFomrat::PrintCenteredLine(mainMsg, CL_WIDTH, fillCh);

	while (true)
	{

	}
}

void UI::ViewAllFlights(const int mode)
{
	const char option1Msg[] = "1.�������";
	const char option2Msg[] = "2.�������������";
	const char option3Msg[] = "3.�����";
	const char optoin4Msg[] = "4.�������� �����";
	const char option4Msg[] = "Esc.�����";
	bool done = false;
	char answer;
	while (!done)
	{
		Flight::PrintInfoWhole(mode);
		std::cout << '\n';
		ClFomrat::PrintCenteredLine(option1Msg, fillCh);
		ClFomrat::PrintCenteredLine(option2Msg, fillCh);
		ClFomrat::PrintCenteredLine(option3Msg, fillCh);
		ClFomrat::PrintCenteredLine(option4Msg, fillCh);
		do
		{
			answer = _getch();
		} while (answer < 1 && answer > 2 && answer != ESC_KEY);
		switch (answer)
		{
		case '1':
			int num;
			InputVar(num, 1, Flight::getVectorSize(), "�����: ");
			AdminViewFlight(num - 1);
			break;
		case '2':
			SortFlights();
			break;
		case '3':
			SearchFligths(mode);
		case '4':
			Admin;
		case ESC_KEY:
			done = true;
			break;
		}
	}
}



void UI::AdminViewAllUsers()
{
}

void UI::AdminViewSpecificUsers(std::vector<Flight> vec, const int mode)
{

}




void UI::ClientMain()
{

}

void UI::AdminViewFlight(const int index)
{
	const char option1Msg[] = "1. �������� ������";
	const char option2Msg[] = "Esc. �����";
	bool correctInput = false;
	char input;
	while (!correctInput)
	{
		correctInput = true;
		ClFomrat::PrintCenteredLine(option1Msg, fillCh);
		ClFomrat::PrintCenteredLine(option2Msg, fillCh);
		input = _getch();
		switch (input)
		{
		case '1':
			AdminEditFlight(index);
			break;
		case ESC_KEY:
			break;
		default:
			correctInput = false;
			break;
		}
	}
}

void UI::SearchFligths(const int mode)
{
	std::vector<Flight> foundFlights;
	const char option1Msg[] = "1.����� �� ������ �����";
	const char option2Msg[] = "2.����� �� ����� ������";
	const char option3Msg[] = "3.����� �� ����� ��������";
	const char option4Msg[] = "Esc.�����";
	const char noFoundMsg[] = "�� ������ �������� ������ �� �������";

	ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(option3Msg, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(option4Msg, CL_WIDTH_LINE, fillCh);

	bool correctInput = true;
	char input;
	do
	{
		correctInput = true;
		input = _getch();
		switch (input)
		{
		case '1':
		{
			int num;
			InputVar(num, 0, UINT_MAX, "����� �����");
			foundFlights = Flight::Search(FlightSearchType::Id, std::to_string(num));
			break;
		}
		case '2':
		{
			std::string location;
			InputString(location, "����� ������");
			foundFlights = Flight::Search(FlightSearchType::LocDeparture, location);
			break;
		}
		case '3':
		{
			std::string location;
			InputString(location, "����� ��������");
			foundFlights = Flight::Search(FlightSearchType::LocArrival, location);
			break;
		}
		case ESC_KEY:
		{
			break;
		}
	defalut:
		correctInput = false;
		break;
		}

	} while (!correctInput);

	if (input != ESC_KEY)
	{
		if (foundFlights.size() < 1)
			ClFomrat::PrintCenteredLine(noFileMsg, CL_WIDTH_LINE, fillCh);
		else
			ViewSpecificFlights(foundFlights, mode);
	}

}

void UI::ViewSpecificFlights(std::vector<Flight> vec, const int mode)
{

}


void UI::SortFlights()
{
	const char option1Msg[] = "1.���������� �� ������ �����";
	const char option2Msg[] = "2.���������� �� ����� ������";
	const char option3Msg[] = "3.���������� �� ����� ��������";
	const char option4Msg[] = "4.���������� �� ���� ������";
	const char option5Msg[] = "Esc.�����";

	ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(option3Msg, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(option4Msg, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(option5Msg, CL_WIDTH_LINE, fillCh);
	bool correctInput;
	do
	{
		correctInput = true;
		char input = _getch();
		switch (input)
		{
		case '1':
			Flight::Sort(FlightSortType::Id);
			break;
		case '2':
			Flight::Sort(FlightSortType::LocDeparture);
			break;
		case '3':
			Flight::Sort(FlightSortType::LocArrival);
			break;
		case '4':
			Flight::Sort(FlightSortType::DateDeparture);
			break;
		default:
			correctInput = false;
			break;
		}
	} while (!correctInput);
}

void UI::AdminAddFlight()
{
	const char acceptMsg[] = "���� ��� �������� � ������";
	const char declineMsg[] = "���� �� ��� �������� � ������";
	Flight newFlight;
	if (newFlight.InputInfo())
	{
		newFlight.PushToVector();
		ClFomrat::PrintCenteredLine(acceptMsg, fillCh);
	}
	else
	{
		ClFomrat::PrintCenteredLine(declineMsg, fillCh);
	}
}

void UI::AdminEditFlight(const int index)
{
	const char option1Msg[] = "1.�������� ����� ������";
	const char option2Msg[] = "2.�������� ����� ��������";
	const char option3Msg[] = "3.�������� ���� ������";
	const char option4Msg[] = "4.�������� ����� ������";
	const char option5Msg[] = "5.�������� ����� �������";
	const char option6Msg[] = "Esc.�����";
	Flight* ptr = Flight::GetFlight(index);
	char input;
	bool done = false;
	while (!done)
	{
		ClFomrat::PrintCenteredLine(option1Msg, fillCh);
		ClFomrat::PrintCenteredLine(option2Msg, fillCh);
		ClFomrat::PrintCenteredLine(option3Msg, fillCh);
		ClFomrat::PrintCenteredLine(option4Msg, fillCh);
		ClFomrat::PrintCenteredLine(option5Msg, fillCh);
		ClFomrat::PrintCenteredLine(option6Msg, fillCh);
		input = _getch();
		switch (input)
		{
		case '1':
		{
			std::string location;
			InputString(location, "����� ������");
			ptr->setLocDeparture(location);
		}
		break;
		case '2':
		{
			std::string location;
			InputString(location, "����� ��������");
			ptr->setLocArrival(location);
			break;
		}
		case '3':
		{
			tm tm;
			InputDate(tm, "���� ������");
			ptr->setDateDepatrue(tm);
			break;
		}
		case '4':
		{
			tm tm;
			InputTime(tm, "����� ������");
			ptr->setTimeDeparture(tm);
			break;
		}
		case '5':
		{
			tm tm;
			InputTime(tm, "����� ��������");
			ptr->setTimeArrival(tm);
		}
		case ESC_KEY:
		{
			done = true;
			break;
		}
		default:
			break;
		}
	}
}
