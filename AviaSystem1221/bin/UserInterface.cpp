#include "UserClass.h"
#include <iostream>
#include "UserInterface.h"
#include "Table.h"

#include <iomanip>

#define CONSOLE_WIDTH 188
#define CELL_WIDTH 46

extern const std::string PATH_USERBASE;



void PrintLineCentered(const unsigned width, const char fill = ' ')
{

}

int UI::StartProgram()
{
	const char helloMsg[] = "Информационная система по билетикам";
	Table::PrintCenteredLine(helloMsg, CONSOLE_WIDTH);
	std::cout << '\n';
	/*PrintCell("Как в игре или в жизни?", CELL_WIDTH, Border_LR::Left, Border_UD::UpDown);
	PrintCell("Сейчас или вообще?", CELL_WIDTH, Border_LR::Right, Border_UD::UpDown);
	PrintCell("Кто я?", CELL_WIDTH, Border_LR::Right, Border_UD::Down);
	PrintCell("1231231332312", CELL_WIDTH, Border_LR::NoBorder, Border_UD::UpDown);*/
	Table::PrintCenteredLine("", CONSOLE_WIDTH, '_');
	std::cout << '\n';
	std::cout << '\n';
	int status = UserBase_initial();
	if (status < 0)
		return status;

	return 0;
}

int UI::UserBase_initial()
{
	const char noFileMsg[] = "Не сущетсвует файла по пути: ";
	const char newFileMsg[] = "Будет создан новый файл.";
	const char noAdministratorMsg[] = "Пользователь с правами администратора найден не был.";
	const char newAdminMsg[] = "Введенные вами данные будут использоваться для входа как администратор";
	const char unknownErrFileMsg[] = "Непредвиденная ошибка при открытии пользовательской базы";

	int status = User::InitUserBase();
	if (status == FileStatus::NotOpened || status == FileStatus::Empty)
	{
		std::cout << noFileMsg << PATH_USERBASE << '\n'
			<< newFileMsg << '\n'
			<< newAdminMsg << "\n\n";
		User::CreateUserBase();
		User::NewUser(User::AccessLevel::Admin);
		return 0;
	}
	else if (status == FileStatus::Opened)
	{
		if (User::checkForAdmin() == false)
		{
			std::cout << noAdministratorMsg << '\n'
				<< newAdminMsg << '\n';
			User::NewUser(User::AccessLevel::Admin);
		}
		return 0;
	}
	else
	{
		std::cout << unknownErrFileMsg << PATH_USERBASE;
		return -1;
	}
}

int UI::User_Registration()
{
	Table::PrintCenteredLine("Регистрация", CONSOLE_WIDTH / 2);
	std::cout << '\n';
	User::NewUser();
	return 0;
}
