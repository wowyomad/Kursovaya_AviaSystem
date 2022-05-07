#include "UserInterface.h"
#include "FlightClass.h"
#include "Table.h"
#include <iomanip>
#include <iostream>
#include <conio.h>

extern const char PATH_FILE_USERS[];
extern const char PATH_FILE_FLIGHTS[];

const char noFileMsg[] = "Не сущетсвует файла по пути: ";
const char newAdminMsg[] = "Введенные вами данные будут использоваться для входа как администратор";
const char unknownErrFileMsg[] = "Непредвиденная ошибка при открытии пользовательской базы";
const char registrationFailMsg[] = "Пользователь с таким логином уже существует.";
const char pressKeyMsg[] = "Нажмите любую кнопку чтобы продолжить.\n";

const char fillCh = '-';

void UI::Start()
{
	const char helloMsg1[] = "Добро пожаловуть в информационную систему по билетикам";
	const char helloMsg2[] = "AviaSystem1221";
	const char userFileMsg[] = "Файл с пользовательскими данными";
	const char flightFileMsg[] = "Файл с данными об авиарейсах";
	const char clientFileMsg[] = "Файл с данными о клиентах";
	const char noFileMsg[] = "не существует.";
	const char emptyFileMsg[] = "пустой.";
	const char newFileMsg[] = "Будет создан новый файл.";
	const char noAdminMsg[] = "Пользователь с правами администратора найден не был.";
	const char adminReqMsg[] = "Рекомендуется войти как администратор.";
	const char badFileMsg[] = "Один или несколько файлов с данными пустые.";
	const char goodFileMsg[] = "Все файлы были успешно инициализированы.";


	FormattedOutput::PrintCenteredLine(helloMsg1, CONSOLE_WIDTH, fillCh);
	FormattedOutput::PrintCenteredLine(helloMsg2, CONSOLE_WIDTH, fillCh);
	std::cout << '\n';

	int status1 = Entity::GetFileStatus();
	if (status1 == FileStatus::NotOpened)
	{
		std::cout << userFileMsg << ' ' << noFileMsg << '\n';
		std::cout << newFileMsg << '\n';
		std::cout << newAdminMsg << '\n';
		Entity::AddNewEntity(AccessLevel::AdminLvl);
	}
	else if (status1 == FileStatus::Empty)
	{
		std::cout << userFileMsg << ' ' << emptyFileMsg << '\n';
		std::cout << newAdminMsg << '\n';
		Entity::AddNewEntity(AccessLevel::AdminLvl);
	}
	else
	{
		Entity::ReadFile();
		bool adminExist = Entity::CheckFileForAdmin;
		if (!adminExist)
		{
			std::cout << noAdminMsg << '\n';
		}
	}

	int status2 = Client::GetFileStatus();
	if (status2 == FileStatus::NotOpened)
	{
		std::cout << clientFileMsg << '  ' << noFileMsg << '\n';
		std::cout << newFileMsg << '\n';
		Client::CreateFile();
	}
	else if (status2 == FileStatus::Empty)
	{
		std::cout << clientFileMsg << ' ' << emptyFileMsg << '\n';
	}
	else
	{
		Client::ReadFile();
	}

	int status3 = Flight::GetFileStatus();
	if (status1 == FileStatus::NotOpened)
	{
		std::cout << flightFileMsg << ' ' << noFileMsg << '\n';
		std::cout << newFileMsg << '\n';

		Flight::CreateFile();
	}
	else if (status3 == FileStatus::Empty)
	{
		std::cout << flightFileMsg << ' ' << emptyFileMsg << '\n';
	}
	else
	{
		Flight::ReadFile();
	}

	if ((status1 + status2 + status3) == 0)
	{
		std::cout << goodFileMsg << '\n';
	}
	else
	{
		std::cout << badFileMsg << '\n';
		std::cout << adminReqMsg << '\n';
	}

	std::cout << '\n';
	std::cout << pressKeyMsg << '\n';
	_getch();

	Main();
}

void UI::Main()
{
	system("cls");

	const char mainMsg[] = "Главное меню";
	const char option1Msg[] = "1.Регистрация";
	const char option2Msg[] = "2.Логин";
	const char option3Msg[] = "0.Выйти";

	FormattedOutput::PrintCenteredLine(mainMsg, CONSOLE_WIDTH, fillCh);
	std::cout << '\n';
	FormattedOutput::PrintCenteredLine(option1Msg, CONSOLE_WIDTH, fillCh);
	FormattedOutput::PrintCenteredLine(option2Msg, CONSOLE_WIDTH, fillCh);
	FormattedOutput::PrintCenteredLine(option3Msg, CONSOLE_WIDTH, fillCh);

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
		case '0':
			success = true;
			break;
		default:
			break;
		}
	}
}

void UI::Register(const int accessLevel)
{
	system("cls");

	const char rigestserMsg[] = "Регистрация";
	const char successMsg[] = "Пользователь успешно зарегистрирован.";
	const char userExistMsg[] = "Пользователь с таким логином уже зарегистрирован.";
	const char waitMsg[] = "Ожидайте подтверждения от администратора.";

	FormattedOutput::PrintCenteredLine(rigestserMsg, CONSOLE_WIDTH, '*');
	std::cout << '\n';
	
	if (Entity::AddNewEntity())
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
	system("cls");

	const char loginMsg[] = "Вход";
	const char noUserMsg[] = "Пользователь не найден.";
	const char retryMsg[] = "Хотите повторить попытку?";
	const char successMsg[] = "Вы успешно вошли в аккаунт.";
	const char adminMsg[] = "Вы будете перенаправлены в меню администратора.";
	const char clientMsg[] = "Вы будете перенаправлены в меню клиента";



	FormattedOutput::PrintCenteredLine(loginMsg, CONSOLE_WIDTH, '*');
	do
	{
		int accessLevel = Entity::LoginEntity();
		if (accessLevel > -1)
		{
			FormattedOutput::PrintCenteredLine(successMsg, CONSOLE_WIDTH, '*');
			if (accessLevel == AccessLevel::AdminLvl)
			{
				FormattedOutput::PrintCenteredLine(adminMsg, CONSOLE_WIDTH, '*');
				break;
			}
			else
			{
				FormattedOutput::PrintCenteredLine(clientMsg, CONSOLE_WIDTH, '*');
				break;
			}
		}
		else
		{
			std::cout << noUserMsg << '\n';
			std::cout << retryMsg << '\n';

			bool correctInput = false;
			while (!correctInput)
			{
				char ch = _getch();
				switch (ch)
				{
				case '1':
					correctInput = true;
					break;
				case '0':
					Main();
				default:
					break;
				}
			}
		}
	} while (true);
}

void UI::Admin::Main()
{
	system("cls");
	const char mainMsg[] = "Меню администратора";



}
