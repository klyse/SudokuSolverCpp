#pragma once
#include <string>
#include "SudokuSolver.h"

class TextSudokuSolver : public SudokuSolver
{
public:
	TextSudokuSolver() = default;

	explicit TextSudokuSolver(const std::string& filePath) : SudokuSolver(filePath)
	{
	}

	explicit TextSudokuSolver(const Sudoku& sudoku) : SudokuSolver(sudoku)
	{
	}

	bool Init() override
	{
		return TextSudokuSolver::LoadSudoku(_filePath);
	}

	bool LoadSudoku(std::string path) override;
};
