#include <random>
#include <string>
#include <cmath>
#include "Hash.h"

#define ASCII_a 97
#define ASCII_z 122
#define ENG_LETTERS 26
#define HASH_LENGTH 32

std::string RNG::salt()
{
	unsigned long long value;
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, UINT64_MAX);
	value = dist(rng);
	return std::to_string(value);
}

std::string RNG::hash(std::string password, std::string key)
{
	unsigned long long key_value = 0;
	for (int i = 0; i < key.length(); i++)
	{
		key_value += (int)(key[i]);
	}
	std::string unhashed = password + key;
	std::string hashed;
	hashed.resize(HASH_LENGTH);
	for (int i = 0; i < unhashed.length(); i++)
	{
		for (int j = 0; j < key.length(); j++)
		{
			unhashed[i] += ENG_LETTERS % (i * (j + 1) + j + 1);
			while (unhashed[i] > ASCII_z) unhashed[i] -= ENG_LETTERS;
			while (unhashed[i] < ASCII_a) unhashed[i] += ENG_LETTERS;
			if (i == unhashed.length()) break;
		}
	}
	int iterations = unhashed.length() / HASH_LENGTH + 1;
	for (int k = 0; k < iterations; k++)
	{
		for (int i = 0; i < HASH_LENGTH; i++)
		{
			for (int j = 0; j < unhashed.length(); j++)
			{
				hashed[i] += i + j +
					hashed[i] % unhashed.length() + unhashed[j] % ENG_LETTERS;
			}
			while (hashed[i] > ASCII_z) (hashed[i]) -= ENG_LETTERS;
			while (hashed[i] < ASCII_a) hashed[i] += ENG_LETTERS;

			//по таблице ASCII, char - 32 = CHAR.
			if (hashed[i] % 16 > 8) hashed[i] -= 32;
		}
	}
	return hashed;
}
