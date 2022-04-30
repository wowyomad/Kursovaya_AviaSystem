#include "Hash.h"
#include "UserClass.h"
#include "FileHandle.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int status = User::InitUserBase();
	if (status == FileStatus::NotOpened)
	{
		std::cout << "Пользовательской базы не существует.\n"
			<< "Будет создан новый файл.\n"
			<< "Введенные вами данные будут использоваться для входа как администратор\n\n";
		User::CreateUserBase();
		User::NewUser(User::AccessLevel::Admin);
	}
	else if (status == FileStatus::Empty)
	{
		std::cout << "Пользовательская база пуста.\n"
			<< "Введенные вами данные будут использоваться для входа как администратор\n\n";
		User::NewUser(User::AccessLevel::Admin);
	}
	else if (status != FileStatus::Opened)
	{
		std::cerr << "Непредвиденная ошибка при открытии пользовательской базы!\n\n";
		return -1;
	}

	User::NewUser();
	User user;
	status = user.Login();
	if (status > -1)
	{
		std::cout << "Пользователь найден.\n";
	}
	else
	{
		std::cout << "Пользователь не найден\n";
	}

	User::SaveUserArray();

	return 0;
}