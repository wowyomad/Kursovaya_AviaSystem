#include "FileHandle.h"

#define STRING_BUFF



//������ �� ����� ������ �� ����-�����������
std::string FileHandle::ReadString(std::fstream& file)
{
	char ch;
	std::string output;
	output.reserve(STRING_BUFF);
	do
	{
		file.read((char*)&ch, sizeof(char));
		if (ch != '\0') output.push_back(ch);
		else break;

	} while (!file.eof());
	return output;
}

//������ � ���� ������, � ��� ����� ����-����������
void FileHandle::WriteString(std::fstream& file, const std::string& input)
{
	//length + 1 = ������ + ����
	file.write(input.c_str(), input.length() + 1);
}


int getFileStatus(std::fstream& file)
{
	if (file.is_open())
	{
		std::streampos current = file.tellg();
		file.seekp(0, std::ios::end);
		int status = file.tellp() > 0 ? FileStatus::Opened : FileStatus::Empty;
		file.seekp(current);
		return status;
	}
	else return FileStatus::NotOpened;
}



