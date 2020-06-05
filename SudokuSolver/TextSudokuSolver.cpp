#include "stdafx.h"
#include "FileOperations.h"
#include "TextSudokuSolver.h"

bool TextSudokuSolver::LoadSudoku(std::string path)
{
	if (path.empty())
		return true;

	auto sudokuString = std::string();

	ReadFromFile(path, sudokuString);

	_originalSudoku.FillFromString(sudokuString);

	return true;
}
