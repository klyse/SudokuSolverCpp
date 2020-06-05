#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

struct Numbers
{
public:
	int _Number{};
	std::vector<int> posNumbers;

	void Clear()
	{
		_Number = 0;
		posNumbers.clear();
	}

	Numbers()
	{
		Clear();
	}
};


struct Sudoku
{
	Numbers val[9][9];

	void Clear()
	{
		for (auto& y : val)
		{
			for (auto& x : y)
			{
				x.Clear();
			}
		}
	}

	std::vector<int> ConvertToVector() const
	{
		std::vector<int> vector;
		vector.clear();
		for (const auto& y : val)
		{
			for (const auto& x : y)
			{
				vector.push_back(x._Number);
			}
		}
		return vector;
	}

	std::string ConvertToString() const
	{
		std::string str;
		for (int y = 0; y < 9; ++y)
		{
			for (int x = 0; x < 9; ++x)
			{
				str.append(val[y][x]._Number == 0 ? "." : std::to_string(val[y][x]._Number));

				if ((x + 1) % 3 == 0 && x != 8)
				{
					str.append(" ");
				}
			}
			if (y != 8)
				str.append("\n");
		}
		return str;
	}

	bool FillFromVector(std::vector<int> vector)
	{
		if (vector.empty())
		{
			throw std::invalid_argument("vector is empty");
		}
		if (vector.size() != 9 * 9)
			throw std::out_of_range("vector has the wrong size");

		Clear();
		for (int y = 0; y < 9; ++y)
		{
			for (int x = 0; x < 9; ++x)
			{
				val[y][x]._Number = vector[y * 9 + x];
			}
		}

		return true;
	}

	bool FillFromString(std::string value)
	{
		value.erase(remove(value.begin(), value.end(), ' '), value.end());
		value.erase(remove(value.begin(), value.end(), '\r'), value.end());
		value.erase(remove(value.begin(), value.end(), '\n'), value.end());
		std::replace(value.begin(), value.end(), '.', '0');

		if (value.size() != 9 * 9)
			throw std::out_of_range("string has the wrong size");

		Clear();
		for (int y = 0; y < 9; ++y)
		{
			for (int x = 0; x < 9; x++)
			{
				val[y][x]._Number = (short)value[y * 9 + x] - (int)'0';
			}
		}

		return false;
	}

	Sudoku()
	{
		Clear();
	}
};
