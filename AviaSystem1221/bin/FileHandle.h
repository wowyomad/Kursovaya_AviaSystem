#pragma once

#include <fstream>

namespace FileHandle
{
	std::string ReadString(std::fstream& file);
	void WriteString(std::fstream& file,const std::string& input);
	
	template <class T>
	T ReadVar(std::fstream& file)
	{
		T var;
		file.read((char*)&var, sizeof(var));
		return var;
	}
	
	template <class T>
	void WriteVar(std::fstream& file, const T& var)
	{
		file.write((char*)&var, sizeof(var));
	}

	bool file_is_empty(std::fstream& file);
		
}
