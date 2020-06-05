#pragma once
#include <ctime>
#include "Sudoku.h"

namespace SudokuSolverTest
{
	class SudokuSolverTest;
}

class SudokuSolver
{
public:
	virtual ~SudokuSolver() = default;

	SudokuSolver() : _beginTime(0), _endTime(0), _nrTrials(0)
	{
		_originalSudoku = Sudoku();
	}

	explicit SudokuSolver(const std::string& filePath) : _beginTime(0), _endTime(0), _nrTrials(0)
	{
		_filePath = filePath;
		_originalSudoku = Sudoku();
	}

	explicit SudokuSolver(const Sudoku& sudoku) : _beginTime(0), _endTime(0), _nrTrials(0)
	{
		_originalSudoku = sudoku;
	}

private:
	void CalcPossibleNumbers(Sudoku* sudoku, int x, int y) const;
	void CalcPossibleNumbers(Sudoku* sudoku) const;
	bool IsNumberPossible(Sudoku* sud, int y, int x) const;
	int SetNumber(Sudoku* sud);

public:
	virtual int Solver();
	virtual bool Init() = 0;
	virtual bool LoadSudoku(std::string path) = 0;
	virtual std::string GetTimeString();

	Sudoku GetLoadedSudoku() const
	{
		return _originalSudoku;
	}

	Sudoku GetSolvedSudoku() const
	{
		return _solvedSudoku;
	}

	int GetNrTrials() const
	{
		return _nrTrials;
	}

protected:
	Sudoku _originalSudoku;
	Sudoku _solvedSudoku;
	clock_t _beginTime;
	clock_t _endTime;
	int _nrTrials;
	std::string _filePath;

	friend SudokuSolverTest::SudokuSolverTest;
};
