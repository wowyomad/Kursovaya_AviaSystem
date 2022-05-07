#pragma once
#include <string>
namespace RNG
{
	std::string randomNumber(const int size = 8);
	std::string hash(std::string password, std::string key, const int hash_length = 8);
}
