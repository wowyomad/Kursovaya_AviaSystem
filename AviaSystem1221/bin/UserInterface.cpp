#include "UserInterface.h"
#include "FlightClass.h"
#include "UserConsoleInput.h"
#include "PrintFormat.h"
#include <iomanip>
#include <iostream>
#include <conio.h>
#include <memory>

extern const char PATH_FILE_USERS[];
extern const char PATH_FILE_FLIGHTS[];

static const char ENTER_KEY = 13;
static const char ESC_KEY = 27;

const char escBackMsg[] = "ESC.�����";
const char escExitMsg[] = "ESC.�����";
const char pressKeyMsg[] = "������� ����� ������ ����� ����������.";

const char fillCh = ':';

void UI::StartSystem()
{
	PrintMessage3l("����� ���������� � ��������� ��� ������� �����������");
	PrintMessage3l("��������� ������� ������������� ��� ���������������� �����");
	PrintMessage3l("��������� �� ������������� ��� ������������� �������������");
	PrintMessage3l("��� ����� ����������� ��������� ������");
	PrintMessage3l("���� ��� ���-�� �� ����������, �������� � ����");

	int status = User::GetFileStatus();
	if (status == FileStatus::NotOpened)
	{
		PrintCharLine();
		PrintMessage("���� � ����������������� ������� �� ��� ���������");
		PrintMessage("����� ������ ����� ���� � ����������������� �������");
		PrintMessage("��������� ���� ������ ����� �������������� ��� ����� ��� �����-�������������");
		PrintCharLine();
		std::cout << '\n';
		while (!User::CreateNewUser(AccessLevel::SuperAdmin))
		{
			PrintMessage3l("������������ � ����� ������� ��� ����������");
		}
	}
	else if (status == FileStatus::Empty)
	{
		PrintMessage3l("���� � ����������������� ������� ������");
		PrintMessage3l("��������� ���� ������ ����� �������������� ��� ����� ��� �����-�������������");
		std::cout << '\n';
		while (!User::CreateNewUser(AccessLevel::SuperAdmin))
		{
			PrintMessageNL("������������ � ����� ������� ��� ����������");
		}
		PrintMessage("������������ ��� ������� ��������");
	}
	else
	{
		User::ReadFile();
		bool superAdminExist = User::CheckForSuperAdmin;
		if (!superAdminExist)
		{
			PrintMessage3l("������������ � ������� �����-�������������� ������� �� ����");
			PrintMessage3l("��������� ���� ������ ����� �������������� ��� ����� ��� �����-�������������");
			std::cout << '\n';
			if (User::CreateNewUser(AccessLevel::SuperAdmin))
				PrintMessage("������������ ��� ������� ��������");
		}
	}

	status = Flight::GetFileStatus();
	if (status == FileStatus::NotOpened)
		Flight::CreateNewFile();
	else if (status != FileStatus::Empty)
		Flight::ReadFile();

	PrintMessage(pressKeyMsg);
	_getch();

	Menu_Main();
}

void UI::Menu_Main()
{
	const char mainMsg[] = "������� ����";
	const char option1Msg[] = "1.�����������";
	const char option2Msg[] = "2.�����";
	const char option3Msg[] = "Esc.�����";

	bool done = false;
	char ch;
	while (!done)
	{
		ClearConsole();

		PrintMessage3l("������� ����");

		PrintCharLine();
		PrintMessage("1.�����������");
		PrintMessage("2.�����");
		PrintMessage(escExitMsg);
		PrintCharLine();


		do ch = _getch();
		while ((ch < '1' || ch > '2') && ch != ESC_KEY);
		switch (ch)
		{
		case '1':
			Register();
			break;
		case '2':
			Login();
			break;
		case ESC_KEY:
		{
			Flight::WriteToFile();
			User::WriteToFile();
			done = true;
			break;
		}
		}
	}
}

