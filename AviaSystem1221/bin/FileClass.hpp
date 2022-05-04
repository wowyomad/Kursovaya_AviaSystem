#pragma once

#include <fstream>
#include <vector>

static const size_t VECTOR_BUFF = 16384;

enum FileStatus
{
	NotOpened = 0,
	Empty = 1,
	Opened = 2
};

class BaseFile
{
public:
	template <class T>
	void ReadFileToVector(std::fstream& file, std::vector<T> vector)
	{
		
	}
	template <class T>
	void WriteVectorToFile(std::fstream& file, std::vector<T> vector)
	{

	}
	int getFileInfo(std::fstream& file, size_t& fileSize)
	{

	}

};

