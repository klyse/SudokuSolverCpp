#include "stdafx.h"
#include "CppUnitTest.h"
#include "Sudoku.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace SudokuSolverTest
{
	TEST_CLASS(SudokuTest)
	{
	public:
		TEST_METHOD(ConvertToVector_ConvertFromSudokuToVector_IsCorrect)
		{
			// Arrange
			Sudoku sudoku;
			const std::vector<int> vector = {
				7, 5, 0, 4, 0, 0, 1, 2, 0, 0, 2, 0, 1, 7, 0, 0, 5, 6, 9, 0, 0, 2, 5, 0, 0, 0, 0, 0, 0, 8, 0, 4, 0, 2, 9, 0, 0, 0, 9,
				0, 0, 0, 7, 6, 0, 0, 0, 6, 0, 3, 2, 0, 0, 0, 6, 8, 0, 7, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 1, 1, 3, 0, 0, 0, 9,
				0, 0, 2
			};

			// Act
			sudoku.FillFromVector(vector); // this function mus work otherwhise a other test fails (aswell)

			const std::vector<int> converted = sudoku.ConvertToVector();
			// Assert
			Assert::IsTrue(vector == converted);
		}

		TEST_METHOD(ConvertToString_ConvertFromSudokuToString_IsCorrect)
		{
			// Arrange
			Sudoku sudoku;

			const auto string = std::string{
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

			// Act
			sudoku.FillFromString(string); // this function must work otherwise it would fail in other unit test

			const auto converted = sudoku.ConvertToString();

			// Assert
			Assert::IsTrue(string == converted);
		}

		TEST_METHOD(FillFromVector_WrongVectorToShort_ThrowsError)
		{
			// Arrange
			Sudoku sudoku;
			std::vector<int> vector;

			// Act
			const auto func = [&] { sudoku.FillFromVector(vector); };

			// Assert
			Assert::ExpectException<std::invalid_argument>(func);
		}

		TEST_METHOD(FillFromVector_WrongVectorEmpty_ThrowsError)
		{
			// Arrange
			Sudoku sudoku;
			std::vector<int> vector;

			// Act
			vector.push_back(1);
			vector.push_back(2);
			vector.push_back(3);
			const auto func = [&] { sudoku.FillFromVector(vector); };

			// Assert
			Assert::ExpectException<std::out_of_range>(func);
		}

		TEST_METHOD(FillFromVector_WrongVectorToLong_ThrowsError)
		{
			// Arrange
			Sudoku sudoku;
			std::vector<int> vector;

			// Act
			for (int i = 0; i < 9 * 9; ++i)
			{
				vector.push_back(1);
			}
			vector.push_back(1);
			const auto func = [&] { sudoku.FillFromVector(vector); };

			// Assert
			Assert::ExpectException<std::out_of_range>(func);
		}

		TEST_METHOD(FillFromVector_ConvertFromVectorToSudoku_IsCorrect)
		{
			// Arrange
			Sudoku sudoku;
			const std::vector<int> vector = {
				7, 5, 0, 4, 0, 0, 1, 2, 0, 0, 2, 0, 1, 7, 0, 0, 5, 6, 9, 0, 0, 2, 5, 0, 0, 0, 0, 0, 0, 8, 0, 4, 0, 2, 9, 0, 0, 0, 9,
				0, 0, 0, 7, 6, 0, 0, 0, 6, 0, 3, 2, 0, 0, 0, 6, 8, 0, 7, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 1, 1, 3, 0, 0, 0, 9,
				0, 0, 2
			};

			// Act
			sudoku.FillFromVector(vector);

			const std::vector<int> converted = sudoku.ConvertToVector();
			// this function mus work otherwhise a other test fails (aswell)
			// Assert
			Assert::IsTrue(vector == converted);
		}


		TEST_METHOD(FillFromString_VectorToShort_ThrowsError)
		{
			// Arrange
			Sudoku sudoku;
			std::string str = string("asdf");

			// Act
			const auto func = [&] { sudoku.FillFromString(str); };

			// Assert
			Assert::ExpectException<std::out_of_range>(func);
		}

		TEST_METHOD(FillFromString_VectorToLong_ThrowsError)
		{
			// Arrange
			Sudoku sudoku;
			std::string str = string("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd");
			//82 chars

			// Act
			const auto func = [&] { sudoku.FillFromString(str); };

			// Assert
			Assert::ExpectException<std::out_of_range>(func);
		}

		TEST_METHOD(FillFromString_ConvertFromStringToSudoku_IsCorrect)
		{
			// Arrange
			Sudoku sudoku;
			const auto string = std::string{
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

			// Act
			sudoku.FillFromString(string);

			const auto converted = sudoku.ConvertToString();
			// this function must work otherwise it would fail in other unit test

			// Assert
			Assert::IsTrue(string == converted);
		}
	};
}
