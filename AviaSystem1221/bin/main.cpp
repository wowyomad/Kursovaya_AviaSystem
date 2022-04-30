#include "Hash.h"
#include "UserClass.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>

	
int main(int argc, char** argv)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	if (!User::InitUserBase())
	{
		User::CreateUserBase();
	}
	User::NewUser();
	User::SaveUserArray();
	return 0;
}