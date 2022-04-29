#pragma once
#include <string>
namespace RNG
{
	std::string salt();
	std::string hash(std::string password, std::string key);
}
