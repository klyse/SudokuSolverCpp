#include "stdafx.h"
#include <sstream>
#include "ImageSudokuSolver.h"

bool ImageSudokuSolver::LoadSudoku(string path)
{
	if (path.empty())
		return false;

	char imgPath[255];
	sprintf_s(imgPath, 255, path.c_str());

	_imgAnalyzation.ProcessImage(imgPath);
	auto sudokuVector = _imgAnalyzation.GetSudoku();

	if (sudokuVector.empty())
		return false;

	return _originalSudoku.FillFromVector(sudokuVector);
}

int ImageSudokuSolver::Solver()
{
	const int ret = SudokuSolver::Solver();

	if (ret == 1)
	{
		_imgAnalyzation.DrawSolution(_solvedSudoku.ConvertToVector());
	}

	return ret;
}

string ImageSudokuSolver::GetTimeString()
{
	const string timeStr = SudokuSolver::GetTimeString();

	std::ostringstream stringStream;
	stringStream << timeStr << " Analysation time [ms]: " << _imgAnalyzation.GetTime();
	return stringStream.str();
}
