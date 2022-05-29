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

const char escBackMsg[] = "ESC.Назад";
const char escExitMsg[] = "ESC.Выйти";
const char pressKeyMsg[] = "Нажмите любую кнопку чтобы продолжить.";

const char fillCh = ':';

void UI::StartSystem()
{
	PrintMessage3l("Добро пожаловать в программу для продажи авиабилетов");
	PrintMessage3l("Программа создана исключительно для демонстрационных целей");
	PrintMessage3l("Программа не предназначена для коммерческого использования");
	PrintMessage3l("Все права принадлежат Сундукову Вадиму");
	PrintMessage3l("Если вас что-то не устраивает, увидимся в суде");

	int status = User::GetFileStatus();
	if (status == FileStatus::NotOpened)
	{
		PrintCharLine();
		PrintMessage("Файл с пользовательскими данными не был обнаружен");
		PrintMessage("Будет создан новый файл с пользовательскими данными");
		PrintMessage("Введенные вами данные будут использоваться для входа как супер-администратор");
		PrintCharLine();
		std::cout << '\n';
		while (!User::CreateNewUser(AccessLevel::SuperAdmin))
		{
			PrintMessage3l("Пользователь с таким логином уже существует");
		}
	}
	else if (status == FileStatus::Empty)
	{
		PrintMessage3l("Файл с пользовательскими данными пустой");
		PrintMessage3l("Введенные вами данные будут использоваться для входа как супер-администратор");
		std::cout << '\n';
		while (!User::CreateNewUser(AccessLevel::SuperAdmin))
		{
			PrintMessageNL("Пользователь с таким логином уже существует");
		}
		PrintMessage("Пользователь был успешно добавлен");
	}
	else
	{
		User::ReadFile();
		bool superAdminExist = User::CheckForSuperAdmin;
		if (!superAdminExist)
		{
			PrintMessage3l("Пользователя с правами супер-администратора найдено не было");
			PrintMessage3l("Введенные вами данные будут использоваться для входа как супер-администратор");
			std::cout << '\n';
			if (User::CreateNewUser(AccessLevel::SuperAdmin))
				PrintMessage("Пользователь был успешно добавлен");
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
	const char mainMsg[] = "Главное меню";
	const char option1Msg[] = "1.Регистрация";
	const char option2Msg[] = "2.Логин";
	const char option3Msg[] = "Esc.Выйти";

	bool done = false;
	char ch;
	while (!done)
	{
		ClearConsole();

		PrintMessage3l("Главное меню");

		PrintCharLine();
		PrintMessage("1.Регистрация");
		PrintMessage("2.Логин");
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
		PrintMessage3l("Регистрация");

		User newUser;
		newUser.InputInfo(access);
		std::string login = newUser.getLogin();
		if (User::loginExist(login) < 0)
		{
			PrintMessage3l("Логин доступен для регистрации");
			PrintMessage3l("Подтвердите регитрацию");
			if (AcceptAction())
			{
				registred = true;
				newUser.PushToVector();
				PrintMessage3l("Пользователь успешно зарегистрирован");
				if (access < AccessLevel::Admin)
					PrintMessage3l("Ожидайте подтверждения от администратора");
				done = true;
			}
			else
			{
				PrintMessage3l("Вы отменили регистрацию");
				PrintMessage3l("Хотите повторить попытку?");
				if (!AcceptAction()) done = true;
			}
		}
		else
		{
			PrintMessage3l("Данный логин уже зарегистрирован");
			PrintMessage3l("Хотите повторить попытку?");
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
		PrintMessage3l("Логин");

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
					PrintMessage3l("Пользователь ещё не подтвержден");
				else PrintMessage3l("Неверный логин или пароль");
			}
		}
		catch (std::exception& exc)
		{
			PrintMessage3l("Пользователь не найден");
		}
		if (!done)
		{
			PrintMessage3l("Хотите повторить попытку?");
			if (!AcceptAction()) done = true;
		}

	} while (!done);
	if (this_user != nullptr)
		if (this_user->getAccessLevel() > AccessLevel::NoAccess)
		{
			PrintMessage3l("Вы успешно вошли в аккаунт");


			void (*Main)(User*);
			switch (this_user->getAccessLevel())
			{
			case AccessLevel::SuperAdmin:
				PrintMessage3l("Вы будете перенаправлены в меню супер-пользователя");
				Main = Main_SuperAdmin;
				break;
			case AccessLevel::Admin:
				PrintMessage3l("Вы будете перенаправлены в меню администратора");
				Main = Main_Admin;
				break;
			case AccessLevel::Client:
				PrintMessage3l("Вы будете перенаправлены в меню клиента");
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
		PrintMessage3l("Меню супер-пользователя");
		std::string helloString = "Добро пожаловать, " + this_user->getLogin();
		PrintMessage3l(helloString.c_str());

		PrintCharLine();
		PrintMessage("1.Просмотр рейсов");
		PrintMessage("2.Мои билеты");
		PrintMessage("3.Просмотр клиентов");
		PrintMessage("4.Добавить администратора");
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

		PrintMessage3l("Меню администратора");
		std::string helloString = "Добро пожаловать, " + this_user->getLogin();
		PrintMessage3l(helloString.c_str());

		PrintCharLine();
		PrintMessage("1.Просмотр рейсов");
		PrintMessage("2.Мои билеты");
		PrintMessage("3.Просмотр клиентов");
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

		PrintMessage3l("Меню клиента");
		std::string helloString = "Добро пожаловать, " + this_user->getLogin();
		PrintMessage3l(helloString.c_str());

		PrintCharLine();
		PrintMessage("1.Список рейсов");
		PrintMessage("2.Мои билеты");
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
		PrintMessage3l("Список рейсов");

		if (Flight::getFlightCount() < 1)
			PrintMessage3l("Рейсов нет");
		else Flight::PrintInfoVector(flightVector);

		PrintCharLine();
		PrintMessage("1.Выбрать");
		PrintMessage("2.Сортировка");
		PrintMessage("3.Поиск");
		PrintMessage("4.Добавить");
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
			InputVar(number, 1, flightVector.size(), "Номер: ");
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
			PrintMessage3l("Рейсов нет");
		Flight::PrintInfoVector(flightVector);

		PrintCharLine();
		PrintMessage("1.Выбрать");
		PrintMessage("2.Сортировка");
		PrintMessage("3.Поиск");
		PrintMessage(escBackMsg);
		PrintCharLine();

		do input = _getch();
		while ((input < '1' || input > '5') && input != ESC_KEY);

		switch (input)
		{
		case '1':
			InputVar(number, 1, flightVector.size(), "Номер: ");
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

		PrintMessage3l("Найденные рейсы по вашему запросу");

		if (Flight::getFlightCount() < 1)
		{
			PrintMessage3l("По вашему запросу ничего не найдено");
			PressEnterAction();
			return;
		}
		Flight::PrintInfoVector(flightVector);

		PrintCharLine();
		PrintMessage("1.Выбрать");
		PrintMessage(escBackMsg);
		PrintCharLine();
		std::cout << '\n';

		do input = _getch();
		while (input != '1' && input != ESC_KEY);

		switch (input)
		{
		case '1':
			InputVar(number, 1, flightVector.size(), "Номер: ");
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

		PrintMessage3l("Найденные рейсы по вашему запросу");

		if (Flight::getFlightCount() < 1)
		{
			PrintMessage3l("По вашему запросу ничего не найдено");
			PressEnterAction();
			return;
		}

		if (Flight::getFlightCount() < 1)
			PrintMessage("По вашему запросу ничего не найдено");
		Flight::PrintInfoVector(flightVector);

		PrintCharLine();
		PrintMessage("1.Выбрать");
		PrintMessage(escBackMsg);
		PrintCharLine();
		std::cout << '\n';

		do input = _getch();
		while (input != '1' && input != ESC_KEY);

		switch (input)
		{
		case '1':
			InputVar(number, 1, flightVector.size(), "Номер: ");
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
	PrintMessage3l("Мои билеты");
	if (this_user->getTicketCount() < 1)
	{
		PrintMessage3l("У вас нет билетов");
	}
	else
	{
		ClFomrat::PrintCenteredLine("Ваши билеты", CL_WIDTH_LINE);
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

		PrintMessage3l("Список пользователей");

		User::PrintInfoVector(userVector);

		PrintCharLine();
		PrintMessage("1.Выбрать");
		PrintMessage("2.Добавить");
		PrintMessage(escBackMsg);
		PrintCharLine();

		do input = _getch();
		while ((input < '1' || input > '2') && input != ESC_KEY);

		switch (input)
		{
		case '1':
			if (userVector.empty()) break;
			InputVar(number, 1, userVector.size(), "Номер: ");
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

		PrintMessage3l("Бронирование билета");

		flight->PrintInfoWithTop();

		if (flight->getTicketCount(Business) + flight->getTicketCount(Economy) < 1)
		{
			PrintMessage3l("На данный рейс нет билетов");
			PressEnterAction();
			ClearConsole();
			return;
		}

		PrintCharLine();
		PrintMessage("1.Эконом класс");
		PrintMessage("2.Бизнес-класс");
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
			InputVar(amount, 1, flight->getTicketCount(chosenType), "Количество билетов: ");
			if (AcceptAction())
			{
				this_user->BookTicket(flight, chosenType, amount);
				PrintMessage3l("Вы успешно забронировали билеты");
			}
			else PrintMessage3l("Вы отменили бронирование билетов");
		}
		else
		{
			PrintMessage3l("Билетов данного типа нет");
			PrintMessage3l("Вы можете выбрать другой класс");
		}
		PressEnterAction();
	}
}

void UI::Action_AcceptAll_Admin(std::vector<User*>& clientVector)
{
	PrintMessage3l("Подтвердить всех пользователей?");
	if (AcceptAction())
	{
		User::AcceptAll();
		PrintMessage3l("Все пользователи были подтверждены");
	}
	else PrintMessage3l("Вы отказались от подтверждения");
	PressEnterAction();

}

bool UI::Action_RemoveUser(User* user, const int index)
{
	std::string login = user->getLogin();

	PrintMessage3l("Удаление пользователя");

	if (AcceptAction())
	{
		user->RemoveUser(index);
		PrintMessage("Пользователь успешно удален");
		return true;
	}
	else PrintMessage("Пользователь не был удален");
	return false;
}

bool UI::Action_RemoveFlight(Flight* flightPtr)
{
	bool done = false;
	char input;
	bool isRemoved = false;

	ClearConsole();

	PrintMessage3l("Удаление рейса");

	if (flightPtr->getPassangerCount() < 1)
	{
		PrintMessage3l("Подтвердите удаление");

		if (AcceptAction())
		{
			int index = Flight::GetFlightIndex(flightPtr);
			Flight::RemoveFlight(index);
			PrintMessage3l("Рейс был удален");
			isRemoved = true;
		}
		else PrintMessage3l("Рейс не был удален");
	}
	else
		PrintMessage3l("Невозможно удалить рейсы, на которые были куплены билеты");

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

		PrintMessage3l("Редактирование информации");

		flight->PrintInfoWithTop();

		PrintCharLine();
		PrintMessage("1.Изменить место вылета");
		PrintMessage("2.Изменить место прибытия");
		PrintMessage("3.Изменить дату вылета");
		PrintMessage("4.Изменить время вылета");
		PrintMessage("5.Изменить время прибытия");
		PrintMessage("6.Изменить количество билетов");
		PrintMessage(escBackMsg);
		PrintCharLine();
		std::cout << '\n';

		do input = _getch();
		while ((input < '1' || input > '6') && input != ESC_KEY);

		switch (input)
		{
		case '1':
			InputString(str, "Место вылета: ");
			if (AcceptAction()) flight->setLocDeparture(str);
			else PrintMessage3l("Изменений внесено не было");
			break;
		case '2':
			InputString(str, "Место прибытия: ");
			if (AcceptAction()) flight->setLocArrival(str);
			else PrintMessage3l("Изменений внесено не было");
			break;
		case '3':
			InputDate(tm, "Дата вылета: ");
			if (AcceptAction()) flight->setDateDepatrue(tm);
			else PrintMessage3l("Изменений внесено не было");
			break;
		case '4':
			InputTime(tm, "Время вылета: ");
			if (AcceptAction()) flight->setTimeDeparture(tm);
			else PrintMessage3l("Изменений внесено не было");
			break;
		case '5':
			InputTime(tm, "Время прибытия: ");
			if (AcceptAction()) flight->setTimeArrival(tm);
			else PrintMessage3l("Изменений внесено не было");
			break;
		case '6':
			PrintMessage3l("Данная функция не рабоатет");
			PrintMessage3l("Изменений внесено не было");
			break;
		case ESC_KEY:
			ClearConsole();
			PrintMessage3l("Прошлая информация");
			flightCopy.PrintInfoWithTop();
			PrintMessage3l("Новая информациия");
			flight->PrintInfoWithTop();
			if (AcceptAction())
				PrintMessage3l("Информция была изменена");
			else
			{
				*flight = flightCopy;
				PrintMessage3l("Все изменения были отменены");
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
		PrintMessage3l("Новый пользователь был успешно добавлен");
		return true;
	}
	else PrintMessage3l("Пользователь с таким логином уже существует");
}

void UI::Action_SortFlights(std::vector<Flight*>& vectorFlight)
{
	ClearConsole();

	PrintMessage3l("Сортировка списка рейсов");

	PrintCharLine();
	PrintMessage("1.Сортировка по номеру рейса");
	PrintMessage("2.Сортировка по месту вылета");
	PrintMessage("3.Сортировку по дате вылета");
	PrintMessage("4.Сортировка по месту прибытия");
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

	PrintMessage3l("Поиск рейсов");

	PrintCharLine();
	PrintMessage("1.Поиск по месту вылета");
	PrintMessage("2.Поиск по месту прибытия");
	PrintMessage("3.Поиск по номеру рейса");
	PrintMessage(escBackMsg);
	PrintCharLine();
	std::cout << '\n';

	do input = _getch();
	while ((input < '1' || input > '3') && input != ESC_KEY);

	switch (input)
	{
	case '1':
		InputString(data, "Место вылета: ");
		Flight::SearchByDep(flightVector, flightVector2, data);
		break;
	case '2':
		InputString(data, "Место прибытия: ");
		Flight::SearchByArr(flightVector, flightVector2, data);

		break;
	case '3':
		InputString(data, "Номер рейса: ");
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

	PrintMessage3l("Поиск рейсов");

	PrintCharLine();
	PrintMessage("1.Поиск по месту вылета");
	PrintMessage("2.Поиск по месту прибытия");
	PrintMessage("3.Поиск по номеру рейса");
	PrintMessage(escBackMsg);
	PrintCharLine();
	std::cout << '\n';

	do input = _getch();
	while ((input < '1' || input > '3') && input != ESC_KEY);

	switch (input)
	{
	case '1':
		InputString(data, "Место вылета: ");
		Flight::SearchByDep(flightVector, flightVector2, data);
		break;
	case '2':
		InputString(data, "Место прибытия: ");
		Flight::SearchByArr(flightVector, flightVector2, data);
		break;
	case '3':
		InputString(data, "Номер рейса: ");
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

	PrintMessage3l("Добававление нового рейса");

	Flight newFlight;
	newFlight.InputInfo();
	newFlight.PrintInfoWithTop();
	PrintMessage3l("Номер рейса будет сгенерирован при добавлении в массив");
	if (AcceptAction())
	{
		newFlight.PushToVector();
		PrintMessage("Новый рейс был добавлен");
		Flight::CopyVectorPtr(flightVector);
	}
	else PrintMessage("Новый рейс не было добавлен");

	PressEnterAction();
}

void UI::Action_ViewFlight_Admin(User* this_user, Flight* flight)
{
	bool done = false;
	char input;


	while (!done)
	{
		ClearConsole();

		PrintMessage3l("Просмотр рейса");

		flight->PrintInfoWithTop();
		PrintCharLine();
		std::cout << '\n';

		PrintMessage("Пассажиры", ' ');
		if (flight->getPassangerCount() < 1)
			PrintMessage3l("Пассажиров нет");
		else
		{
			flight->PrintPassangers();
			PrintCharLine();
		}

		PrintCharLine();
		PrintMessage("1.Забронировать");
		PrintMessage("2.Редактировать");
		PrintMessage("3.Удалить");
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

		PrintMessage3l("Просмотр рейса");

		flight->PrintInfoWithTop();

		PrintCharLine();
		PrintMessage("1.Забронировать");
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
		PrintMessage3l("Вы не можете просматировать данную учетную запись");
		PressEnterAction();
		ClearConsole();
		return;
	}
	while (!done)
	{
		PrintMessage3l("Просмотр пользователя");

		char maxInput = userVector[index]->getAccessLevel() == AccessLevel::NoAccess ? '5' : '4';

		userVector[index]->PrintInfoWithTop();

		PrintCharLine();
		PrintMessage("1.Просмотреть билеты");
		PrintMessage("2.Поменять логин");
		PrintMessage("3.Поменять пароль");
		PrintMessage("4.Удалить");
		if (maxInput == '5') PrintMessage("5.Подтвердить аккаунт");
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
				PrintMessage3l("Нельзя удалить себя");
			else if (Action_RemoveUser(userVector[index], index))
			{
				User::CopyVectorPtr(userVector);
				done = true;
			}
			break;
		}
		case '5':
			PrintMessage3l("Вы уверены, что хотите подтвердить пользоватлея?");
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

	PrintMessage3l("Изменение пароля");

	std::string newPassword;
	InputPassword(newPassword, 8, 32);
	if (AcceptAction)
	{
		user->ChangePassword(newPassword);
		PrintMessage3l("Проль успешно изменен");
	}
	else PrintMessage3l("Пароль не был изменен");
}

void UI::Action_ChangeLogin(User* user)
{
	ClearConsole();

	PrintMessage3l("Изменение логина");

	std::string newLogin;
	InputLogin(newLogin, 3, 32);
	if (AcceptAction)
	{
		user->setLogin(newLogin);
		PrintMessage3l("Логин успешно изменен");
	}
	else PrintMessage3l("Логин не был изменен");
}

void UI::Action_ViewClientTickets(User* user)
{
	ClearConsole();

	PrintMessage3l("Просмотр билетов пользователя");

	if (user->getTicketCount() < 1)
	{
		PrintMessage3l("У пользователя нет билетов");
	}
	else
	{
		ClFomrat::PrintCenteredLine("Билеты пользователя", CL_WIDTH_LINE);
		user->ShowTickets();
	}
	PressEnterAction();
}

bool UI::AcceptAction()
{
	PrintMessage("Нажмите ENTER, чтобы подтвердить, или ESC чтобы отменить");
	char input;
	do input = _getch();
	while (input != ENTER_KEY && input != ESC_KEY);
	return input == ENTER_KEY ? true : false;
}

void UI::PressEnterAction()
{
	ClFomrat::PrintCenteredLine("Нажмите ENTER, чтобы продолжить", CL_WIDTH_LINE, fillCh);
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
