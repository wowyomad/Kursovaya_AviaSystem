#include <random>
#include <string>
#include <cmath>
#include "Hash.h"

std::string RNG::salt(const int size)
{
	std::string salt;
	unsigned long long value;
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 9);
	for (int i = 0; i < size; i++)
	{
		value = dist(rng);
		salt += std::to_string(value);
	}
	return salt;
}

std::string RNG::hash(std::string password, std::string key, const int hash_length)
{
	unsigned long long key_value1 = 0;
	unsigned long long key_value2 = 0;
	unsigned long long key_value3 = 0;
	for (int i = 0; i < key.length(); i++)
	{
		key_value1 += key[i];
	}
	for (int i = 0; i < password.length(); i++)
	{
		key_value2 += password[i];
	}
	key_value3 += key_value1 + key_value2;
	std::string unhashed = password + key;
	std::string hashed;
	hashed.resize(hash_length);
	for (int i = 0; i < unhashed.length(); i++)
	{
		for (int j = 0; j < key.length(); j++)
		{
			unhashed[i] += ('z' - 'a') % (i * (j + 1) + j + 1);
			while (unhashed[i] > 'z') unhashed[i] -= ('z' - 'a');
			while (unhashed[i] < 'a') unhashed[i] += ('z' - 'a');
			key_value3 += key_value1 % unhashed[i] + key_value2 % unhashed[i];
			if (i == unhashed.length()) break;
		}
	}
	int iterations = unhashed.length() / hash_length + 1;
	for (int k = 0; k < iterations; k++)
	{
		for (int i = 0; i < hash_length; i++)
		{
			for (int j = 0; j < unhashed.length(); j++)
			{
				int m = i + j;
				hashed[i] += m + (key_value3 * (m - 1) % (m + 1)) + (key_value3 * (m - 1) % (m + 1))
						+ hashed[i] % unhashed.length() + unhashed[j] % ('z' - 'a');
			}
			while (hashed[i] > 'z') (hashed[i]) -= 'z' - 'a';
			while (hashed[i] < 'a') hashed[i] += 'z' - 'a';
			//по таблице ASCII, char - 32 = CHAR.
			if ((unhashed[i] + unhashed.length()) % 16 > 8) hashed[i] -= 32;

		}
	}
	return hashed;
}