bool UI::Register(const AccessLevel access)
{
	bool done = false;
	bool registred = false;
	do
	{
		ClearConsole();
		PrintMessage3l("�����������");

		User newUser;
		newUser.InputInfo(access);
		std::string login = newUser.getLogin();
		if (User::loginExist(login) < 0)
		{
			PrintMessage3l("����� �������� ��� �����������");
			PrintMessage3l("����������� ����������");
			if (AcceptAction())
			{
				registred = true;
				newUser.PushToVector();
				PrintMessage3l("������������ ������� ���������������");
				if (access < AccessLevel::Admin)
					PrintMessage3l("�������� ������������� �� ��������������");
				done = true;
			}
			else
			{
				PrintMessage3l("�� �������� �����������");
				PrintMessage3l("������ ��������� �������?");
				if (!AcceptAction()) done = true;
			}
		}
		else
		{
			PrintMessage3l("������ ����� ��� ���������������");
			PrintMessage3l("������ ��������� �������?");
			if (!AcceptAction()) done = true;
		}
	} while (!done);
	PressEnterAction();
	return registred;
}

void UI::Login()
{
	enum AccessLevel access;
	bool done = false;
	User* this_user = nullptr;
	std::string login;
	do
	{
		ClearConsole();
		PrintMessage3l("�����");

		try
		{
			this_user = User::LoginUser(&login);

			if (this_user->getAccessLevel() > AccessLevel::NoAccess)
			{
				done = true;
			}
			else
			{
				if (this_user->getAccessLevel() == AccessLevel::NoAccess)
					PrintMessage3l("������������ ��� �� �����������");
				else PrintMessage3l("�������� ����� ��� ������");
			}
		}
		catch (std::exception& exc)
		{
			PrintMessage3l("������������ �� ������");
		}
		if (!done)
		{
			PrintMessage3l("������ ��������� �������?");
			if (!AcceptAction()) done = true;
		}

	} while (!done);
	if (this_user != nullptr)
		if (this_user->getAccessLevel() > AccessLevel::NoAccess)
		{
			PrintMessage3l("�� ������� ����� � �������");


			void (*Main)(User*);
			switch (this_user->getAccessLevel())
			{
			case AccessLevel::SuperAdmin:
				PrintMessage3l("�� ������ �������������� � ���� �����-������������");
				Main = Main_SuperAdmin;
				break;
			case AccessLevel::Admin:
				PrintMessage3l("�� ������ �������������� � ���� ��������������");
				Main = Main_Admin;
				break;
			case AccessLevel::Client:
				PrintMessage3l("�� ������ �������������� � ���� �������");
				Main = Main_Client;
				break;
			default: throw std::exception(INVALID_USER_ACCESS);
			}
			PrintMessage(pressKeyMsg);
			_getch();

			Main(this_user);
		}
}

void UI::Main_SuperAdmin(User* this_user)
{
	std::vector<Flight*> flightVector;
	Flight::CopyVectorPtr(flightVector);

	std::vector<User*> userVector;
	User::CopyVectorPtr(userVector);


	bool done = false;
	while (!done)
	{
		ClearConsole();
		PrintMessage3l("���� �����-������������");
		std::string helloString = "����� ����������, " + this_user->getLogin();
		PrintMessage3l(helloString.c_str());

		PrintCharLine();
		PrintMessage("1.�������� ������");
		PrintMessage("2.��� ������");
		PrintMessage("3.�������� ��������");
		PrintMessage("4.�������� ��������������");
		PrintMessage(escBackMsg);
		PrintCharLine();

		char input;
		do
		{
			input = _getch();
		} while ((input < '1' || input > '4') && input != ESC_KEY);

		switch (input)
		{
		case '1':
			Menu_Fligths_Admin(this_user, flightVector);
			Flight::CopyVectorPtr(flightVector);
			break;
		case '2':
			Menu_Tickets(this_user);
			break;
		case '3':
			Menu_Users(this_user, userVector);
			break;
		case '4':
			if (Register(AccessLevel::Admin))
				User::CopyVectorPtr(userVector);
			break;
		case ESC_KEY:
			done = true;
			break;
		}
	}
}

