#include "UserInterface.h"
#include "FlightClass.h"
#include "UserConsoleInput.h"
#include "Table.h"
#include <iomanip>
#include <iostream>
#include <conio.h>
#include <memory>

extern const char PATH_FILE_USERS[];
extern const char PATH_FILE_FLIGHTS[];

static const char ENTER_KEY = 13;
static const char ESC_KEY = 27;

const char noFileMsg[] = "Не сущетсвует файла по пути: ";
const char newAdminMsg[] = "Введенные вами данные будут использоваться для входа как администратор";
const char unknownErrFileMsg[] = "Непредвиденная ошибка при открытии пользовательской базы";
const char registrationFailMsg[] = "Пользователь с таким логином уже существует.";
const char pressKeyMsg[] = "Нажмите любую кнопку чтобы продолжить.";

const char fillCh = '-';

void UI::Start()
{
	const char helloMsg1[] = "Добро пожаловуть в информационную систему по билетикам";
	const char helloMsg2[] = "AviaSystem1221";
	const char noUserFileMsg[] = "Файл с пользовательскими данными не обнаружен";
	const char noFlightFileMsg[] = "Файл с данными об авиарейсах не обнаружен";
	const char noClientFileMsg[] = "Файл с клиентской базой не обнаружен";
	const char emptyUserFIleMsg[] = "Файл с пользовательскими данными пустой.";
	const char emptyFlightFIleMsg[] = "Файл с данными об авиарейсах пустой.";
	const char emptyClientFileMsg[] = "Файл с клиентской базой пустой.";
	const char newFileMsg[] = "Будет создан новый файл.";
	const char noAdminMsg[] = "Пользователь с правами Супер-Админа найден не был.";
	const char adminReqMsg[] = "Рекомендуется войти как Админ.";
	const char goodFileMsg[] = "Все файлы были успешно инициализированы.";


	ClFomrat::PrintCenteredLine(helloMsg1, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(helloMsg2, CL_WIDTH_LINE, fillCh);

	int status = User::GetFileStatus();
	if (status == FileStatus::NotOpened)
	{
		ClFomrat::PrintCenteredLine(noUserFileMsg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(newFileMsg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(newAdminMsg, CL_WIDTH_LINE, fillCh);
		User::CreateNewUser(AccessLevel::SuperAdminLvl);
	}
	else if (status == FileStatus::Empty)
	{
		ClFomrat::PrintCenteredLine(emptyUserFIleMsg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(newAdminMsg, CL_WIDTH_LINE, fillCh);
		User::CreateNewUser(AccessLevel::SuperAdminLvl);
	}
	else
	{
		User::ReadFile();
		bool superAdminExist = User::CheckForSuperAdmin;
		if (!superAdminExist)
		{
			ClFomrat::PrintCenteredLine(noAdminMsg, CL_WIDTH_LINE, fillCh);
			ClFomrat::PrintCenteredLine(newAdminMsg, CL_WIDTH_LINE, fillCh);
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

	ClFomrat::PrintCenteredLine(pressKeyMsg, CL_WIDTH_LINE, fillCh);
	_getch();

	Main();
}

void UI::Main()
{
	ClearConsole();

	const char mainMsg[] = "Главное меню";
	const char option1Msg[] = "1.Регистрация";
	const char option2Msg[] = "2.Логин";
	const char option3Msg[] = "Esc.Выйти";


	bool success = false;
	char ch;
	while (!success)
	{
		ClFomrat::PrintCenteredLine(mainMsg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option3Msg, CL_WIDTH_LINE, fillCh);

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
			Flight::WriteToFile();
			Client::WriteToFile();
			User::WriteToFile();
			success = true;
			break;
		}

	}
}

void UI::Register(const int accessLevel)
{
	ClearConsole();

	const char rigestserMsg[] = "Регистрация";
	const char successMsg[] = "Пользователь успешно зарегистрирован.";
	const char userExistMsg[] = "Пользователь с таким логином уже зарегистрирован.";
	const char waitMsg[] = "Ожидайте подтверждения от администратора.";

	ClFomrat::PrintCenteredLine(rigestserMsg, CL_WIDTH_LINE, fillCh);
	std::cout << '\n';

	if (User::CreateNewUser())
	{

		ClFomrat::PrintCenteredLine(successMsg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredNewLine(waitMsg, CL_WIDTH_LINE, fillCh);
	}
	else
	{
		ClFomrat::PrintCenteredNewLine(userExistMsg, CL_WIDTH_LINE, fillCh);
	}
	ClFomrat::PrintCenteredNewLine(pressKeyMsg, CL_WIDTH_LINE, fillCh);
	_getch();
	Main();
}

void UI::Login()
{
	ClearConsole();
	const char loginMsg[] = "Вход";
	const char noUserMsg[] = "Пользователь не найден.";
	const char retryMsg[] = "Хотите повторить попытку?";
	const char successMsg[] = "Вы успешно вошли в аккаунт.";
	const char unavailableMsg[] = "Ваш аккаунт еще не подтвержден";
	const char superAdminMsg[] = "Вы будете перенапрелвены в меню супер-админа";
	const char adminMsg[] = "Вы будете перенаправлены в меню админа.";
	const char clientMsg[] = "Вы будете перенаправлены в меню клиента";
	int accessLevel;
	std::string login;
	ClFomrat::PrintCenteredLine(loginMsg, CL_WIDTH_LINE, fillCh);
	do
	{
		accessLevel = User::LoginUser(&login);
		if (accessLevel > 0)
		{
			break;
		}
		else
		{
			ClFomrat::PrintCenteredLine(noUserMsg, CL_WIDTH_LINE, fillCh);
			ClFomrat::PrintCenteredLine(retryMsg, CL_WIDTH_LINE, fillCh);
			ClFomrat::PrintCenteredLine("1.Да", CL_WIDTH_LINE, fillCh);
			ClFomrat::PrintCenteredNewLine("2.Нет", CL_WIDTH_LINE, fillCh);

			bool correctInput = false;
			while (!correctInput)
			{
				char ch = _getch();
				switch (ch)
				{
				case '1':
					correctInput = true;
					break;
				case '2':
					correctInput = true;
					UI::Main();
				default:
					break;
				}
			}
		}
	} while (true);

	ClFomrat::PrintCenteredLine(successMsg, CL_WIDTH_LINE, fillCh);
	if (accessLevel == AccessLevel::SuperAdminLvl)
	{
		ClFomrat::PrintCenteredLine(superAdminMsg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(pressKeyMsg, CL_WIDTH_LINE, fillCh);
		_getch();
		UI::SuperAdminMain();
		//Ui::SuperAdminMain();
	}
	else if (accessLevel == AccessLevel::AdminLvl)
	{
		ClFomrat::PrintCenteredLine(adminMsg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(pressKeyMsg, CL_WIDTH_LINE, fillCh);
		_getch();
		UI::AdminMain();
	}
	{

		ClFomrat::PrintCenteredLine(clientMsg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(pressKeyMsg, CL_WIDTH_LINE, fillCh);
		_getch();
		std::unique_ptr<Client> client(new Client);
		try
		{
			*client = Client::getClient(login);
		}
		catch (std::invalid_argument exc)
		{
			std::cout << exc.what();

		}
		UI::ClientMain(*client);
	}
}

void UI::AdminMain()
{
	ClearConsole();
	const char mainMsg[] = "Меню администратора";
	const char option1Msg[] = "1.Просмотре рейсов";
	const char option2Msg[] = "2.Просмотре пользователей";
	const char option3Msg[] = "3.Просмотр клиентов";
	const char option4Msg[] = "4.Подтвердить пользователей";
	const char option0Msg[] = "Esc.Назад";

	ClFomrat::PrintCenteredNewLine(mainMsg, CL_WIDTH_LINE, fillCh);

	bool done = false;
	while (!done)
	{
		ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option3Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option4Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option0Msg, CL_WIDTH_LINE, fillCh);
		char input;
		do
		{
			input = _getch();
		} while (input < 1 && input > 4 && input != ESC_KEY);

		switch (input)
		{
		case '1':
			ViewAllFlights(InfoMode::AdminMode);
			break;
		case '2':
			AdminViewAllUsers();
			break;
		case '3':
			AdminViewAllClients();
			break;
		case '4':
			AdminAcceptUsers();
			break;
		case ESC_KEY:
			done = true;
			break;
		}
		ClearConsole();
	}

	Main();
}


void UI::ViewAllFlights(const int mode, Client* client)
{
	const char option1Msg[] = "1.Выбрать";
	const char option2Msg[] = "2.Отсортировать";
	const char option3Msg[] = "3.Поиск";
	const char option4Msg[] = "4.Добавить рейса";
	const char option0Msg[] = "Esc.Назад";
	bool done = false;
	char input;
	const char MIN = '1';
	const char MAX = mode != InfoMode::UserMode ? '4' : '3';
	while (!done)
	{
		Flight::PrintInfoVector(mode);
		ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option3Msg, CL_WIDTH_LINE, fillCh);
		if (mode != InfoMode::UserMode)
			ClFomrat::PrintCenteredLine(option4Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option0Msg, CL_WIDTH_LINE, fillCh);
		do
		{
			input = _getch();
		} while ((input < MIN || input > MAX) && input != ESC_KEY);
		switch (input)
		{
		case '1':
			int num;
			InputVar(num, 1, Flight::getVectorSize(), "Номер: ");
			if (mode == InfoMode::UserMode)
				ClientViewFlight(*client, num - 1);
			else
				AdminViewFlight(num - 1);
			break;
		case '2':
			std::cout << '\n';
			SortFlights();
			break;
			if (true)
		case '3':
			std::cout << '\n';
			SearchFligths(mode);
		case '4':
		{
			Flight flight;
			flight.InputInfo();
			flight.PushToVector();
			break;
		}
		case ESC_KEY:
			done = true;
			break;
		}
	}
}

void UI::AdminViewAllUsers()
{
	const char option1Msg[] = "1.Выбрать";
	const char option2Msg[] = "2.Отсортировать";
	const char option3Msg[] = "3.Поиск";
	const char option0Msg[] = "Esc.Назад";

	bool done = false;
	char input;
	while (!done)
	{
		User::PrintInfoWhole();

		ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option3Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option0Msg, CL_WIDTH_LINE, fillCh);

		do
		{
			input = _getch();
		} while ((input < '1' || input > '2') && input != ESC_KEY);
		switch (input)
		{
		case '1':
			int num;
			InputVar(num, 1, User::getVectorSize(), "Номер: ");
			AdminViewUser(num - 1);
			break;
		case '2':
			std::cout << '\n';
			AdminSortUsers();
			break;
		case ESC_KEY:
			done = true;
			break;
		}
	}
}

void UI::AdminViewSpecificUsers(std::vector<Flight> vec, const int mode)
{

}

void UI::AdminViewUser(const int index)
{
}

void UI::ClientMain(Client& client)
{
	const char mainMsg[] = "Меню клиента";
	const char option1Msg[] = "1.Список рейсов";
	const char option2Msg[] = "2.Посмотреть купленные билеты";
	const char option0Msg[] = "Esc.Назад";

	ClFomrat::PrintCenteredNewLine(mainMsg, CL_WIDTH_LINE, fillCh);

	bool done = false;
	while (!done)
	{
		ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option0Msg, CL_WIDTH_LINE, fillCh);
		char input;
		do
		{
			input = _getch();
		} while (input < 1 && input > 4 && input != ESC_KEY);

		switch (input)
		{
		case '1':
			ViewAllFlights(InfoMode::UserMode, &client);
			break;
		case '2':
			client.PrintTickets();
			ClFomrat::PrintCenteredLine(pressKeyMsg, CL_WIDTH_LINE, fillCh);
			_getch();
			break;
		case ESC_KEY:
			done = true;
			break;
		}
		ClearConsole();
	}

	Main();

}

void UI::ClientViewFlight(Client& client, const int index)
{
	ClearConsole();

	const char option1Msg[] = "1.Забронировать билет";
	const char option0Msg[] = "Esc.Назад";

	bool correctInput = false;
	char input;
	Flight* flight = Flight::GetFlight(index);
	flight->PrintInfoWithTop(InfoMode::UserMode);
	while (!correctInput)
	{
		correctInput = true;
		ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option0Msg, CL_WIDTH_LINE, fillCh);
		input = _getch();
		switch (input)
		{
		case '1':
			ClientBookTicket(client, flight);
			break;
		case ESC_KEY:
			break;
		default:
			correctInput = false;
			break;
		}
	}
}

void UI::ClientBookTicket(Client& client, Flight* flight)
{
	const char acceptMsg[] = "Подтвердите бронирование";
	const char option1Msg[] = "1.Подтвердить";
	const char type1Msg[] = "1.Эконом-класс";
	const char type2Msg[] = "2.Бизнес-класс";
	const char noType1Msg[] = "Билетов эконом-класса нет";
	const char noType2Msg[] = "Билетов бизнес-класса нет";
	const char successMsg[] = "Вы успешно забронировали билет";
	const char option0Msg[] = "Esc.Назад";
	bool done;
	bool ticketBooked = true;
	std::string id = flight->getId();
	const int index = Flight::GetFlightIndex(id);
	char input;
	do
	{

		ClFomrat::PrintCenteredLine(type1Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(type2Msg, CL_WIDTH_LINE, fillCh);

		done = true;
		do input = _getch();
		while (input != '1' && input != '2' && input != ESC_KEY);

		if (input == '1')
			if (flight->TicketAvailable(TicketType::Economy))
			{
				ClFomrat::PrintCenteredNewLine(acceptMsg, CL_WIDTH_LINE, fillCh);
				ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
				ClFomrat::PrintCenteredLine(option0Msg, CL_WIDTH_LINE, fillCh);
				do input = _getch();
				while ((input != '1' && input != ESC_KEY));
				if (input == '1')
					client.BookTicket(index, TicketType::Economy);
				else
				{
					done = false;
					break;
				}
			}
			else
			{
				ClFomrat::PrintCenteredNewLine(noType1Msg, CL_WIDTH, fillCh);
			}
		else if (input == '2')
			if (flight->TicketAvailable(TicketType::Business))
			{
				ClFomrat::PrintCenteredLine(acceptMsg, CL_WIDTH_LINE, fillCh);
				ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
				ClFomrat::PrintCenteredLine(option0Msg, CL_WIDTH_LINE, fillCh);
				do input = _getch();
				while ((input != '1' && input != ESC_KEY));
				if (input == '1')
					client.BookTicket(index, TicketType::Business);
				else
				{
					done = false;
					break;
				}
			}
			else
			{
				ClFomrat::PrintCenteredNewLine(noType2Msg, CL_WIDTH, fillCh);
			}
		else
		{
			ticketBooked = false;
			done = true;
		}

	} while (!done);

	if (ticketBooked)
	{
		ClFomrat::PrintCenteredLine(successMsg, CL_WIDTH_LINE, fillCh);
	}
	ClFomrat::PrintCenteredLine(pressKeyMsg, CL_WIDTH_LINE, fillCh);
	_getch();
}

void UI::AdminViewFlight(const int index)
{
	const char option1Msg[] = "1. Изменить данные";
	const char option2Msg[] = "Esc. Назад";
	bool correctInput = false;
	char input;
	const Flight* flight = Flight::GetFlight(index);
	flight->PrintInfoWithTop(InfoMode::AdminMode_noCount);
	while (!correctInput)
	{
		correctInput = true;
		ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH_LINE, fillCh);
		input = _getch();
		switch (input)
		{
		case '1':
			std::cout << '\n';
			AdminEditFlight(index);
			break;
		case ESC_KEY:
			std::cout << '\n';
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
	const char option1Msg[] = "1.Поиск по номеру рейса";
	const char option2Msg[] = "2.Поиск по месту вылета";
	const char option3Msg[] = "3.Поиск по месту прибытия";
	const char option4Msg[] = "Esc.Назад";
	const char noFoundMsg[] = "По вашему запроссу ничего не найдено";

	ClFomrat::PrintCenteredLine("Поиск", CL_WIDTH_LINE, fillCh);
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
			InputVar(num, 0, Flight::getVectorSize(), "Номер рейса: ");
			foundFlights = Flight::Search(FlightSearchType::Id, std::to_string(num));
			break;
		}
		case '2':
		{
			std::string location;
			InputString(location, "Место вылета: ");
			foundFlights = Flight::Search(FlightSearchType::LocDeparture, location);
			break;
		}
		case '3':
		{
			std::string location;
			InputString(location, "Место прибытия: ");
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
			ClFomrat::PrintCenteredNewLine(noFoundMsg, CL_WIDTH_LINE, fillCh);
		else
			ViewSpecificFlights(foundFlights, mode);
	}

}

void UI::AdminSearchUsers()
{

}

void UI::ViewSpecificFlights(std::vector<Flight> vec, const int mode)
{
	const char option1Msg[] = "1.Выбрать";
	const char option2Msg[] = "Esc.Назад";
	bool done = false;
	char input;

	Flight::PrintInfoVector(vec, mode);
	std::cout << '\n';
	ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH_LINE, fillCh);

	do
	{
		input = _getch();
	} while (input != '1' && input != ESC_KEY);
	switch (input)
	{
	case '1':
	{
		int num;
		InputVar(num, 1, vec.size(), "Номер: ");
		std::string id = vec[num - 1].getId();
		num = Flight::GetFlightIndex(id);
		AdminViewFlight(num);
		break;
	}
	case ESC_KEY:
		break;
	}
}

void UI::SortFlights()
{
	const char option1Msg[] = "1.Соритровка по номеру рейса";
	const char option2Msg[] = "2.Сортировка по месту вылета";
	const char option3Msg[] = "3.Соритровка по месту прибытия";
	const char option4Msg[] = "4.Сортировка по дате вылета";
	const char option5Msg[] = "Esc.Назад";

	ClFomrat::PrintCenteredNewLine("Сортировка", CL_WIDTH_LINE, fillCh);
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
			std::cout << "Отсортировано по номеру рейса\n";
			break;
		case '2':
			Flight::Sort(FlightSortType::LocDeparture);
			std::cout << "Отсортировано по месту вылета\n";
			break;
		case '3':
			Flight::Sort(FlightSortType::LocArrival);
			std::cout << "Отсортировано по месту прибытия\n";
			break;
		case '4':
			Flight::Sort(FlightSortType::DateDeparture);
			std::cout << "Отсортировано по дате вылета\n";
			break;
		default:
			correctInput = false;
			break;
		}
	} while (!correctInput);
}

void UI::AdminAddFlight()
{
	const char acceptMsg[] = "Рейс был добавлен в список";
	const char declineMsg[] = "Рейс не был добавлен в список";
	Flight newFlight;
	if (newFlight.InputInfo())
	{
		newFlight.PushToVector();
		ClFomrat::PrintCenteredLine(acceptMsg, CL_WIDTH_LINE, fillCh);
	}
	else
	{
		ClFomrat::PrintCenteredLine(declineMsg, CL_WIDTH_LINE, fillCh);
	}
}

void UI::AdminEditFlight(const int index)
{
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	const char option1Msg[] = "1.Изменить место вылета";
	const char option2Msg[] = "2.Изменить место прибытия";
	const char option3Msg[] = "3.Изменить дату вылета";
	const char option4Msg[] = "4.Изменить время вылета";
	const char option5Msg[] = "5.Изменить время прибытия";
	const char option6Msg[] = "Esc.Назад";
	Flight* ptr = Flight::GetFlight(index);
	char input;
	bool done = false;
	while (!done)
	{
		ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option2Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option3Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option4Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option5Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredNewLine(option6Msg, CL_WIDTH_LINE, fillCh);
		input = _getch();
		switch (input)
		{
		case '1':
		{
			std::string location;
			InputString(location, "Место вылета: ");
			ptr->setLocDeparture(location);
			break;
		}
		case '2':
		{
			std::string location;
			InputString(location, "Место прибытия: ");
			ptr->setLocArrival(location);
			break;
		}
		case '3':
		{
			tm tm;
			InputDate(tm, "Дата вылета: ");
			ptr->setDateDepatrue(tm);
			break;
		}
		case '4':
		{
			tm tm;
			InputTime(tm, "Время вылета: ");
			ptr->setTimeDeparture(tm);
			break;
		}
		case '5':
		{
			tm tm;
			InputTime(tm, "Время прибытия: ");
			ptr->setTimeArrival(tm);
			break;
		}
		case ESC_KEY:
		{
			done = true;
			break;
		}
		default:
			break;
		}

		if (input != ESC_KEY)
			ptr->PrintInfoWithTop(InfoMode::AdminMode_noCount);
		std::cout << '\n';
	}
}

void UI::AdminAcceptUsers()
{
	const char option1Msg[] = "1.Подтвердить всех";
	const char option0Msg[] = "Esc.Назад";
	bool done = false;
	char input;

	std::cout << '\n';
	ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(option0Msg, CL_WIDTH_LINE, fillCh);
	do
	{
		input = _getch();

	} while (input != '1' && input != ESC_KEY);
	switch (input)
	{
	case '1':
		Admin::AcceptAll();
		done = true;
		break;
	case ESC_KEY:
		done = true;
		break;
	}while (!done);

}

void UI::AdminSortUsers()
{
	const char option1Msg[] = "1.Соритровка по логину";
	const char option0Msg[] = "Esc.Назад";
	std::cout << '\n';
	ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
	ClFomrat::PrintCenteredLine(option0Msg, CL_WIDTH_LINE, fillCh);
	char input;
	do
	{
		input = _getch();
	} while (input != '1' && input != ESC_KEY);

	switch (input)
	{
	case '1':
		User::Sort(UserSortType::Login);
		ClFomrat::PrintCenteredLine("Отсортировано по логину", CL_WIDTH_LINE, fillCh);
		break;
	case ESC_KEY:
		break;
	}
}

void UI::AdminViewAllClients()
{

}

void UI::SuperAdminMain()
{
	ClearConsole();
	const char option1Msg[] = "1.Добавить админа";
	const char option0Msg[] = "Esc.Назад";
	bool done = false;
	char input;
	while (!done)
	{
		ClFomrat::PrintCenteredLine(option1Msg, CL_WIDTH_LINE, fillCh);
		ClFomrat::PrintCenteredLine(option0Msg, CL_WIDTH_LINE, fillCh);
		do
		{
			input = _getch();
		} while (input != '1' && input != ESC_KEY);
		switch (input)
		{
		case '1':
			SuperAdminAddAdmin();
			break;
		case ESC_KEY:
			done = true;
			break;
		default:
			break;
		}
	}

	Main();
}

void UI::SuperAdminAddAdmin()
{
	if (User::CreateNewUser(AccessLevel::AdminLvl))
		ClFomrat::PrintCenteredLine("Админ был добавлен", CL_WIDTH_LINE, fillCh);
	else
		ClFomrat::PrintCenteredLine("Пользователь с таким логином уже сущетсвует", CL_WIDTH_LINE, fillCh);
}
