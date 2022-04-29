#include "FileHandle.h"

#define STRING_BUFF
//Чтение из файла строки до нуль-терминатора
std::string FileHandle::ReadString(std::fstream& file)
{
	char ch;
	std::string output;
	output.reserve(STRING_BUFF);
	do
	{
		file.read((char*)&ch, sizeof(char));
		if (ch == '\0') break;
		output.push_back(ch);

	} while (!file.eof());
	return output;
}

//Запись в файл строки, в том числе нуль-терминатор
void FileHandle::WriteString(std::fstream& file,const std::string& input)
{
	//length + 1 запишет всю строку в файл, включая нуль.
	file.write(input.c_str(), input.length() + 1);
}

bool FileHandle::file_is_empty(std::fstream& file)
{
	std::streampos current = file.tellg();
	file.seekp(0, std::ios::end);
	//Если tellp() вернет 0, значит указатель в начале и файл пустой,
	//иначе, файл не пустой
	bool is_empty = file.tellp() > 0 ? false : true;
	file.seekp(current);
	return is_empty;
}