void UI::Main_Admin(User* this_user)
{
	std::vector<Flight*> flightVector;
	Flight::CopyVectorPtr(flightVector);

	std::vector<User*> userVector;
	User::CopyVectorPtr(userVector);

	bool done = false;
	while (!done)
	{
		ClearConsole();

		PrintMessage3l("���� ��������������");
		std::string helloString = "����� ����������, " + this_user->getLogin();
		PrintMessage3l(helloString.c_str());

		PrintCharLine();
		PrintMessage("1.�������� ������");
		PrintMessage("2.��� ������");
		PrintMessage("3.�������� ��������");
		PrintMessage(escBackMsg);
		PrintCharLine();

		char input;
		do
		{
			input = _getch();
		} while ((input < '1' || input > '3') && input != ESC_KEY);

		switch (input)
		{
		case '1':
			Menu_Fligths_Admin(this_user, flightVector);
			break;
		case '2':
			Menu_Tickets(this_user);
			break;
		case '3':
			Menu_Users(this_user, userVector);
			break;
		case ESC_KEY:
			done = true;
			break;
		}
	}
}

void UI::Main_Client(User* this_user)
{
	std::vector<Flight*> flightVector;
	Flight::CopyVectorPtr(flightVector);

	bool done = false;
	while (!done)
	{
		ClearConsole();

		PrintMessage3l("���� �������");
		std::string helloString = "����� ����������, " + this_user->getLogin();
		PrintMessage3l(helloString.c_str());

		PrintCharLine();
		PrintMessage("1.������ ������");
		PrintMessage("2.��� ������");
		PrintMessage(escBackMsg);
		PrintCharLine();
		char input;
		do
		{
			input = _getch();
		} while (input < '1' && input > '2' && input != ESC_KEY);

		switch (input)
		{
		case '1':
			Menu_Fligths_Client(this_user, flightVector);
			break;
		case '2':
			Menu_Tickets(this_user);
			break;
		case ESC_KEY:
			done = true;
			break;
		}
	}
}

void UI::Menu_Fligths_Admin(User* this_user, std::vector<Flight*>& flightVector)
{
	bool done = false;
	char input = 0;
	size_t number = 0;

	while (!done)
	{
		ClearConsole();
		PrintMessage3l("������ ������");

		if (Flight::getFlightCount() < 1)
			PrintMessage3l("������ ���");
		else Flight::PrintInfoVector(flightVector);

		PrintCharLine();
		PrintMessage("1.�������");
		PrintMessage("2.����������");
		PrintMessage("3.�����");
		PrintMessage("4.��������");
		PrintMessage(escBackMsg);
		PrintCharLine();

		do input = _getch();
		while ((input < '1' || input > '4') && input != ESC_KEY);

		if (Flight::getFlightCount() < 1)
		{
			if (input != '4')
				continue;
		}

		switch (input)
		{
		case '1':
			InputVar(number, 1, flightVector.size(), "�����: ");
			Action_ViewFlight_Admin(this_user, flightVector[number - 1]);
			Flight::CopyVectorPtr(flightVector);
			break;
		case '2':
			Action_SortFlights(flightVector);
			break;
		case '3':
		{
			Action_SearchFligths_Admin(this_user, flightVector);
			Flight::CopyVectorPtr(flightVector);
			break;
		}
		case '4':
			Action_AddFlight(flightVector);
			break;
		case ESC_KEY:
			done = true;
			break;
		}
	}
}

