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

	const char mainMsg[] = "Главное меню";
	const char option1Msg[] = "1.Регистрация";
	const char option2Msg[] = "2.Логин";
	const char option3Msg[] = "Esc.Выйти";

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

	const char rigestserMsg[] = "Регистрация";
	const char successMsg[] = "Пользователь успешно зарегистрирован.";
	const char userExistMsg[] = "Пользователь с таким логином уже зарегистрирован.";
	const char waitMsg[] = "Ожидайте подтверждения от администратора.";

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

	const char loginMsg[] = "Вход";
	const char noUserMsg[] = "Пользователь не найден.";
	const char retryMsg[] = "Хотите повторить попытку?";
	const char successMsg[] = "Вы успешно вошли в аккаунт.";
	const char adminMsg[] = "Вы будете перенаправлены в меню администратора.";
	const char clientMsg[] = "Вы будете перенаправлены в меню клиента";

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
	const char mainMsg[] = "Меню администратора";
	const char op1ion1Msg[] = "1.Просмотре рейсов";
	const char option2Msg[] = "2.Просмотре пользователей";
	const char option3Msg[] = "2.Просмотр клиентов";

	ClFomrat::PrintCenteredLine(mainMsg, CL_WIDTH, fillCh);

	while (true)
	{

	}
}

void UI::ViewAllFlights(const int mode)
{
	const char option1Msg[] = "1.Выбрать";
	const char option2Msg[] = "2.Отсортировать";
	const char option3Msg[] = "3.Поиск";
	const char optoin4Msg[] = "4.Добавить рейса";
	const char option4Msg[] = "Esc.Назад";
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
			InputVar(num, 1, Flight::getVectorSize(), "Номер: ");
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
	const char option1Msg[] = "1. Изменить данные";
	const char option2Msg[] = "Esc. Назад";
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
	const char option1Msg[] = "1.Поиск по номеру рейса";
	const char option2Msg[] = "2.Поиск по месту вылета";
	const char option3Msg[] = "3.Поиск по месту прибытия";
	const char option4Msg[] = "Esc.Назад";
	const char noFoundMsg[] = "По вашему запроссу ничего не найдено";

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
			InputVar(num, 0, UINT_MAX, "Номер рейса");
			foundFlights = Flight::Search(FlightSearchType::Id, std::to_string(num));
			break;
		}
		case '2':
		{
			std::string location;
			InputString(location, "Место вылета");
			foundFlights = Flight::Search(FlightSearchType::LocDeparture, location);
			break;
		}
		case '3':
		{
			std::string location;
			InputString(location, "Место прибытия");
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
	const char option1Msg[] = "1.Соритровка по номеру рейса";
	const char option2Msg[] = "2.Сортировка по месту вылета";
	const char option3Msg[] = "3.Соритровка по месту прибытия";
	const char option4Msg[] = "4.Сортировка по дате вылета";
	const char option5Msg[] = "Esc.Назад";

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
	const char acceptMsg[] = "Рейс был добавлен в список";
	const char declineMsg[] = "Рейс не был добавлен в список";
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
	const char option1Msg[] = "1.Изменить место вылета";
	const char option2Msg[] = "2.Изменить место прибытия";
	const char option3Msg[] = "3.Изменить дату вылета";
	const char option4Msg[] = "4.Изменить время вылета";
	const char option5Msg[] = "5.Изменить время прилета";
	const char option6Msg[] = "Esc.Назад";
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
			InputString(location, "Место вылета");
			ptr->setLocDeparture(location);
		}
		break;
		case '2':
		{
			std::string location;
			InputString(location, "Место прибытия");
			ptr->setLocArrival(location);
			break;
		}
		case '3':
		{
			tm tm;
			InputDate(tm, "Дата вылета");
			ptr->setDateDepatrue(tm);
			break;
		}
		case '4':
		{
			tm tm;
			InputTime(tm, "Время вылета");
			ptr->setTimeDeparture(tm);
			break;
		}
		case '5':
		{
			tm tm;
			InputTime(tm, "Время прибытия");
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
