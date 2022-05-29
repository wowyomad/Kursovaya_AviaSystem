#pragma once
#include "UserClass.h"
#include "common.h"
namespace UI
{
	void StartSystem();

	void Menu_Main();
	bool Register(const AccessLevel = AccessLevel::NoAccess);
	void Login();

	void Main_SuperAdmin(User* this_user);
	void Main_Admin(User* this_user);
	void Main_Client(User* this_user);

	void Menu_Fligths_Admin(User* this_user, std::vector<Flight*>& flightVector);
	void Menu_Fligths_Client(User* this_user, std::vector<Flight*>& flightVector);

	void Menu_CustomFligths_Admin(User* this_user, std::vector<Flight*>& flightVector);
	void Menu_CustomFligths_Client(User* this_user, std::vector<Flight*>& flightVector);

	void Menu_Tickets(User* this_user);

	void Menu_Users(User* this_user, std::vector <User*>& userVector);

	void Action_BookTicket(User* this_user, Flight* flight);

	void Action_AcceptAll_Admin(std::vector <User*>& clientVector);

	bool Action_RemoveUser(User* user, const int index);

	bool Action_RemoveFlight(Flight* flightPtr);

	bool Action_EditFlight(Flight* flight);

	bool Action_AddUser(std::vector<User*>& userVector);

	void Action_SortFlights(std::vector<Flight*>& vectorFlight);

	void Action_SearchFligths_Admin(User* this_user, std::vector<Flight*>& flightVector);

	void Action_SearchFlights_Client(User* this_user, std::vector<Flight*>& flightVector);

	void Action_AddFlight(std::vector<Flight*>& flightVector);

	void Action_ViewFlight_Admin(User* this_user, Flight* flight);

	void Action_ViewFlight_Client(User* this_user, Flight* flight);

	void Action_ViewUser(User* this_user, std::vector<User*>& userVector, const size_t index);

	void Action_ChangePassword(User* user);

	void Action_ChangeLogin(User* user);

	void Action_ViewClientTickets(User* user);

	bool AcceptAction();
	void PressEnterAction();
	void PrintMessage(const char* str, const char fill = ':');
	void PrintMessageNL(const char* str);
	void PrintMessage3l(const char* str, const char ch = '-');
	void PrintCharLine(char ch = '-');
}