void UI::Menu_Fligths_Client(User* this_user, std::vector<Flight*>& flightVector)
{
	bool done = false;
	char input = 0;
	size_t number = 0;

	while (!done)
	{

		ClearConsole();
		if (Flight::getFlightCount() < 1)
			PrintMessage3l("������ ���");
		Flight::PrintInfoVector(flightVector);

		PrintCharLine();
		PrintMessage("1.�������");
		PrintMessage("2.����������");
		PrintMessage("3.�����");
		PrintMessage(escBackMsg);
		PrintCharLine();

		do input = _getch();
		while ((input < '1' || input > '5') && input != ESC_KEY);

		switch (input)
		{
		case '1':
			InputVar(number, 1, flightVector.size(), "�����: ");
			Action_ViewFlight_Client(this_user, flightVector[number - 1]);
			break;
		case '2':
			Action_SortFlights(flightVector);
			break;
		case '3':
		{
			Action_SearchFlights_Client(this_user, flightVector);
			Flight::CopyVectorPtr(flightVector);
			break;
		}
		case ESC_KEY:
			done = true;
			break;
		}
	}
}

void UI::Menu_CustomFligths_Admin(User* this_user, std::vector<Flight*>& flightVector)
{

	bool done = false;
	char input = 0;
	size_t number = 0;

	while (!done)
	{
		ClearConsole();

		PrintMessage3l("��������� ����� �� ������ �������");

		if (Flight::getFlightCount() < 1)
		{
			PrintMessage3l("�� ������ ������� ������ �� �������");
			PressEnterAction();
			return;
		}
		Flight::PrintInfoVector(flightVector);

		PrintCharLine();
		PrintMessage("1.�������");
		PrintMessage(escBackMsg);
		PrintCharLine();
		std::cout << '\n';

		do input = _getch();
		while (input != '1' && input != ESC_KEY);

		switch (input)
		{
		case '1':
			InputVar(number, 1, flightVector.size(), "�����: ");
			Action_ViewFlight_Admin(this_user, flightVector[number - 1]);
			done = true;
			break;
		case ESC_KEY:
			done = true;
			break;
		}
		ClearConsole();
	}
}

void UI::Menu_CustomFligths_Client(User* this_user, std::vector<Flight*>& flightVector)
{
	bool done = false;
	char input = 0;
	size_t number = 0;

	while (!done)
	{
		ClearConsole();

		PrintMessage3l("��������� ����� �� ������ �������");

		if (Flight::getFlightCount() < 1)
		{
			PrintMessage3l("�� ������ ������� ������ �� �������");
			PressEnterAction();
			return;
		}

		if (Flight::getFlightCount() < 1)
			PrintMessage("�� ������ ������� ������ �� �������");
		Flight::PrintInfoVector(flightVector);

		PrintCharLine();
		PrintMessage("1.�������");
		PrintMessage(escBackMsg);
		PrintCharLine();
		std::cout << '\n';

		do input = _getch();
		while (input != '1' && input != ESC_KEY);

		switch (input)
		{
		case '1':
			InputVar(number, 1, flightVector.size(), "�����: ");
			Action_ViewFlight_Client(this_user, flightVector[number - 1]);
			done = true;
			break;
		case ESC_KEY:
			done = true;
			break;
		}
		ClearConsole();
	}
}

void UI::Menu_Tickets(User* this_user)
{
	ClearConsole();
	PrintMessage3l("��� ������");
	if (this_user->getTicketCount() < 1)
	{
		PrintMessage3l("� ��� ��� �������");
	}
	else
	{
		ClFomrat::PrintCenteredLine("���� ������", CL_WIDTH_LINE);
		this_user->ShowTickets();
	}
	PressEnterAction();
}

void UI::Menu_Users(User* this_user, std::vector<User*>& userVector)
{
	ClearConsole();

	bool done = false;
	char input;
	size_t number = 0;
	while (!done)
	{
		ClearConsole();

		PrintMessage3l("������ �������������");

		User::PrintInfoVector(userVector);

		PrintCharLine();
		PrintMessage("1.�������");
		PrintMessage("2.��������");
		PrintMessage(escBackMsg);
		PrintCharLine();

		do input = _getch();
		while ((input < '1' || input > '2') && input != ESC_KEY);

		switch (input)
		{
		case '1':
			if (userVector.empty()) break;
			InputVar(number, 1, userVector.size(), "�����: ");
			Action_ViewUser(this_user, userVector, number - 1);
			break;
		case '2':
			if (Register(AccessLevel::Client))
				User::CopyVectorPtr(userVector);
			break;
		case ESC_KEY:
			done = true;
			break;
		}
		User::CopyVectorPtr(userVector);
	}
}

