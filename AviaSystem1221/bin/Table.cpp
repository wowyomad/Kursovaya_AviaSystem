#include "Table.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

void FormattedOutput::PrintCenteredLine(std::string str, const unsigned width, const char fill)
{
	unsigned offset = width / 2;
	size_t half_length = str.length() / 2;
	std::cout << std::setw(offset) << std::setfill(fill)
		<< str.substr(0, half_length) << str.substr(half_length)
		<< std::setw(offset - str.length() + half_length) << std::setfill(fill) << '\n';
	std::cout << std::setfill(' ');
}

void FormattedOutput::PrintCenteredRow(const std::vector<std::string>& stringVector, const unsigned cellWidth, const int horizontal, const int vertical, const int inner, const int consoleWidth)
{
	if (cellWidth < 3) return;
	unsigned maxLength = [stringVector]()->unsigned
	{
		unsigned maxLength = 0;
		for(const auto &it : stringVector)
		{
			if (it.length() > maxLength)
				maxLength = it.length();
		}
		return maxLength;
	}();
	unsigned int offset = (consoleWidth - cellWidth * stringVector.size()) / 2;
	unsigned size = stringVector.size();
	unsigned lines = maxLength % (cellWidth - 2) == 0 ? 0 : 1;
	lines += maxLength / (cellWidth - 2);
	unsigned line_length = cellWidth - 2;

	char downBorder = (vertical == Bottom || vertical == Both) ? '_' : ' ';
	char leftBorder = (horizontal == Left || horizontal == Both) ? '|' : ' ';
	char rightBorder = (horizontal == Right || horizontal == Both) ? '|' : ' ';
	char innerBottomBorder = inner == Both ? '_' : ' ';
	char innerSideBorder = inner == Both ? '|' : ' ';

	if (vertical == Up || vertical == Both)
	{
		std::cout << std::setw(offset) << std::setfill(' ') << ' ';
		std::cout << ' ';
		for (int i = 0; i < stringVector.size(); i++)
		{
			std::cout << std::setw(cellWidth - 1) << std::setfill('_') << ' ';
		}
		std::cout << '\n';
	}

	std::cout << std::setw(offset) << std::setfill(' ') << ' ';
	std::cout << leftBorder << std::setw(cellWidth - 1) << std::setfill(' ');
	for (int i = 1; i < stringVector.size() - 1; i++)
	{
		std::cout << innerSideBorder << std::setw(cellWidth - 1) << std::setfill(' ');
	}
	if (stringVector.size() > 1)
		std::cout << innerSideBorder;
	std::cout << std::setw(cellWidth - 1) << std::setfill(' ') << rightBorder << '\n';

	unsigned* index = new unsigned[size]{ 0 };
	for (int k = 0; k < lines; k++)
	{
		int m = 0;
		std::cout << std::setw(offset) << std::setfill(' ') << ' ';
		std::cout << leftBorder;
		for (int m = 0; m < size; m++)

		{
			if (stringVector[m][index[m]] == ' ') index[m]++;
			int j = 0;
			while (j < line_length && index[m] < stringVector[m].length())
			{
				std::cout << stringVector[m][index[m]];
				index[m]++;
				j++;
			}
			std::cout << std::setw(line_length - j + 1) << std::setfill(' ');
			if (m == size - 1) std::cout << rightBorder;
			else std::cout << innerSideBorder;
		}
		if (horizontal == Right || horizontal == Both)
			printf("\b \b%c", rightBorder);
		std::cout << '\n';
	}
	std::cout << std::setw(offset) << std::setfill(' ') << ' ';
	std::cout << leftBorder << std::setw(cellWidth - 1) << std::setfill(downBorder);
	for (int i = 1; i < size - 1; i++)
	{
		std::cout << innerSideBorder << std::setw(cellWidth - 1) << std::setfill(downBorder);
	}
	if (size > 1) std::cout << innerSideBorder;
	std::cout << std::setw(cellWidth - 1) << std::setfill(downBorder) << rightBorder << '\n';

	delete[] index;
}
