#include "UserClass.h"
#include <iostream>
#include "UserInterface.h"
#include "Table.h"
#include <iomanip>

#define CONSOLE_WIDTH 186
#define CELL_WIDTH 23

extern const std::string PATH_USERBASE;

const char noFileMsg[] = "Не сущетсвует файла по пути: ";
const char newFileMsg[] = "Будет создан новый файл.";
const char noAdministratorMsg[] = "Пользователь с правами администратора найден не был.";
const char newAdminMsg[] = "Введенные вами данные будут использоваться для входа как администратор";
const char unknownErrFileMsg[] = "Непредвиденная ошибка при открытии пользовательской базы";
const char registrationFailMsg[] = "Пользователь с таким логином уже существует.";

void Start()
{

}

void UI::Register()
{
	
}