void UI::Action_BookTicket(User* this_user, Flight* flight)
{
	bool done = false;
	char input;
	int amount = 0;
	int chosenType = 0;

	while (!done)
	{
		ClearConsole();

		PrintMessage3l("������������ ������");

		flight->PrintInfoWithTop();

		if (flight->getTicketCount(Business) + flight->getTicketCount(Economy) < 1)
		{
			PrintMessage3l("�� ������ ���� ��� �������");
			PressEnterAction();
			ClearConsole();
			return;
		}

		PrintCharLine();
		PrintMessage("1.������ �����");
		PrintMessage("2.������-�����");
		PrintMessage(escBackMsg);
		PrintCharLine();

		do input = _getch();
		while ((input < '1' || input > '2') && input != ESC_KEY);

		if (input == ESC_KEY)
		{
			ClearConsole();
			return;
		}

		chosenType = input - '0';

		if (flight->TicketAvailable(chosenType, 1))
		{
			InputVar(amount, 1, flight->getTicketCount(chosenType), "���������� �������: ");
			if (AcceptAction())
			{
				this_user->BookTicket(flight, chosenType, amount);
				PrintMessage3l("�� ������� ������������� ������");
			}
			else PrintMessage3l("�� �������� ������������ �������");
		}
		else
		{
			PrintMessage3l("������� ������� ���� ���");
			PrintMessage3l("�� ������ ������� ������ �����");
		}
		PressEnterAction();
	}
}

void UI::Action_AcceptAll_Admin(std::vector<User*>& clientVector)
{
	PrintMessage3l("����������� ���� �������������?");
	if (AcceptAction())
	{
		User::AcceptAll();
		PrintMessage3l("��� ������������ ���� ������������");
	}
	else PrintMessage3l("�� ���������� �� �������������");
	PressEnterAction();

}

bool UI::Action_RemoveUser(User* user, const int index)
{
	std::string login = user->getLogin();

	PrintMessage3l("�������� ������������");

	if (AcceptAction())
	{
		user->RemoveUser(index);
		PrintMessage("������������ ������� ������");
		return true;
	}
	else PrintMessage("������������ �� ��� ������");
	return false;
}

bool UI::Action_RemoveFlight(Flight* flightPtr)
{
	bool done = false;
	char input;
	bool isRemoved = false;

	ClearConsole();

	PrintMessage3l("�������� �����");

	if (flightPtr->getPassangerCount() < 1)
	{
		PrintMessage3l("����������� ��������");

		if (AcceptAction())
		{
			int index = Flight::GetFlightIndex(flightPtr);
			Flight::RemoveFlight(index);
			PrintMessage3l("���� ��� ������");
			isRemoved = true;
		}
		else PrintMessage3l("���� �� ��� ������");
	}
	else
		PrintMessage3l("���������� ������� �����, �� ������� ���� ������� ������");

	PressEnterAction();
	return isRemoved;
}

