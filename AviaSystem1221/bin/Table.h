#pragma once

#include <string>
#include <vector>

#include "common.h"
namespace FormattedOutput
{
	void PrintCenteredLine(std::string str, const unsigned width, const char fill = ' ');
	void PrintCenteredRow(const std::vector<std::string>& vectorString, const unsigned width, const int horizontal = 1, const int vertical = 1, const int inner = 1, const int consoleWidth = CONSOLE_WIDTH);

	enum Border
	{
		NoBorder = 0,
		Both = 1,
		Left = 2,
		Right = 3,
		Up = 4,
		Bottom = 5
	};

	class TableInterface
	{
		virtual void PrintInfo(const int mode, const int& count = 1) = 0;
		virtual void PrintInfoWithTop(const int mode) = 0;
		virtual void PrintInfoWhole(const int mode) = 0;
		virtual void PrintTopRow(const int mode) = 0;
	};
}
