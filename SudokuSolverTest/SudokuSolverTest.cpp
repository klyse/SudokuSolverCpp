#include "stdafx.h"
#include "CppUnitTest.h"
#include <memory>
#include "SudokuSolver.h"
#include "TextSudokuSolver.h"
#include "ImageSudokuSolver.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SudokuSolverTest
{
	TEST_CLASS(SudokuSolverTest)
	{
	public:
		TEST_METHOD(CalcpossibleNumers_UninitializedSudoku_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func = [&] { sudokuSolver->CalcPossibleNumbers(nullptr); };

			// Assert
			Assert::ExpectException<std::invalid_argument>(func);
		}

		TEST_METHOD(CalcpossibleNumers2_UninitializedSudoku_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func = [&] { sudokuSolver->CalcPossibleNumbers(nullptr, 0, 0); };

			// Assert
			Assert::ExpectException<std::invalid_argument>(func);
		}

		TEST_METHOD(CalcpossibleNumers_OutOfRangeXPossitive_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func1 = [&]
			{
				auto sud = Sudoku();
				sudokuSolver->CalcPossibleNumbers(&sud, 9, 0);
			};

			// Assert
			Assert::ExpectException<std::out_of_range>(func1);
		}

		TEST_METHOD(CalcpossibleNumers_OutOfRangeXNegative_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func1 = [&]
			{
				auto sud = Sudoku();
				sudokuSolver->CalcPossibleNumbers(&sud, -10, 0);
			};

			// Assert
			Assert::ExpectException<std::out_of_range>(func1);
		}

		TEST_METHOD(CalcpossibleNumers_OutOfRangeYPossitive_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func1 = [&]
			{
				auto sud = Sudoku();
				sudokuSolver->CalcPossibleNumbers(&sud, 0, 9);
			};

			// Assert
			Assert::ExpectException<std::out_of_range>(func1);
		}

		TEST_METHOD(CalcpossibleNumers_OutOfRangeYNegative_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func1 = [&]
			{
				auto sud = Sudoku();
				sudokuSolver->CalcPossibleNumbers(&sud, 0, -10);
			};

			// Assert
			Assert::ExpectException<std::out_of_range>(func1);
		}

		TEST_METHOD(IsNumberpossible_UninitializedSudoku_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func = [&]
			{
				sudokuSolver->IsNumberPossible(nullptr, 0, 0);
			};

			// Assert
			Assert::ExpectException<std::invalid_argument>(func);
		}

		TEST_METHOD(IsNumberpossible_OutOfRangeXPossitive_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func1 = [&]
			{
				auto sud = Sudoku();
				sudokuSolver->IsNumberPossible(&sud, 9, 0);
			};

			// Assert
			Assert::ExpectException<std::out_of_range>(func1);
		}

		TEST_METHOD(IsNumberpossible_OutOfRangeXNegative_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func1 = [&]
			{
				auto sud = Sudoku();
				sudokuSolver->IsNumberPossible(&sud, -10, 0);
			};

			// Assert
			Assert::ExpectException<std::out_of_range>(func1);
		}

		TEST_METHOD(IsNumberpossible_OutOfRangeYPossitive_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func1 = [&]
			{
				auto sud = Sudoku();
				sudokuSolver->IsNumberPossible(&sud, 0, 9);
			};

			// Assert
			Assert::ExpectException<std::out_of_range>(func1);
		}

		TEST_METHOD(IsNumberpossible_OutOfRangeYNegative_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func1 = [&]
			{
				auto sud = Sudoku();
				sudokuSolver->IsNumberPossible(&sud, 0, -10);
			};

			// Assert
			Assert::ExpectException<std::out_of_range>(func1);
		}

		TEST_METHOD(SetNumber_UninitializedSudoku_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func = [&] { sudokuSolver->SetNumber(nullptr); };

			// Assert
			Assert::ExpectException<std::invalid_argument>(func);
		}

		TEST_METHOD(GetTimeString_SolveAlgorythmNotStartedButTimeRequested_ThrowsError)
		{
			// Arrange
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<ImageSudokuSolver>();

			// Act
			const auto func = [&] { sudokuSolver->GetTimeString(); };

			// Assert
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(Solver_SolveSudoku1_IsCorrect)
		{
			// Arrange
			Sudoku sudoku;
			const auto input = std::string{
				"... 217 ...\n"
				".5. ... ..1\n"
				".4. ... ..2\n"
				"..6 .7. 9..\n"
				"..2 ... 6..\n"
				"..7 .85 ...\n"
				"8.. 4.6 ...\n"
				"... 89. .5.\n"
				"9.. ... .4."
			};

			const auto solved = std::string{
				"369 217 584\n"
				"258 934 761\n"
				"741 568 392\n"
				"486 372 915\n"
				"532 149 678\n"
				"197 685 423\n"
				"875 426 139\n"
				"614 893 257\n"
				"923 751 846"
			};

			// Act
			sudoku.FillFromString(input);
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<TextSudokuSolver>(sudoku);

			sudokuSolver->Init();
			sudokuSolver->Solver();

			// Assert
			const auto solverSolved = sudokuSolver->GetSolvedSudoku().ConvertToString();
			Assert::IsTrue(solverSolved == solved);
		}

		TEST_METHOD(Solver_SolveSudoku2_IsCorrect)
		{
			// Arrange
			Sudoku sudoku;
			const auto input = std::string{
				"..8 .73 5..\n"
				"... ..5 34.\n"
				"37. 021 ...\n"
				"..2 ... 951\n"
				"9.6 ... ...\n"
				"... 75. ...\n"
				"841 ... ...\n"
				"... ... .28\n"
				"... 61. ..."
			};

			const auto solved = std::string{
				"468 973 512\n"
				"219 865 347\n"
				"375 421 869\n"
				"782 346 951\n"
				"956 182 473\n"
				"134 759 286\n"
				"841 297 635\n"
				"697 534 128\n"
				"523 618 794"
			};

			// Act
			sudoku.FillFromString(input);
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<TextSudokuSolver>(sudoku);

			sudokuSolver->Init();
			sudokuSolver->Solver();

			// Assert
			const auto solverSolved = sudokuSolver->GetSolvedSudoku().ConvertToString();
			Assert::IsTrue(solverSolved == solved);
		}

		TEST_METHOD(Solver_SolveSudoku1_IsNotCorrect)
		{
			// Arrange
			Sudoku sudoku;
			const auto input = std::string{
				"... 217 ...\n"
				".5. ... ..1\n"
				".4. ... ..2\n"
				"..6 .7. 9..\n"
				"..2 ... 6..\n"
				"..7 .85 ...\n"
				"8.. 4.6 ...\n"
				"... 89. .5.\n"
				"9.. ... .4."
			};

			const auto solved = std::string{
				"369 217 584\n"
				"258 934 761\n"
				"741 568 392\n"
				"486 372 915\n"
				"532 149 678\n"
				"197 685 423\n"
				"875 426 139\n"
				"614 893 257\n"
				"923 751 847"
			}; //"923 751 846"}; last nr

			// Act
			sudoku.FillFromString(input);
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<TextSudokuSolver>(sudoku);

			sudokuSolver->Init();
			sudokuSolver->Solver();

			// Assert
			const auto solverSolved = sudokuSolver->GetSolvedSudoku().ConvertToString();
			Assert::IsFalse(solverSolved == solved);
		}

		TEST_METHOD(Solver_SolveSudoku2_IsNotCorrect)
		{
			// Arrange
			Sudoku sudoku;
			const auto input = std::string{
				"..8 .73 5..\n"
				"... ..5 34.\n"
				"37. 021 ...\n"
				"..2 ... 951\n"
				"9.6 ... ...\n"
				"... 75. ...\n"
				"841 ... ...\n"
				"... ... .28\n"
				"... 61. ..."
			};

			const auto solved = std::string{
				"468 973 512\n"
				"219 865 347\n"
				"375 421 867\n" //"375 421 869\n" last nr
				"782 346 951\n"
				"956 182 473\n"
				"134 759 286\n"
				"841 297 635\n"
				"697 534 128\n"
				"523 618 794"
			};

			// Act
			sudoku.FillFromString(input);
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<TextSudokuSolver>(sudoku);

			sudokuSolver->Init();
			sudokuSolver->Solver();

			// Assert
			const auto solverSolved = sudokuSolver->GetSolvedSudoku().ConvertToString();
			Assert::IsFalse(solverSolved == solved);
		}

		TEST_METHOD(Solver_ImpossibleToSolve_ReturnsFalse)
		{
			// Arrange
			Sudoku sudoku;
			const auto input = std::string{
				"..8 .73 5..\n"
				".5. ..5 34.\n"
				"37. 021 ...\n"
				"..2 ... 951\n"
				"9.6 ... ...\n"
				"... 75. ...\n"
				"841 ... ...\n"
				"... ... .28\n"
				"... 61. ..."
			};

			// Act
			sudoku.FillFromString(input);
			shared_ptr<SudokuSolver> sudokuSolver = make_shared<TextSudokuSolver>(sudoku);

			sudokuSolver->Init();

			// Assert
			Assert::IsTrue(sudokuSolver->Solver() == 0);
		}
	};
}