bool UI::Action_EditFlight(Flight* flight)
{
	bool done = false;
	char input;
	bool isEdited = true;

	Flight flightCopy = *flight;

	std::string str;
	int var;
	tm tm;

	while (!done)
	{
		ClearConsole();

		PrintMessage3l("�������������� ����������");

		flight->PrintInfoWithTop();

		PrintCharLine();
		PrintMessage("1.�������� ����� ������");
		PrintMessage("2.�������� ����� ��������");
		PrintMessage("3.�������� ���� ������");
		PrintMessage("4.�������� ����� ������");
		PrintMessage("5.�������� ����� ��������");
		PrintMessage("6.�������� ���������� �������");
		PrintMessage(escBackMsg);
		PrintCharLine();
		std::cout << '\n';

		do input = _getch();
		while ((input < '1' || input > '6') && input != ESC_KEY);

		switch (input)
		{
		case '1':
			InputString(str, "����� ������: ");
			if (AcceptAction()) flight->setLocDeparture(str);
			else PrintMessage3l("��������� ������� �� ����");
			break;
		case '2':
			InputString(str, "����� ��������: ");
			if (AcceptAction()) flight->setLocArrival(str);
			else PrintMessage3l("��������� ������� �� ����");
			break;
		case '3':
			InputDate(tm, "���� ������: ");
			if (AcceptAction()) flight->setDateDepatrue(tm);
			else PrintMessage3l("��������� ������� �� ����");
			break;
		case '4':
			InputTime(tm, "����� ������: ");
			if (AcceptAction()) flight->setTimeDeparture(tm);
			else PrintMessage3l("��������� ������� �� ����");
			break;
		case '5':
			InputTime(tm, "����� ��������: ");
			if (AcceptAction()) flight->setTimeArrival(tm);
			else PrintMessage3l("��������� ������� �� ����");
			break;
		case '6':
			PrintMessage3l("������ ������� �� ��������");
			PrintMessage3l("��������� ������� �� ����");
			break;
		case ESC_KEY:
			ClearConsole();
			PrintMessage3l("������� ����������");
			flightCopy.PrintInfoWithTop();
			PrintMessage3l("����� �����������");
			flight->PrintInfoWithTop();
			if (AcceptAction())
				PrintMessage3l("��������� ���� ��������");
			else
			{
				*flight = flightCopy;
				PrintMessage3l("��� ��������� ���� ��������");
				isEdited = false;
			}
			done = true;
			break;
		}

		ClearConsole();
	}
	return isEdited;
}

bool UI::Action_AddUser(std::vector<User*>& userVector)
{
	if (User::CreateNewUser(AccessLevel::Client))
	{
		PrintMessage3l("����� ������������ ��� ������� ��������");
		return true;
	}
	else PrintMessage3l("������������ � ����� ������� ��� ����������");
}

void UI::Action_SortFlights(std::vector<Flight*>& vectorFlight)
{
	ClearConsole();

	PrintMessage3l("���������� ������ ������");

	PrintCharLine();
	PrintMessage("1.���������� �� ������ �����");
	PrintMessage("2.���������� �� ����� ������");
	PrintMessage("3.���������� �� ���� ������");
	PrintMessage("4.���������� �� ����� ��������");
	PrintMessage(escBackMsg);
	PrintCharLine();

	char input;
	do input = _getch();
	while ((input < '1' || input > '4') && input != ESC_KEY);

	switch (input)
	{
	case '1':
		Flight::SortById(vectorFlight);
		break;
	case '2':
		Flight::SortByDep(vectorFlight);
		break;
	case '3':
		Flight::SortByDate(vectorFlight);
		break;
	case '4':
		Flight::SortByArr(vectorFlight);
		break;
	case ESC_KEY:
		break;
	}
}

void UI::Action_SearchFligths_Admin(User* this_user, std::vector<Flight*>& flightVector)
{
	char input;
	std::string data;
	std::vector<Flight*> flightVector2;

	ClearConsole();

	PrintMessage3l("����� ������");

	PrintCharLine();
	PrintMessage("1.����� �� ����� ������");
	PrintMessage("2.����� �� ����� ��������");
	PrintMessage("3.����� �� ������ �����");
	PrintMessage(escBackMsg);
	PrintCharLine();
	std::cout << '\n';

	do input = _getch();
	while ((input < '1' || input > '3') && input != ESC_KEY);

	switch (input)
	{
	case '1':
		InputString(data, "����� ������: ");
		Flight::SearchByDep(flightVector, flightVector2, data);
		break;
	case '2':
		InputString(data, "����� ��������: ");
		Flight::SearchByArr(flightVector, flightVector2, data);

		break;
	case '3':
		InputString(data, "����� �����: ");
		Flight::SearchById(flightVector, flightVector2, data);
		break;
	}

	if (input != ESC_KEY)
		Menu_CustomFligths_Admin(this_user, flightVector2);

	ClearConsole();
}

