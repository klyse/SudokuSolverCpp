#pragma once
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>

// Leptonica
#pragma warning( push )
#pragma warning( disable : 4305)
#include <leptonica/allheaders.h>
#pragma warning( pop )

// Tesseract
#include <tesseract/baseapi.h>

#pragma comment(lib, "leptonica-1.74.4.lib")

#ifdef _DEBUG
#pragma comment (lib, "opencv_world331d.lib")
#pragma comment (lib, "tesseract305d.lib")
#else
#pragma comment (lib, "opencv_world331.lib")
#pragma comment (lib, "tesseract305.lib")
#endif

namespace SudokuSolverTest {
	class ImageProcessingTest;
}

using namespace cv;
using namespace std;

namespace ImageProcessing
{
	struct SingleImage
	{
		Mat _Img;
		Rect _Pos;

		SingleImage(const Mat& image, const Rect& roi)
		{
			_Img = image;
			_Pos = roi;
		}

		SingleImage() = default;
	};

	struct ConnectedElements
	{
		vector<Point2i> _Points;
		Rect _Rect;
		int _Value;
		int _PtInsideCenter;

		ConnectedElements(const vector<Point2i>& pt, const Rect& inRect, const int val, const int ptInside)
		{
			_Points = pt;
			_Rect = inRect;
			_Value = val;
			_PtInsideCenter = ptInside;
		}

		struct Comparer
		{
			bool operator ()(const ConnectedElements con1, const ConnectedElements con2) const
			{
				return con1._Value > con2._Value;
			}
		};
	};

	class ImageAnalization
	{
		// Constructor
	public:
		ImageAnalization() : _tessApi(nullptr), _beginTime(0), _endTime(0)
		{
		}

		// Enums
	public:
		enum OperationImages
		{
			Warped,
			Binarized,
			MorphologicalOperation,
			RectangleInformation,
			NumOperationImages
		};

	private:
		// Variables
		Mat _sourceImage;
		tesseract::TessBaseAPI* _tessApi;
		vector<int> _sudoku; // Contains analized numbers from image, will be forwared to solver
		Mat _operationImages[NumOperationImages]; // This struct is used for debugging purposes
		SingleImage _singleImages[9 * 9];
		clock_t _beginTime, _endTime;

		// For warp
		// Input Quadilateral or Image plane coordinates
		Point2f _inputQuad[4] = {0};
		// Output Quadilateral or World plane coordinates
		Point2f _outputQuad[4] = {0};
		int _roiPos = 0;
	public:
		// Methods and Functions
		Mat* GetSourceImage() { return &_sourceImage; }
		Mat* GetOperationImage(int idx) { return &_operationImages[idx]; }
		vector<int> GetSudoku() const { return _sudoku; }

		void ProcessImage(char* path);
		void DrawSolution(vector<int> solution);

		void SetBeginTime() { _beginTime = clock(); }
		void SetEndTime() { _endTime = clock(); }
		int GetTime() const { return _endTime - _beginTime; }


	private:
		SingleImage* GetSingleImage(int idx) { return &_singleImages[idx]; }
		void SortRoiPoints(Point2f input[4]);
		void CropImageToRoi(Mat img);
		void DrawRoiOnImage(const Mat& img, int pos) const;

		vector<ConnectedElements> FindConnectedParts(const Mat& inputImg) const;
		tuple<string, int> RunTesseract(Mat img) const;
		void SaveImage(const Mat& img, const string& recognizedValue, int confidence) const;
		void EvaluateNumberSegments();
		void SegmentSingleNumbers(Mat img);

		void SetDebugPosition(int* pos)
		{
			for (int i = 0; i < 8; i += 2)
			{
				_inputQuad[i / 2].x = (float)pos[i];
				_inputQuad[i / 2].y = (float)pos[i + 1];
			}
			_roiPos = 5;
		}

		friend void CallBackFunc(int event, int x, int y, int flags, void* userdata);
		friend void OnTrackBarNumberChanged(int sliderValue, void* userData);
		friend void OnTrackbarOperationImagesChanged(int sliderValue, void* userData);

		friend SudokuSolverTest::ImageProcessingTest;
	};

	void CallBackFunc(int event, int x, int y, int flags, void* userdata);
	void OnTrackBarNumberChanged(int sliderValue, void* userData);
	void OnTrackbarOperationImagesChanged(int sliderValue, void* userData);
}
