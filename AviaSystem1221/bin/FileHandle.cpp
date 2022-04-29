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
		if (ch == '\0') break;
		output.push_back(ch);

	} while (!file.eof());
	return output;
}

//������ � ���� ������, � ��� ����� ����-����������
void FileHandle::WriteString(std::fstream& file,const std::string& input)
{
	//length + 1 ������� ��� ������ � ����, ������� ����.
	file.write(input.c_str(), input.length() + 1);
}

bool FileHandle::file_is_empty(std::fstream& file)
{
	std::streampos current = file.tellg();
	file.seekp(0, std::ios::end);
	//���� tellp() ������ 0, ������ ��������� � ������ � ���� ������,
	//�����, ���� �� ������
	bool is_empty = file.tellp() > 0 ? false : true;
	file.seekp(current);
	return is_empty;
}