void UI::Action_SearchFlights_Client(User* this_user, std::vector<Flight*>& flightVector)
{
	char input;
	std::string data;
	std::vector<Flight*> flightVector2;

	ClearConsole();

	PrintMessage3l("����� ������");

	PrintCharLine();
	PrintMessage("1.����� �� ����� ������");
	PrintMessage("2.����� �� ����� ��������");
	PrintMessage("3.����� �� ������ �����");
	PrintMessage(escBackMsg);
	PrintCharLine();
	std::cout << '\n';

	do input = _getch();
	while ((input < '1' || input > '3') && input != ESC_KEY);

	switch (input)
	{
	case '1':
		InputString(data, "����� ������: ");
		Flight::SearchByDep(flightVector, flightVector2, data);
		break;
	case '2':
		InputString(data, "����� ��������: ");
		Flight::SearchByArr(flightVector, flightVector2, data);
		break;
	case '3':
		InputString(data, "����� �����: ");
		Flight::SearchById(flightVector, flightVector2, data);
		break;
	}
	if (input != ESC_KEY)
		Menu_CustomFligths_Client(this_user, flightVector2);

	ClearConsole();
}

void UI::Action_AddFlight(std::vector<Flight*>& flightVector)
{
	ClearConsole();

	PrintMessage3l("������������ ������ �����");

	Flight newFlight;
	newFlight.InputInfo();
	newFlight.PrintInfoWithTop();
	PrintMessage3l("����� ����� ����� ������������ ��� ���������� � ������");
	if (AcceptAction())
	{
		newFlight.PushToVector();
		PrintMessage("����� ���� ��� ��������");
		Flight::CopyVectorPtr(flightVector);
	}
	else PrintMessage("����� ���� �� ���� ��������");

	PressEnterAction();
}

void UI::Action_ViewFlight_Admin(User* this_user, Flight* flight)
{
	bool done = false;
	char input;


	while (!done)
	{
		ClearConsole();

		PrintMessage3l("�������� �����");

		flight->PrintInfoWithTop();
		PrintCharLine();
		std::cout << '\n';

		PrintMessage("���������", ' ');
		if (flight->getPassangerCount() < 1)
			PrintMessage3l("���������� ���");
		else
		{
			flight->PrintPassangers();
			PrintCharLine();
		}

		PrintCharLine();
		PrintMessage("1.�������������");
		PrintMessage("2.�������������");
		PrintMessage("3.�������");
		PrintMessage(escBackMsg);
		PrintCharLine();

		do input = _getch();
		while ((input < '1' || input > '3') && input != ESC_KEY);

		switch (input)
		{
		case '1':
			Action_BookTicket(this_user, flight);
			break;
		case '2':
			Action_EditFlight(flight);
			break;
		case '3':
			if (Action_RemoveFlight(flight))
				done = true;
			break;
		case ESC_KEY:
			done = true;
			break;
		}
	}
}

void UI::Action_ViewFlight_Client(User* this_user, Flight* flight)
{

	bool done = false;
	char input;

	while (!done)
	{
		ClearConsole();

		PrintMessage3l("�������� �����");

		flight->PrintInfoWithTop();

		PrintCharLine();
		PrintMessage("1.�������������");
		PrintMessage(escBackMsg);
		PrintCharLine();

		do input = _getch();
		while ((input < '1' || input > '2') && input != ESC_KEY);

		switch (input)
		{
		case '1':
			Action_BookTicket(this_user, flight);
			break;
		case ESC_KEY:
			done = true;
			break;
		}
	}
}


