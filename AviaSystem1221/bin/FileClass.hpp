#pragma once

#include <fstream>
#include <vector>

static const size_t VECTOR_BUFF = 0x100000;	//Flight - ~256MB; 

enum FileStatus
{
	NotOpened = 0,
	Empty = 1,
	Opened = 2
};

class File
{
public:
	template <class T>
	static bool ReadFileToVector(std::fstream& file, std::vector<T>& vector)
	{
		if (!file.is_open()) return false;
		if (vector.capacity() < VECTOR_BUFF) vector.reserve(VECTOR_BUFF);
		T temp;
		while (!file.eof())
		{
			file >> temp;
			vector.emplace_back(std::move(temp));
			file.get();
		}
	}

	template <class T>
	static bool WriteVectorToFile(std::fstream& file, std::vector<T>& vector)
	{
		if (!file.is_open()) return false;
		if (vector.capacity() < VECTOR_BUFF) vector.reserve(VECTOR_BUFF);
		size_t limiter = vector.size() - 1;
		for (size_t i = 0; i < limiter; i++)
		{
			file << vector[i] << '\n';
			std::cout << file.tellg() << '\n';
		}
		file << vector[limiter];
	}

	template <class T>
	static bool ReadFile(const char* path, std::vector<T>& vector)
	{
		std::fstream file(path, std::ios::in);
		bool state = ReadFileToVector(file, vector);
		file.close();
		return state;
	}

	template <class T>
	static bool  WriteToFile(const char* path, std::vector<T>& vector)
	{
		std::fstream file(path, std::ios::out);
		bool state = WriteVectorToFile(file, vector);
		file.close();
		return state;
	}

	template<class T>
	static int GetFileStatus(const char* path, std::vector<T>& vector)
	{
		std::fstream file(path, std::ios::in);
		if (file.is_open())
		{
			int state;
			file.seekg(0, std::ios::end);
			if (file.tellg() > 0)
				state = FileStatus::Opened;
			else
				state = FileStatus::Empty;
			file.close();
			return state;
		}
		else return FileStatus::NotOpened;
	}
	
};
