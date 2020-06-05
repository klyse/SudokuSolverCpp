#include "stdafx.h"
#include <string>
#include <vector>
#include "SudokuSolver.h"
#include <ctime>
#include "ImageProcessing.h"
#include "Main.h"


void SudokuSolver::CalcPossibleNumbers(Sudoku* sudoku, int x, int y) const
{
	NULLPTR_CHECK(sudoku);
	OUTOFRANGE_CHECK(x, 0, 9);
	OUTOFRANGE_CHECK(y, 0, 9);

	Numbers* currNumber = &sudoku->val[y][x];

	currNumber->posNumbers.clear();
	// if the field is to be filled
	if (currNumber->_Number == 0)
	{
		// check y line
		bool exists[10] = {false};
		for (auto& yy : sudoku->val)
		{
			exists[yy[x]._Number] = true;
		}
		// check x line
		for (int xx = 0; xx < 9; ++xx)
		{
			exists[sudoku->val[y][xx]._Number] = true;
		}
		// check square
		for (int yy = int(y / 3) * 3; yy < int(y / 3) * 3 + 3; ++yy)
		{
			for (int xx = int(x / 3) * 3; xx < int(x / 3) * 3 + 3; ++xx)
			{
				exists[sudoku->val[yy][xx]._Number] = true;
			}
		}

		// add _Number to possible solutions
		for (int i = 1; i < 10; ++i)
		{
			if (!exists[i])
				currNumber->posNumbers.push_back(i);
		}

		// if only one valid _Number, must be the right one
		if (currNumber->posNumbers.size() == 1)
			currNumber->_Number = currNumber->posNumbers[0];
	}
}

void SudokuSolver::CalcPossibleNumbers(Sudoku* sudoku) const
{
	NULLPTR_CHECK(sudoku);


	// for all fields
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			CalcPossibleNumbers(sudoku, x, y);
		}
	}
}

bool SudokuSolver::IsNumberPossible(Sudoku* sud, const int y, const int x) const
{
	NULLPTR_CHECK(sud);
	OUTOFRANGE_CHECK(x, 0, 9);
	OUTOFRANGE_CHECK(y, 0, 9);

	{
		// check on y line
		bool exists[10] = {false};
		for (auto& yy : sud->val)
		{
			if (yy[x]._Number > 0)
				if (exists[yy[x]._Number])
					return false;

			exists[yy[x]._Number] = true;
		}
	}

	{
		// check on x line
		bool exists[10] = {false};
		for (int xx = 0; xx < 9; ++xx)
		{
			if (sud->val[y][xx]._Number > 0)
				if (exists[sud->val[y][xx]._Number])
					return false;

			exists[sud->val[y][xx]._Number] = true;
		}
	}

	{
		// check in square
		bool exists[10] = {false};
		for (int yy = int(y / 3) * 3; yy < int(y / 3) * 3 + 3; ++yy)
		{
			for (int xx = int(x / 3) * 3; xx < int(x / 3) * 3 + 3; ++xx)
			{
				if (sud->val[yy][xx]._Number > 0)
					if (exists[sud->val[yy][xx]._Number])
						return false;

				exists[sud->val[yy][xx]._Number] = true;
			}
		}
	}
	return true;
}

int SudokuSolver::SetNumber(Sudoku* sud)
{
	NULLPTR_CHECK(sud);

	// cycle over all fields
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			Numbers* currNumber = &sud->val[y][x];

			// field is to be filled
			if (currNumber->_Number == 0)
			{
				// calculate all possible numbers
				CalcPossibleNumbers(sud);

				bool trueSol = false;
				// cycle over possible numbers
				for (size_t i = 0; i < currNumber->posNumbers.size(); i++)
				{
					// _Number was classified as bad path
					if (currNumber->posNumbers[i] == 0)
						continue;

					currNumber->_Number = currNumber->posNumbers[i];
					// validate _Number / position
					if (IsNumberPossible(sud, y, x))
					{
						// check for dead end
#ifdef _DEBUG
						if (clock() - _beginTime > 10000)
							return -1;
#endif
						_nrTrials++; // count trials

						trueSol = true; // at least one _Number is okay
						Sudoku copy = *sud;
						// call recursive function
						if (SetNumber(&copy) == 1)
						{
							// ojah.. found a solution, copy it recursively
							*sud = copy;
							return 1;
						}
					}
					else // classify _Number as bad path
						currNumber->posNumbers[i] = 0;
				}
				if (!trueSol)
					return 0;
			}
		}
	}

	return 1;
}

int SudokuSolver::Solver()
{
	_solvedSudoku = _originalSudoku;

	_beginTime = clock();

	const auto ret = SetNumber(&_solvedSudoku);

	_endTime = clock();

	return ret;
}

string SudokuSolver::GetTimeString()
{
	if (_endTime == 0 || _beginTime == 0)
		throw logic_error("Solve algorithm not yet started / ended");

	ostringstream stringStream;
	stringStream << "Solve time [ms]: " << _endTime - _beginTime;
	return stringStream.str();
}
