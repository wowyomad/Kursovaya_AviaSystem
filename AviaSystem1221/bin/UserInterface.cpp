#include "UserClass.h"
#include <iostream>
#include "UserInterface.h"
#include "Table.h"
#include <iomanip>


extern const char PATH_FILE_USERS[];
extern const char PATH_FILE_FLIGHTS[];
extern const int CONSOLE_WIDTH;
extern const int CELL_WIDTH;

const char noFileMsg[] = "Не сущетсвует файла по пути: ";
const char newFileMsg[] = "Будет создан новый файл.";
const char noAdministratorMsg[] = "Пользователь с правами администратора найден не был.";
const char newAdminMsg[] = "Введенные вами данные будут использоваться для входа как администратор";
const char unknownErrFileMsg[] = "Непредвиденная ошибка при открытии пользовательской базы";
const char registrationFailMsg[] = "Пользователь с таким логином уже существует.";

void UI::Start()
{
	const char helloMsg1[] = "Добро пожаловуть в информационную систему по билетикам";
	const char helloMsg2[] = "AviaSystem1221";

	Table::PrintCenteredLine(helloMsg1, CONSOLE_WIDTH, '-');
	Table::PrintCenteredLine(helloMsg2, CONSOLE_WIDTH, '-');

}

void UI::Register()
{
	
}
