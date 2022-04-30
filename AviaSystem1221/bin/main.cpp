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
		std::cout << "���������������� ���� �� ����������.\n"
			<< "����� ������ ����� ����.\n"
			<< "��������� ���� ������ ����� �������������� ��� ����� ��� �������������\n\n";
		User::CreateUserBase();
		User::NewUser(User::AccessLevel::Admin);
	}
	else if (status == FileStatus::Empty)
	{
		std::cout << "���������������� ���� �����.\n"
			<< "��������� ���� ������ ����� �������������� ��� ����� ��� �������������\n\n";
		User::NewUser(User::AccessLevel::Admin);
	}
	else if (status != FileStatus::Opened)
	{
		std::cerr << "�������������� ������ ��� �������� ���������������� ����!\n\n";
		return -1;
	}

	User::NewUser();
	User user;
	status = user.Login();
	if (status > -1)
	{
		std::cout << "������������ ������.\n";
	}
	else
	{
		std::cout << "������������ �� ������\n";
	}

	User::SaveUserArray();

	return 0;
}