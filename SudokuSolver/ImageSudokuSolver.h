#pragma once
#include <string>
#include "ImageProcessing.h"
#include "SudokuSolver.h"

class ImageSudokuSolver : public SudokuSolver
{
private:

	ImageProcessing::ImageAnalization _imgAnalyzation;
public:
	ImageSudokuSolver() = default;

	explicit ImageSudokuSolver(const std::string& filePath) : SudokuSolver(filePath)
	{
		_filePath = filePath;
		_originalSudoku = Sudoku();
	}

	explicit ImageSudokuSolver(const Sudoku& sudoku) : SudokuSolver(sudoku)
	{
	}

private:
	bool LoadSudoku(std::string path) override;

public:
	bool Init() override
	{
		_imgAnalyzation = ImageProcessing::ImageAnalization();
		return ImageSudokuSolver::LoadSudoku(_filePath);
	}

	int Solver() override;
	std::string GetTimeString() override;
};