void UI::Action_ViewUser(User* this_user, std::vector<User*>& userVector, const size_t index)
{
	ClearConsole();

	bool done = false;
	char input;
	std::string str;
	if (userVector[index]->getAccessLevel() >= this_user->getAccessLevel())
	{
		PrintMessage3l("�� �� ������ �������������� ������ ������� ������");
		PressEnterAction();
		ClearConsole();
		return;
	}
	while (!done)
	{
		PrintMessage3l("�������� ������������");

		char maxInput = userVector[index]->getAccessLevel() == AccessLevel::NoAccess ? '5' : '4';

		userVector[index]->PrintInfoWithTop();

		PrintCharLine();
		PrintMessage("1.����������� ������");
		PrintMessage("2.�������� �����");
		PrintMessage("3.�������� ������");
		PrintMessage("4.�������");
		if (maxInput == '5') PrintMessage("5.����������� �������");
		PrintMessage(escBackMsg);
		PrintCharLine();

		do input = _getch();
		while ((input < '1' || input > maxInput) && input != ESC_KEY);

		switch (input)
		{
		case '1':
			Action_ViewClientTickets(userVector[index]);
			break;
		case '2':
			Action_ChangeLogin(userVector[index]);
			break;
		case '3':
			Action_ChangePassword(userVector[index]);
			break;
		case '4':
		{
			if (userVector[index]->getLogin() == this_user->getLogin())
				PrintMessage3l("������ ������� ����");
			else if (Action_RemoveUser(userVector[index], index))
			{
				User::CopyVectorPtr(userVector);
				done = true;
			}
			break;
		}
		case '5':
			PrintMessage3l("�� �������, ��� ������ ����������� ������������?");
			if (AcceptAction())
				userVector[index]->setAccessLevel(AccessLevel::Client);
			break;
		case ESC_KEY:
			done = true;
			break;
		}

		ClearConsole();
	}
}

void UI::Action_ChangePassword(User* user)
{
	ClearConsole();

	PrintMessage3l("��������� ������");

	std::string newPassword;
	InputPassword(newPassword, 8, 32);
	if (AcceptAction)
	{
		user->ChangePassword(newPassword);
		PrintMessage3l("����� ������� �������");
	}
	else PrintMessage3l("������ �� ��� �������");
}

void UI::Action_ChangeLogin(User* user)
{
	ClearConsole();

	PrintMessage3l("��������� ������");

	std::string newLogin;
	InputLogin(newLogin, 3, 32);
	if (AcceptAction)
	{
		user->setLogin(newLogin);
		PrintMessage3l("����� ������� �������");
	}
	else PrintMessage3l("����� �� ��� �������");
}

void UI::Action_ViewClientTickets(User* user)
{
	ClearConsole();

	PrintMessage3l("�������� ������� ������������");

	if (user->getTicketCount() < 1)
	{
		PrintMessage3l("� ������������ ��� �������");
	}
	else
	{
		ClFomrat::PrintCenteredLine("������ ������������", CL_WIDTH_LINE);
		user->ShowTickets();
	}
	PressEnterAction();
}

bool UI::AcceptAction()
{
	PrintMessage("������� ENTER, ����� �����������, ��� ESC ����� ��������");
	char input;
	do input = _getch();
	while (input != ENTER_KEY && input != ESC_KEY);
	return input == ENTER_KEY ? true : false;
}

void UI::PressEnterAction()
{
	ClFomrat::PrintCenteredLine("������� ENTER, ����� ����������", CL_WIDTH_LINE, fillCh);
	char input;
	do input = _getch();
	while (input != ENTER_KEY);
}


void UI::PrintMessage(const char* msg, const char fill)
{
	ClFomrat::PrintCenteredLine(msg, CL_WIDTH_LINE, fill);
}

void UI::PrintMessageNL(const char* msg)
{
	std::cout << '\n';
	ClFomrat::PrintCenteredLine(msg, CL_WIDTH_LINE, fillCh);
	std::cout << '\n';
}

void UI::PrintMessage3l(const char* str, const char ch)
{
	PrintCharLine(ch);
	PrintMessage(str);
	PrintCharLine(ch);

}

void UI::PrintCharLine(char ch)
{
	std::cout << std::setfill('-') << std::setw(CL_WIDTH_LINE) << '\n';
}
