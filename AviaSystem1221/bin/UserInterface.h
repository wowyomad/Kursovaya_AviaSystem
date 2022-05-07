#pragma once
#include "UserClass.h"
#include "common.h"
namespace UI
{
	void Start();
	void Main();
	void Register(const int accessLevel = AccessLevel::NoAcessLvl);
	void Login();


	void AdminMain();


	void ClientMain();

}