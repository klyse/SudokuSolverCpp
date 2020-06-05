#include "stdafx.h"
#include "CppUnitTest.h"
#include <memory>
#include "ImageProcessing.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ImageProcessing;

namespace SudokuSolverTest
{
	TEST_CLASS(ImageProcessingTest)
	{
	public:
		TEST_METHOD(SortRoiPoints_ClockwiseInputTopLeft_InputOutputSame)
		{
			// Arrange
			ImageAnalization processing = ImageAnalization();

			Point2f correctPoint[4] = {
				Point2f(221, 412),
				Point2f(499, 413),
				Point2f(522, 687),
				Point2f(191, 687)
			};
			Point2f inputPoint[4] = {
				Point2f(221, 412),
				Point2f(499, 413),
				Point2f(522, 687),
				Point2f(191, 687)
			};
			// Act
			processing.SortRoiPoints(inputPoint);

			// Assert
			bool correct = true;
			for (int i = 0; i < 4; ++i)
			{
				correct &= correctPoint[i] == inputPoint[i];
			}
			Assert::IsTrue(correct);
		}

		TEST_METHOD(SortRoiPoints_ClockwiseInputTopRight_IsSorted)
		{
			// Arrange
			ImageAnalization processing = ImageAnalization();

			Point2f correctPoint[4] = {
				Point2f(221, 412),
				Point2f(499, 413),
				Point2f(522, 687),
				Point2f(191, 687)
			};
			Point2f inputPoint[4] = {
				Point2f(499, 413),
				Point2f(522, 687),
				Point2f(191, 687),
				Point2f(221, 412),
			};
			// Act
			processing.SortRoiPoints(inputPoint);

			// Assert
			bool correct = true;
			for (int i = 0; i < 4; ++i)
			{
				correct &= correctPoint[i] == inputPoint[i];
			}
			Assert::IsTrue(correct);
		}

		TEST_METHOD(SortRoiPoints_ClockwiseInputBottomRight_IsSorted)
		{
			// Arrange
			ImageAnalization processing = ImageAnalization();

			Point2f correctPoint[4] = {
				Point2f(221, 412),
				Point2f(499, 413),
				Point2f(522, 687),
				Point2f(191, 687)
			};
			Point2f inputPoint[4] = {
				Point2f(522, 687),
				Point2f(191, 687),
				Point2f(221, 412),
				Point2f(499, 413),
			};
			// Act
			processing.SortRoiPoints(inputPoint);

			// Assert
			bool correct = true;
			for (int i = 0; i < 4; ++i)
			{
				correct &= correctPoint[i] == inputPoint[i];
			}
			Assert::IsTrue(correct);
		}

				TEST_METHOD(SortRoiPoints_CounterClockwiseInputBottomRight_IsSorted)
		{
			// Arrange
			ImageAnalization processing = ImageAnalization();

			Point2f correctPoint[4] = {
				Point2f(221, 412),
				Point2f(499, 413),
				Point2f(522, 687),
				Point2f(191, 687)
			};
			Point2f inputPoint[4] = {
				Point2f(522, 687),
				Point2f(499, 413),
				Point2f(221, 412),
				Point2f(191, 687),
			};
			// Act
			processing.SortRoiPoints(inputPoint);

			// Assert
			bool correct = true;
			for (int i = 0; i < 4; ++i)
			{
				correct &= correctPoint[i] == inputPoint[i];
			}
			Assert::IsTrue(correct);
		}

	};
}
