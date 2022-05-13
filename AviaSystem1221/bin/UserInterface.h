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
	void ViewAllFlights(const int mode);
	void ViewSpecificFlights(std::vector<Flight> vec, const int mode);
	void AdminViewAllUsers();
	void AdminViewSpecificUsers(std::vector<Flight> vec, const int mode);
	void AdminViewUser(const int index);
	void AdminViewFlight(const int index);
	void SearchFligths(const int mode);
	void AdminSearchUsers();
	void AdminAddFlight();
	void AdminEditFlight(const int index);
	void AdminAcceptUsers();

	void SuperAdminAddAdmin();

	void SortFlights();


	void ClientMain();
	void ClientViewAllFlights();
	void ClientSearchFlights();
	void ClientViewTickets();
	void ClientBuyTicket();


}