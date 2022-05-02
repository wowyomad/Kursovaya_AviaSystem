#pragma once

#include <string>
#include <vector>
namespace Table
{
	void PrintCenteredLine(std::string str, const unsigned width, const char fill = ' ');
	void PrintRow(const std::vector<std::string>& vectorString, const unsigned width, const int horizontal = 1, const int vertical = 1, const int inner = 1);

	enum Border
	{
		NoBorder = 0,
		Both = 1,
		Left = 2,
		Right = 3,
		Up = 4,
		Bottom = 5
	};
}
