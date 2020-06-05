#include "stdafx.h"
#include "Main.h"
#include <string>
#include <memory>
#include "ImageSudokuSolver.h"
#include "TextSudokuSolver.h"
#include "Sudoku.h"
#include "FileOperations.h"

void PrintSudoku(Sudoku sudoku)
{
	for (auto& y : sudoku.val)
	{
		string str;
		str.clear();
		for (int x = 0; x < 9; ++x)
		{
			if (x % 3 == 0)
				str.append(" ");
			str.append(to_string(y[x]._Number));
			str.append(" ");
		}
		cout << str << endl;
	}

	cout << endl;
	cout << endl;
	cout << endl;
}

int main(int argc, char* argv[])
{
	shared_ptr<SudokuSolver> sudokuSolver;

	//auto filename = string("./_txtExamples/sudoku.txt");
	auto filename = string(format("./_imgExamples/sudoku%d.jpg", TESTIMG_NR));

	if (argc > 1)
		filename = string(argv[1]);
	
	auto fileExtension = string(".txt");
	
	bool useFileImport = std::equal(fileExtension.rbegin(), fileExtension.rend(), filename.rbegin());
	if (useFileImport)
	{
		sudokuSolver = make_shared<TextSudokuSolver>(filename);
	}
	else
	{
		sudokuSolver = make_shared<ImageSudokuSolver>(filename);
	}

	if (!sudokuSolver->Init())
	{
		cout << "failed" << endl;
		cin.get();
		return 0;
	}

	PrintSudoku(sudokuSolver->GetLoadedSudoku());


	const int res = sudokuSolver->Solver();
	//WriteToFile(string("C:/Projects/SudokuSolver/_Executable/_txtExamples/test.txt"), sudoku_solver->GetSolvedSudoku().ConvertToString(), ios::out);

	if (res == 1)
		PrintSudoku(sudokuSolver->GetSolvedSudoku());
	else if (res == -1)
		cout << "out of time - dead end" << endl;
	else
		cout << "failed" << endl;

	cout << sudokuSolver->GetTimeString() << endl;
	cout << "nrTrials " << sudokuSolver->GetNrTrials() << endl;

	cin.get();
	return 0;
}
